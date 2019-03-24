#pragma once

#include "JsonParserUtils.hpp"

#include <map>
#include <iostream>

template <typename I>
class InterfaceExposingHelper;

class JsonParser
{
public:
	template <typename T>
	static auto parsedObject(std::string const& filePath) -> std::optional<T>;
	template <typename T>
	static auto parsedList(std::string const& filePath) -> std::optional<std::vector<T>>;
private:
	template <typename T>
	static auto parsedObjectImpl(std::string_view) -> T;
	template <typename T>
	static auto parsedListImpl(std::string_view) -> std::vector<T>;
private:
	using OptPair = std::optional<std::pair<std::string_view, int>>;
	using ValueExtractor = std::function<OptPair(std::string_view, int)>;
	static auto rawLines(std::string const&) -> std::vector<std::string>;
	static auto filteredLines(std::vector<std::string> const&) -> std::vector<std::string>;
	static auto content(std::vector<std::string> const&) -> std::string;
	static auto extract(std::string_view, char open, char close, int from = 0) -> OptPair;
	static auto valueExtractors() -> std::vector<ValueExtractor> const&;
	static auto eq(std::string_view, std::string_view, int) -> OptPair;
private:
	template <typename M>
	using Getter = std::function<std::optional<M>(std::string_view)>;
	template <typename M>
	static auto getters() -> std::vector<Getter<M>> const&;
	template <typename M>
	static auto get(std::string_view) -> std::optional<M>;
private:
	static auto objectContent(std::string_view) -> std::optional<std::string_view>;
	static auto listContent(std::string_view) -> std::optional<std::string_view>;
	static auto map(std::string_view) -> std::map<std::string_view, std::string_view>;
private:
	template <typename T>
	friend class Exposable;
	template <typename I>
	friend class InterfaceExposingHelper;
};

template <typename I>
class InterfaceExposingHelper
{
public:
	using Condition = std::function<bool()>;
	using Action = std::function<std::unique_ptr<I>(std::string_view)>;
public:
	template <typename C>
	void add_mapping(Condition);
	auto tag(std::string_view tagName) const -> std::string_view;
public:
	auto get_result(std::string_view d) const -> std::unique_ptr<I>;
private:
	std::vector<std::pair<Condition, Action>> m_condition2action;
	mutable std::map<std::string_view, std::string_view> m_tag2val;
};

template <typename I> template <typename C>
void InterfaceExposingHelper<I>::add_mapping(Condition c)
{
	m_condition2action.push_back({c, [](std::string_view d) -> std::unique_ptr<I>{
		return std::make_unique<C>(JsonParser::parsedObjectImpl<C>(d));
	}});
}

template <typename I>
auto InterfaceExposingHelper<I>::tag(std::string_view tagName) const -> std::string_view
{
	auto const it = m_tag2val.find(tagName);
	if (it == m_tag2val.cend()){
		std::cerr << JsonParserUtils::tagNotFoundError(tagName) << std::endl;
		return {};
	}
	return it->second;
}

template <typename I>
auto InterfaceExposingHelper<I>::get_result(std::string_view d) const -> std::unique_ptr<I>
{
	m_tag2val = JsonParser::map(d);
	for (auto const& [c, a] : m_condition2action){
		if (c()){
			m_tag2val = std::map<std::string_view, std::string_view>();
			return a(d);
		}
	}
	std::cerr << JsonParserUtils::interfaceHelperError(d) << std::endl;
	m_tag2val = std::map<std::string_view, std::string_view>();
	return {};
}

template <typename T>
class Exposable
{
public:
	using JsonTag = std::string_view;
	template <typename M>
	static void expose(JsonTag const&, M T::*);
	template <typename H>
	static void register_helper();
private:
	using FieldFiller = std::function<void(T& o, std::string_view)>;
	using Schema = std::map<JsonTag, FieldFiller>;
	static auto schema() -> Schema const&;
	static void unexpose();
	static bool has_helper();
	static auto get_helper() -> InterfaceExposingHelper<T> const&;
private:
	static std::optional<Schema> m_opt_schema;
	static std::unique_ptr<InterfaceExposingHelper<T>> m_helper;
private:
	friend class JsonParser;
};

template <typename T>
auto JsonParser::parsedObject(std::string const& c) -> std::optional<T>
{
	auto const cc = content(filteredLines(rawLines(c)));
	auto const opt_oc = objectContent(cc);
	if (!opt_oc.has_value()) return {};
	return parsedObjectImpl<T>(opt_oc.value());
}

template <typename T>
auto JsonParser::parsedList(std::string const& c) -> std::optional<std::vector<T>>
{
	auto const cc = content(filteredLines(rawLines(c)));
	auto const opt_lc = listContent(cc);
	if (!opt_lc.has_value()) return {};
	return parsedListImpl<T>(opt_lc.value());
}

template <typename T>
auto JsonParser::parsedObjectImpl(std::string_view oc) -> T
{
	JsonParserUtils::exposable<T>();
	JsonParserUtils::defaultConstructable<T>();
	auto const m = map(oc);
	auto result = T();
	auto const& schema = Exposable<T>::schema();
	for (auto const& [expectedTag, f] : schema)
		if (m.find(expectedTag) != m.end())
			f(result, m.at(expectedTag));
	Exposable<T>::unexpose();
	return result;
}

template <typename T>
auto JsonParser::parsedListImpl(std::string_view lc) -> std::vector<T>
{
	JsonParserUtils::defaultConstructable<T>();
	auto result = std::vector<T>();
	int i = 0;
	for (auto const& ve : valueExtractors()){
		while (auto const opt_res = ve(lc, i)){
			auto const [v, j] = opt_res.value();
			auto opt_elem = get<T>(v);
			if (!opt_elem){
				std::cerr << JsonParserUtils::parseError(v) << std::endl;
				i = j + 1;
				continue;
			}
			result.push_back(std::move(opt_elem.value()));
			i = j + 1;
		}
	}
	return result;
}

template <typename M>
auto JsonParser::getters() -> std::vector<Getter<M>> const&
{
	static auto const result = std::vector<Getter<M>>{
		[](std::string_view d) -> std::optional<M>{
			if constexpr (JsonParserUtils::is_ord_container_v<M>)
				return JsonParserUtils::move_vector_into_ord<M>(
						JsonParser::parsedListImpl<JsonParserUtils::element_type_t<M>>(d));
			return std::nullopt;
		},
		[](std::string_view d) -> std::optional<M>{
			if constexpr (JsonParserUtils::is_exposable_v<M>)
				return JsonParser::parsedObjectImpl<M>(d);
			return std::nullopt;
		},
		[](std::string_view d) -> std::optional<M>{
			if constexpr (std::is_same_v<M, std::string>)
				return std::string(d);
			return std::nullopt;
		},
		[](std::string_view d) -> std::optional<M>{
			if constexpr (std::is_same_v<M, bool>)
				return d == "true";
			return std::nullopt;
		},
		[](std::string_view d) -> std::optional<M>{
			if constexpr (std::is_signed_v<M>)
				return JsonParserUtils::safelyConvertedArithmetic<M>(d.data(), OVERLOADS_OF(std::stoll));
			return std::nullopt;
		},
		[](std::string_view d) -> std::optional<M>{
			if constexpr (std::is_unsigned_v<M>)
				return JsonParserUtils::safelyConvertedArithmetic<M>(d.data(), OVERLOADS_OF(std::stoull));
			return std::nullopt;
		},
		[](std::string_view d) -> std::optional<M>{
			if constexpr (JsonParserUtils::is_unique_ptr_v<M>){
				using MemberType = JsonParserUtils::dereferenced_type<M>;
				if constexpr (JsonParserUtils::is_exposable_v<MemberType>){
					if (Exposable<MemberType>::has_helper()){
						auto const& helper = Exposable<MemberType>::get_helper();
						return helper.get_result(d);
					}
				}
				auto opt_res = get<MemberType>(d);
				if (!opt_res) return std::nullopt;
				return std::make_unique<MemberType>(std::move(opt_res.value()));
			}
			return std::nullopt;
		}
	};
	return result;
}

template <typename M>
auto JsonParser::get(std::string_view c) -> std::optional<M>
{
	for (auto const& g : getters<M>())
		if (auto opt_res = g(c))
			return opt_res;
	return std::nullopt;
}

template <typename T>
auto Exposable<T>::schema() -> Schema const&
{
	if (!m_opt_schema.has_value()){
		m_opt_schema = Schema();
		T::expose();
	}
	return m_opt_schema.value();
}

template <typename T> template <typename M>
void Exposable<T>::expose(JsonTag const& k, M T::* p)
{
	auto const f = [p, k](T& o, std::string_view d){
		auto opt_res = JsonParser::get<M>(d);
		if (!opt_res){
			std::cerr << JsonParserUtils::ignoreTagError(k) << std::endl;
			std::cerr << JsonParserUtils::parseError(d) << std::endl;
			return;
		}
		JsonParserUtils::assign(o, p, opt_res.value());
	};
	m_opt_schema->emplace(k, f);
}

template <typename T>
void Exposable<T>::unexpose()
{
	m_opt_schema = std::nullopt;
}

template <typename T> template <typename H>
void Exposable<T>::register_helper()
{
	if (m_helper)
		std::cerr << JsonParserUtils::reinitHelperError() << std::endl;
	m_helper = std::make_unique<H>();
}

template <typename T>
bool Exposable<T>::has_helper()
{
	return m_helper != nullptr;
}

template <typename T>
auto Exposable<T>::get_helper() -> InterfaceExposingHelper<T> const&
{
	return *m_helper;
}

template <typename T>
std::optional<typename Exposable<T>::Schema> Exposable<T>::m_opt_schema;

template <typename T>
std::unique_ptr<InterfaceExposingHelper<T>> Exposable<T>::m_helper;

#pragma once

#include <string>
#include <vector>
#include <optional>
#include <map>

class JsonParser
{
public:
	using FilePath = std::string;
	JsonParser(FilePath const&);
	template <typename T>
	auto parsedObject() -> std::optional<T>;
	using Content = std::string;
	template <typename T>
	static auto parsedObject(Content const&) -> T;
private:
	using LinesList = std::vector<std::string>;
	static auto rawLines(FilePath const&) -> LinesList;
	static auto filteredLines(LinesList const&) -> LinesList;
	static auto content(LinesList const&) -> Content;
private:
	static auto objectContent(Content const&) -> std::optional<Content>;
	using Map = std::map<std::string, std::string>;
	static auto map(Content const&) -> Map;
private:
	static auto map(Content const&, Map&, int i = 0) -> void;
private:
	Content const m_file_content;
};

template <typename T>
class Exposable
{
private:
	virtual void expose() const = 0;
public:
	using JsonTag = std::string;
	using FieldFiller = std::function<void(T& o, std::string const&)>;
	using Schema = std::map<JsonTag, FieldFiller>;
	auto schema() const -> Schema const&;
protected:
	template <typename M>
	void field(JsonTag const&, M T::*) const;
private:
	static std::optional<Schema> m_opt_schema;
};

template <typename T>
auto JsonParser::parsedObject() -> std::optional<T>
{
	const auto opt_oc = objectContent(m_file_content);
	if (!opt_oc.has_value()) return {};
	return parsedObject<T>(opt_oc.value());
}

namespace JsonParserUtils
{
	template <typename T>
	static constexpr bool is_exposable_v =
		std::is_base_of<Exposable<T>, T>::value;
}

template <typename T>
std::optional<typename Exposable<T>::Schema> Exposable<T>::m_opt_schema;

template <typename T>
auto Exposable<T>::schema() const -> Schema const&
{
	if (!m_opt_schema.has_value()){
		m_opt_schema = Schema();
		expose();
	}
	return m_opt_schema.value();
}

template <typename T>
auto JsonParser::parsedObject(Content const& oc) -> T
{
	static_assert(JsonParserUtils::is_exposable_v<T>,
		"An object should be exposable so as to be parsed");
	const auto m = map(oc);
	auto result = T();
	auto const& schema = result.schema();
	for (auto const& [expectedTag, f] : schema)
		f(result, m.at(expectedTag));
	return result;
}

namespace Detail
{
	template <typename M>
	M get(std::string const& d)
	{
		if constexpr (std::is_same_v<M, std::string>)
			return d;
		else if constexpr (std::is_same_v<M, int>)
			return std::stoi(d);
		else if constexpr (JsonParserUtils::is_exposable_v<M>)
			return JsonParser::parsedObject<M>(d);
		else
			return {};
	}
}

template <typename T> template <typename M>
void Exposable<T>::field(JsonTag const& k, M T::* p) const
{
	const auto l = [p](T& o, std::string const& d){o.*p = Detail::get<M>(d);};
	m_opt_schema->emplace(k, l);
}

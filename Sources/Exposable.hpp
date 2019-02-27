#pragma once

#include "JsonParser.hpp"

#include <string>
#include <optional>
#include <map>
#include <functional>

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

template <typename T> template <typename M>
void Exposable<T>::field(JsonTag const& k, M T::* p) const
{
	const auto l = [p](T& o, std::string const& d){o.*p = M();};
	m_opt_schema->emplace(k, l);
}

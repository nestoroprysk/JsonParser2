#include "JsonParser.hpp"

#include <fstream>

auto JsonParser::rawLines(std::string const& fp) -> std::vector<std::string>
{
	std::vector<std::string> result;
	auto ifs = std::ifstream(fp);
	for (std::string line; std::getline(ifs, line);)
		result.push_back(std::move(line));
	return result;
}

auto JsonParser::filteredLines(std::vector<std::string> const& ls) -> std::vector<std::string>
{
	std::vector<std::string> result;
	for (auto const& l : ls){
		std::string line;
		for (auto c : l)
			if (!std::isspace(c))
				line += c;
		result.push_back(std::move(line));
	}
	return result;
}

auto JsonParser::content(std::vector<std::string> const& ls) -> std::string
{
	std::string result;
	for (auto const& l : ls)
		result += l;
	return result;
}

auto JsonParser::objectContent(std::string const& c) -> std::optional<std::string>
{
	const auto opt_res = extract(c, '{', '}');
	if (!opt_res) return {};
	return opt_res->first;
}

auto JsonParser::listContent(std::string const& c) -> std::optional<std::string>
{
	const auto opt_res = extract(c, '[', ']');
	if (!opt_res) return {};
	return opt_res->first;
}

auto JsonParser::extract(std::string const& c, char open, char close, int from) -> OptPair
{
	if (c.empty()) return {};
	if (from >= c.size()) return {};
	if (c[from] != open) return {};
	auto nbToClose = 1;
	for (auto i = from + 1; i < c.size(); ++i){
		const auto e = c[i];
		if (e == close)
			--nbToClose;
		else if (e == open)
			++nbToClose;
		if (nbToClose == 0)
			return std::pair<std::string, int>
				{c.substr(from + 1, i - from - 1), i + 1};
	}
	return {};
}

auto JsonParser::extractIntegral(std::string const& s, int i) -> OptPair
{
	const auto r = std::to_string(std::atoi(&s[i]));
	const auto successfulExtraction = s.compare(i, r.size(), r) == 0;
	if (!successfulExtraction)
		return {};
	return std::pair<std::string, int>{r, i + r.size()};
}

auto JsonParser::valueExtractors() -> std::vector<ValueExtractor> const&
{
	static const auto result = std::vector<ValueExtractor>{
		[](std::string const& s, int i){
			return extract(s, '\"', '\"', i);
		},
		[](std::string const& s, int i){
			return extract(s, '{', '}', i);
		},
		[](std::string const& s, int i){
			return extractIntegral(s, i);
		},
		[](std::string const& s, int i) -> OptPair{
			const auto t = std::string("true");
			const auto f = std::string("false");
			if (s.compare(i, t.size(), t) == 0)
				return std::pair<std::string, int>{t, i + t.size()};
			if (s.compare(i, f.size(), f) == 0)
				return std::pair<std::string, int>{f, i + f.size()};
			return {};
		},
	};
	return result;
}

auto JsonParser::map(std::string const& c) -> std::map<std::string, std::string>
{
	auto result = std::map<std::string, std::string>();
	auto i = 0;
	auto opt_s = std::optional<std::string>();
	while (i < c.size()){
		auto opt_res = std::optional<std::pair<std::string, int>>();
		if (!opt_s){
			if ((opt_res = extract(c, '\"', '\"', i))){
				const auto [r, j] = opt_res.value();
				opt_s = r;
				i = j;
			}
		}
		else{
			for (auto ve : valueExtractors()){
				if ((opt_res = ve(c, i))){
					const auto [r, j] = opt_res.value();
					result.emplace(opt_s.value(), r);
					opt_s = std::nullopt;
					i = j;
					break;
				}
			}
		}
		if (!opt_res)
			++i;
	}
	return result;
}

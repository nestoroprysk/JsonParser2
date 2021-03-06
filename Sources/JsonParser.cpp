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

auto JsonParser::objectContent(std::string_view c) -> std::optional<std::string_view>
{
	auto const opt_res = extract(c, '{', '}');
	if (!opt_res) return {};
	return opt_res->first;
}

auto JsonParser::listContent(std::string_view c) -> std::optional<std::string_view>
{
	auto const opt_res = extract(c, '[', ']');
	if (!opt_res) return {};
	return opt_res->first;
}

auto JsonParser::extract(std::string_view c, char open, char close, int from) -> OptPair
{
	if (c.empty()) return {};
	if (from >= c.size()) return {};
	if (c[from] != open) return {};
	auto nbToClose = 1;
	for (auto i = from + 1; i < c.size(); ++i){
		auto const e = c[i];
		if (e == close)
			--nbToClose;
		else if (e == open)
			++nbToClose;
		if (nbToClose == 0)
			return std::pair<std::string_view, int>
				{c.substr(from + 1, i - from - 1), i + 1};
	}
	return {};
}

auto JsonParser::valueExtractors() -> std::vector<ValueExtractor> const&
{
	static auto const result = std::vector<ValueExtractor>{
		[](std::string_view s, int i){
			return extract(s, '[', ']', i);
		},
		[](std::string_view s, int i){
			return extract(s, '{', '}', i);
		},
		[](std::string_view s, int i){
			return extract(s, '\"', '\"', i);
		},
		[](std::string_view s, int i) -> OptPair{
			return JsonParserUtils::add(
				[=]{return eq(s, "true", i);},
				[=]{return eq(s, "false", i);}
			);
		},
		[](std::string_view s, int i) -> OptPair{
			return JsonParserUtils::add([=]{return JsonParserUtils::extractedArithmetic<long long>(s, i, OVERLOADS_OF(std::stoll));},
				[=]{return JsonParserUtils::extractedArithmetic<unsigned long long>(s, i, OVERLOADS_OF(std::stoull));});
		}
	};
	return result;
}

auto JsonParser::eq(std::string_view s, std::string_view b , int i) -> OptPair
{
	if (s.size() < i + b.size()) return {};
	if (s.compare(i, b.size(), b) == 0)
		return std::pair<std::string_view, int>{b, i + b.size()};
	return {};
}

auto JsonParser::map(std::string_view c) -> std::map<std::string_view, std::string_view>
{
	auto result = std::map<std::string_view, std::string_view>();
	auto i = 0;
	auto opt_s = std::optional<std::string_view>();
	while (i < c.size()){
		auto opt_res = std::optional<std::pair<std::string_view, int>>();
		if (!opt_s){
			if ((opt_res = extract(c, '\"', '\"', i))){
				auto const [r, j] = opt_res.value();
				opt_s = r;
				i = j;
			}
		}
		else{
			for (auto const& ve : valueExtractors()){
				if ((opt_res = ve(c, i))){
					auto const [r, j] = opt_res.value();
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

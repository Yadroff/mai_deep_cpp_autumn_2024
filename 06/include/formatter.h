#pragma once

#include <iostream>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <ranges>
#include <charconv>
#include <exception>

namespace fmt {
template<typename T>
concept Streamable = requires(std::ostream& os, const T& val) {
    { os << val } -> std::same_as<std::ostream&>;
};

template<Streamable... Args>
std::string format(std::string_view format, Args... args);

template<>
std::string format(std::string_view format){
    return {format.data(), format.size()};
}

namespace impl {

struct BracketIdx {
  std::size_t start;
  std::size_t end;

  BracketIdx(std::size_t start, std::size_t end)
          :start(start), end(end) { }
};

using indexes = std::vector<BracketIdx>; // indexes of {arg} in string (sorted)
using indexes_map = std::unordered_map<std::size_t,
                                       std::vector<indexes::size_type>>; // arg -> vector of inds in indexes

template<Streamable T>
std::string to_string(const T& val);

struct Index {
  indexes idx;
  indexes_map map;
};

Index parse(std::string_view format, std::size_t args_num);

template<Streamable T, Streamable... Args>
void process(std::string& result, std::size_t idx, Index& index, const T& arg, const Args&... args);

template<Streamable T>
void process(std::string& result, std::size_t cur, Index& index, const T& arg);

} // namespace impl
} // namespace fmt

#include "formatter.hpp"
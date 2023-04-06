#pragma once
#include "CometPCH.h"

#include <unordered_set>

namespace Comet
{

/*
This hash enables std::string_views to be used as key_type on std::unordered_map<std::string, T> methods(e.g.find())
Taken from https://en.cppreference.com/w/cpp/container/unordered_map/find
*/
struct StringHash
{
    using hash_type = std::hash<std::string_view>;
    using is_transparent = void;

    std::size_t operator()(const char* str) const { return hash_type{}(str); }
    std::size_t operator()(std::string_view str) const { return hash_type{}(str); }
    std::size_t operator()(const std::string& str) const { return hash_type{}(str); }
};

template<typename T>
using MapStr2T = std::unordered_map<std::string, T, StringHash, std::equal_to<>>;

using UnorderedStrSet = std::unordered_set<std::string, StringHash, std::equal_to<>>;

}
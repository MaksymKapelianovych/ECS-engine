#pragma once

#include <map>

template <class K, class V, class Allocator = typename std::map<K, V>::allocator_type>
using Map = std::map<K, V>;

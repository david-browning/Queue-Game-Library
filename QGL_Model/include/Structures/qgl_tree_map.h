#pragma once
#include "include/Structures/qgl_basic_tree_map.h"

namespace qgl
{
   template<class KeyT, class ValueT>
   using tree_map = basic_tree_map<std::map, KeyT, ValueT>;
}
#pragma once
#include "include/qgl_model_include.h"

namespace qgl::parse
{
   /*
    Checks if the "map" contains key "key" and the key's "type()" member equals
    the type "t".
    */
   template<class MapT, typename ValueT>
   inline bool check_type_exists(const std::string& key,
                                 const MapT& map,
                                 ValueT t)
   {
      return map.count(key) > 0 && map[key].type() == t;
   }
}
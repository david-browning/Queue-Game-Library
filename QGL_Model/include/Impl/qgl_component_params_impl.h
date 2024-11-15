#pragma once
#include "include/qgl_model_include.h"
#include <map>

namespace qgl::impl
{
   using content_param_types_impl = typename int32_t;
   enum known_param_types_impl : content_param_types_impl
   {
      known_int8 = 1,
      known_uint8,
      known_int16,
      known_uint16,
      known_int32,
      known_uint32,
      known_int64,
      known_uint64,
      known_int_last,

      known_rational8 = 16,
      known_urational8,
      known_rational16,
      known_urational16,
      known_rational32,
      known_urational32,
      known_rational64,
      known_urational64,
      known_rational_last,

      known_str8 = 24,
      known_str16,
      known_str_last,

      known_bool = 32,
      known_compound,

      known_last = 2097
   };

   static const std::map<std::string, int32_t> STR_COMPONENT_PARAM_MAP
   {
      { "int8",        known_param_types_impl::known_int8        },
      { "uint8",       known_param_types_impl::known_uint8       },
      { "int16",       known_param_types_impl::known_int16       },
      { "uint16",      known_param_types_impl::known_uint16      },
      { "int32",       known_param_types_impl::known_int32       },
      { "uint32",      known_param_types_impl::known_uint32      },
      { "int64",       known_param_types_impl::known_int64       },
      { "uint64",      known_param_types_impl::known_uint64      },
      { "int",         known_param_types_impl::known_int32       },

      { "rational8",   known_param_types_impl::known_rational8   },
      { "urational8",  known_param_types_impl::known_urational8  },
      { "rational16",  known_param_types_impl::known_rational16  },
      { "urational16", known_param_types_impl::known_urational16 },
      { "rational32",  known_param_types_impl::known_rational32  },
      { "urational32", known_param_types_impl::known_urational32 },
      { "rational64",  known_param_types_impl::known_rational64  },
      { "urational64", known_param_types_impl::known_urational64 },
      { "rational",    known_param_types_impl::known_rational32  },
      { "float",       known_param_types_impl::known_rational32  },

      { "string8",     known_param_types_impl::known_str8        },
      { "string16",    known_param_types_impl::known_str16       },
      { "string",      known_param_types_impl::known_str8        },

      { "bool",        known_param_types_impl::known_bool        },
      { "compound",    known_param_types_impl::known_compound    },
   };

   static const std::unordered_map<int32_t, std::string> COMPONENT_PARAM_STR_MAP
   {
      { known_param_types_impl::known_int8,         "int8"         },
      { known_param_types_impl::known_uint8,        "uint8"        },
      { known_param_types_impl::known_int16,        "int16"        },
      { known_param_types_impl::known_uint16,       "uint16"       },
      { known_param_types_impl::known_int32,        "int32"        },
      { known_param_types_impl::known_uint32,       "uint32"       },
      { known_param_types_impl::known_int64,        "int64"        },
      { known_param_types_impl::known_uint64,       "uint64"       },

      { known_param_types_impl::known_rational8,    "rational8"    },
      { known_param_types_impl::known_urational8,   "urational8"   },
      { known_param_types_impl::known_rational16,   "rational16"   },
      { known_param_types_impl::known_urational16,  "urational16"  },
      { known_param_types_impl::known_rational32,   "rational32"   },
      { known_param_types_impl::known_urational32,  "urational32"  },
      { known_param_types_impl::known_rational64,   "rational64"   },
      { known_param_types_impl::known_urational64,  "urational64"  },

      { known_param_types_impl::known_str8,         "string8"      },
      { known_param_types_impl::known_str16,        "string16"     },

      { known_param_types_impl::known_bool,         "bool"         },
      { known_param_types_impl::known_compound,     "compound"     },
   };
}
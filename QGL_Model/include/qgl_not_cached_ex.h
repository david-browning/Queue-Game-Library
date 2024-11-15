#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<class Key>
   class not_cached : std::exception
   {
      public:
      not_cached(const Key& k) :
         errorMessage(k + " is not cached.")
      {
      }

      const char* what() const noexcept override
      {
         return errorMessage.c_str();
      }

      private:
      std::string errorMessage;
   };
}
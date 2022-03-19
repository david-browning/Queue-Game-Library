#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_trigger.h"
#include "include/qgl_input_state.h"
#include "include/qgl_input_instance.h"
#include "include/Providers/qgl_uwp_gamepad_input_provider.h"
#include "include/Providers/qgl_xinput_input_provider.h"
#include "include/Providers/qgl_uwp_background_point_input_provider.h"
#include "include/Providers/qgl_uwp_pointer_input_provider.h"
#include "include/Providers/qgl_uwp_key_input_provider.h"

namespace qgl::input
{
   template<class InputProviderTraits>
   class basic_input_provider
   {
      public:
      basic_input_provider(InputProviderTraits&& traits) :
         m_traits(std::forward<InputProviderTraits>(traits))
      {

      }

      basic_input_provider(basic_input_provider&&) = default;

      virtual ~basic_input_provider() noexcept = default;

      /*
       Returns the inputs for the instant this is called.
       */
      input_instance instance() noexcept
      {
         return m_traits.instance();
      }

      private:
      InputProviderTraits m_traits;
   };

   using xinput_input_provider =
      typename basic_input_provider<providers::xinput_provider_traits>;

   using gamepad_input_provider =
      typename basic_input_provider<providers::gamepad_provider_traits>;

   using uwp_key_input_provider =
      typename basic_input_provider<providers::uwp_key_input_provider_traits>;

   using uwp_pointer_input_provider =
      typename basic_input_provider<providers::uwp_point_input_provider_traits>;

   using uwp_background_pointer_input_provider =
      typename basic_input_provider<
      providers::uwp_background_point_input_provider_traits>;
}
#include "pch.h"
#include <nlohmann/json.hpp>
#include "include/Components/qgl_icomponent_metadata.h"
#include "include/Parsing/qgl_parse_helpers.h"
#include "include/Parsing/qgl_parse_constants.h"
#include "src/Components/qgl_dynamic_component_metadata.h"

using param_list_type = 
   typename std::vector<qgl::components::dynamic_component_param_metadata>;

nlohmann::json parse_json(const char* str)
{
   using json = nlohmann::json;
   using namespace qgl::parse;
   using namespace qgl::parse::constants;

   nlohmann::json ret;

   ret = json::parse(str);

   // Check that all the values exist and are the correct type
   if (!check_type_exists(KEY_ID, ret, json::value_t::string) ||
       !check_type_exists(KEY_NAME, ret, json::value_t::string) ||
       !check_type_exists(KEY_DESCRIPTION, ret, json::value_t::string) ||
       !check_type_exists(KEY_PARAMETERS, ret, json::value_t::array))
   {
      throw std::domain_error{ "The array entry is missing required keys." };
   }

   return ret;
}

param_list_type parse_param_array(const nlohmann::json& j, int depth)
{
   using json = nlohmann::json;
   using namespace qgl::parse;
   using namespace qgl::parse::constants;
   using namespace qgl::components;

   param_list_type ret;
   auto a = j.get<json::array_t>();

   for (const auto& p : a)
   {
      // Make sure the array entry is an object.
      if (p.type() != json::value_t::object)
      {
         throw std::domain_error{ "The array entry is not a JSON object." };
      }

      // Make sure the object has the correct parameters.
      if (!check_type_exists(KEY_NAME, p, json::value_t::string) ||
         !check_type_exists(KEY_DESCRIPTION, p, json::value_t::string) ||
         !check_type_exists(KEY_TYPE, p, json::value_t::string))
      {
         throw std::domain_error{ "The array entry is missing required keys." };
      }

      auto paramType = str_to_param_type(p[KEY_TYPE].get<std::string>());
      auto paramName = p[KEY_NAME].get<std::string>();
      auto paramDesc = p[KEY_DESCRIPTION].get<std::string>();

      // If the object has a "parameters" key:
      if (p.contains(KEY_PARAMETERS))
      {
         // Parse the sub-params key.
         auto subParams = parse_param_array(p[KEY_PARAMETERS], depth + 1);
         ret.emplace_back(paramName,
                          paramDesc,
                          subParams.begin(),
                          subParams.end());
      }
      else
      {
         // Else its the base case.
         auto& sizeKey = p[KEY_SIZE];
         if (!sizeKey.is_number())
         {
            throw std::domain_error{ "The size key is not an integer." };
         }

         auto paramCount = p[KEY_SIZE].get<uint8_t>();
         ret.emplace_back(paramType, paramCount, paramName, paramDesc);
      }
   }

   return ret;
}

qgl::result_t QGL_CC qgl::components::make_component_from_json(
   const char* str, icomponent_metadata** out_p) noexcept
{
   if (!out_p)
   {
      return E_INVALIDARG;
   }

   using json = nlohmann::json;
   using namespace qgl::components;
   using namespace qgl::parse;
   using namespace qgl::parse::constants;

   // Parse the string
   json j;
   try
   {
      // Get the JSON from the string.
      j = parse_json(str);
   }
   catch (const json::parse_error& e)
   {
      return MAKE_HRESULT(1, qgl::FACILITY_QGL, e.id);
   }
   catch (...)
   {
      return E_UNEXPECTED;
   }

   // Parse the component parameters.
   param_list_type params;
   try
   {
      params = parse_param_array(j[KEY_PARAMETERS], 0);
   }
   catch (const std::exception& ex)
   {
      return winrt::to_hresult();
   }

   // Create the component metadata.
   qgl::guid compID{ j[KEY_ID].get<std::string>().c_str() };
   auto ret = new (std::nothrow) dynamic_component_metadata(
      j[KEY_NAME].get<std::string>(),
      j[KEY_DESCRIPTION].get<std::string>(),
      std::move(compID),
      params.begin(),
      params.end());

   // Check and return.
   if (!ret)
   {
      return E_OUTOFMEMORY;
   }

   *out_p = ret;
   return S_OK;
}

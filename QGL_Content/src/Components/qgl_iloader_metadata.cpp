#include "pch.h"
#include <nlohmann/json.hpp>
#include "include/Loaders/qgl_iloader_metadata.h"
#include "src/Loaders/qgl_dynamic_loader_metadata.h"

qgl::result_t QGL_CC make_loader_from_json(
   const char* str,
   const qgl::imodule* mod_p,
   qgl::content::iloader_metadata** out_p) noexcept
{
   if (!out_p)
   {
      return E_INVALIDARG;
   }

   using json = nlohmann::json;
   using namespace qgl;
   using namespace qgl::parse;

   // Parse the string
   json j;
   try
   {
      j = json::parse(str);
   }
   catch (const json::parse_error& e)
   {
      return MAKE_HRESULT(1, qgl::FACILITY_QGL, e.id);
   }
   catch (...)
   {
      return E_UNEXPECTED;
   }

   // Check that it has the required keys.
   if (!check_type_exists(constants::KEY_ID, j, json::value_t::string) ||
      !check_type_exists(constants::KEY_LOAD, j, json::value_t::string) ||
      !check_type_exists(constants::KEY_SAVE, j, json::value_t::string))
   {
      return E_FAIL;
   }

   qgl::guid loaderID{ j[constants::KEY_ID].get<std::string>().c_str() };
   auto ret = new (std::nothrow)dynamic_loader_metadata(
      std::move(loaderID),
      mod_p,
      j[constants::KEY_LOAD].get<std::string>(),
      j[constants::KEY_SAVE].get<std::string>());

   // Check and return.
   if (!ret)
   {
      return E_OUTOFMEMORY;
   }

   *out_p = ret;
   return S_OK;
}
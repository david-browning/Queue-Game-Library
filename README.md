## TODO:
* Implement qgl::rational operators
* Test qgl::rational
* Test qgl::content:: entry importers
* Test qgl::content:: file loaders
* Test qgl::qgl_version_t
* Test qgl::icommand
* Verify or implement iterators for icontent_file, icontent_store, and 
  icontent_project.
* icontent_project and icontent_file must support erasing. 
* Implement qgl_load_sampler_file
* Implement text_format_file_loader
* Implement qgl_load_rasterizer_file
* Implement depth_stencil_file_loader
* Implement light_file_loader
* Implement depth_stencil_file_loader
* Wrap all file loaders as functors?
* content_store_1_0::flush_loads_thread 
* content_store_1_0::load leaks exceptions when calling loaderFn(). (Line 123)
  Either require all file loaders to be noexcept, or catch exceptions and
  translate to HRESULT.
* Organize qgl::content::RESOURCE_TYPES
* Organize qgl::content::CONTENT_LOADER_IDS
* Change qgl::content::check_loader_id to noexcept to avoid file loaders 
  leaking exceptions? Return bool or HRESULT?
* Change qgl::content::check_resource_type to noexcept to avoid file loaders 
  leaking exceptions? Return bool or HRESULT?
* Change qgl::content::check_loader_and_resource to noexcept to avoid file 
  loaders leaking exceptions? Return bool or HRESULT?
* Before returning from a function with a failed HRESULT, print a message to
  the debug console only in DEBUG mode.
* OBJ Importer.
* Move math stuff out of annotations.h
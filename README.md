## Build Requirements:
* Fully up-to-date Visual Studio 2017. Tested only on Enterprise.
* Required Workloads:
    * Universal Windows Platform Development
    * Game development with C++
* Additional Required Components:
    * Windows Universal C Runtime
    * Windows 10 SDK (10.17763.0)
    * Live Unit Testing
    * Coded UI test
    * C++ profiling tools
    * .NET profiling tools
    * Testing tools core features
    * C++ Universal Windows Platform tools for ARM64
    * Modules for Standard Library
    * VC++ 2017 latest version tools
    * Libraries for Spectre (ARM)
    * Libraries for Spectre (ARM64)
    * Libraries for Spectre (x86 and x64)
    * Visual C++ compilers and libraries for ARM
    * Visual C++ compilers and libraries for ARM64
    * Visual C++ runtime for UWP
    * Windows Universal CRT SDK
    * Git for Windows
* Required Plugins:
    * C++/WinRT  
    * GitHub Extension for Visual Studio  

## TODO:
* Implement qgl::rational operators
* Test qgl::rational
* Test qgl::content:: entry importers
* Test qgl::content:: file loaders
* Test qgl::qgl_version_t
* Test qgl::icommand
* Test math functions, comparators and template variables.
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
* qgl::content::buffers::BRUSH_BUFFER has floating point data types in it.
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
* Define vertex layout in content. Needs entry importer and file loader.
* OBJ Importer. Needs to be aware of the vertex layout.
* Define a component-like class for use in rendering, handling input, and 
  physics updating. Components need to be type safe and allow programmers to 
  define new functionality. Components are functors with additional 
  requirements?
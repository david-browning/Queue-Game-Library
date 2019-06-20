Queue Game Library
==================

## Overview
QGL is a DirectX 12 game library. The primary philosophy of this project is to 
achieve high performance with parallelism. Where applicable, code should focus 
on vector processing and queuing operations to run in parallel.

Ultimately, the code base balances being fast, friendly, flexible, and 
familiar.
* Achieve fast code by using optimized algorithms and taking advantage of 
  modern hardware. DirectX math uses SSE registers. The C++ STL provides many 
  optimized data structures.
* Achieve familiar code by mimicking C++'s style and using STL data types. The 
  code should look and act like modern C++ code. At DLL boundaries, the code 
  should look and act like COM.
* Achieve flexible code by utilizing templates where appropriate and allowing 
  classes to be extended. 
* Achieve friendly code by providing robust error handling, catching errors at 
  compile time, and using C++ idioms. Using static_assert is a good way to 
  ensure template instantiation is correct. RAII removes cleanup from the 
  caller which is especially useful when there is an error.

## Important Topics:
### Best Practices:
* https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization
* https://en.cppreference.com/w/cpp/language/raii
* https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)
* https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
* https://stackoverflow.com/questions/4421706/what-are-the-basic-rules-and-idioms-for-operator-overloading
* https://stackoverflow.com/questions/6500313/why-should-c-programmers-minimize-use-of-new
* https://stackoverflow.com/questions/1452721/why-is-using-namespace-std-considered-bad-practice
* https://isocpp.org/wiki/faq/const-correctness
* https://stackoverflow.com/questions/16699247/what-is-a-cache-friendly-code?noredirect=1&lq=1

### Technologies Used in QGL:
* https://docs.microsoft.com/en-us/windows/desktop/com/error-handling-in-com
* https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt
* https://docs.microsoft.com/en-us/windows/desktop/direct3d12/direct3d-12-graphics

### Additional C++ Info:
* https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used?rq=1
* https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
* https://stackoverflow.com/questions/1041866/what-is-the-effect-of-extern-c-in-c
* https://stackoverflow.com/questions/106508/what-is-a-smart-pointer-and-when-should-i-use-one?rq=1

### DLL Programming:
* https://devblogs.microsoft.com/cppblog/exception-boundaries/
* https://chadaustin.me/cppinterface.html

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
* OBJ Importer. Needs to be aware of the vertex layout.
* Define a component-like class for use in rendering, handling input, and 
  physics updating. Components need to be type safe and allow programmers to 
  define new functionality. Components are functors with additional 
  requirements?
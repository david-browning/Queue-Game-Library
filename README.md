Queue Game Library
==================
## Overview:
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

## Target Platform:
QGL is developed for x64 Windows 10 platforms. The minimum supported version of
Windows 10 is 1803. There are no plans to support 32-bit. In the future, QGL 
may support ARM64.

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
* Fully up-to-date Visual Studio 2017. Tested on Enterprise and Community.
* Required Workloads:
    * Universal Windows Platform Development
    * Game development with C++
* Additional Required Components:
    * Git for Windows
    * GitHub extension for Visual Studio
    * NuGet package manager
    * C++ profiling tools
    * .NET profiling tools
    * Testing tools core features
    * VC++ 2017 latest version tools
    * Libraries for Spectre (x86 and x64)
    * Visual C++ runtime for UWP
    * Windows Universal CRT SDK
    * Windows Universal C Runtime
    * Windows 10 SDK (10.17734.0)
* Required Plugins:
    * C++/WinRT  
    * GitHub Extension for Visual Studio  
* Recommended Plugins:
    * Productivity PowerTools 2017/2019
    * Custom Web Search
    * Editor Guidelines

## Testing:
* Change the solution platform to x64. By default, it is x86.
* Change test architecture to X64: Select "Test" -> "Test Settings" -> 
  "Default Processor Architecture".

## TODO:
* Fix failed HRESULT when opening existing files.
* Support multi-instance with content builder.
* Test qgl::content:: entry importers.
* Test qgl::content:: file loaders.
* Test qgl::qgl_version_t.
* Test qgl::icommand.
* Test qgl::math::rectangle.
* Test icontent_file::erase.
* Test content_project_entry.
* Test math functions, comparators and template variables.
* Test static pointer.
* Test component.
* Test tree_map and unordered_tree_map
* Test xform_tree
* Verify converting timer's elapsed ticks to seconds is always accurate to 8  
  decimal places. 
  Test it after these time spans:  
  * 0 seconds
  * 3 microseconds
  * 10 microseconds
  * 25 microseconds
  * 100 microseconds
  * 500 microseconds
  * 1 millisecond
  * 3 milliseconds
  * 25 milliseconds
  * 100 milliseconds
  * 500 milliseconds
  * 1 second
  * 3 seconds
  * 1 day
  * 1 month
  * 140 years
  * 200 years
* Cross compiler/VC Runtime integration tests.
* qgl::content::buffers::BRUSH_BUFFER has floating point data types in it.
* OBJ Importer. Needs to be aware of the vertex layout.
* PSO supports steam output?
* Resize frame? This resized render targets and depth stencils? Resize viewport 
  and scissor?
* Provide CPU access to depth-stencil buffer.
* Require AVX support?
* Memory map the file that icontent_file points to?

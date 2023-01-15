Error Handling
==============

## Overview:
While the preferred error handling in C++ is exceptions, they are a poor choice 
for DLLs. Exceptions do not translate across the ABI. DLL exported functions 
and classes are not allowed to throw exceptions. Dynamic memory allocations in 
a DLL must use std::nothrow to keep std::bad_alloc exceptions from leaking 
across the ABI. One exception to this rule are constructors. Constructors that 
perform dynamic memory allocations do not need to use std::nothrow.  

Constructors can leak exceptions because they are not allocated in the DLL's 
address space and to enforce RAII.  

Instead of throwing exceptions, DLL exported functions that may encounter an 
error return an HRESULT. To indicate success, return S_OK.  

Before throwing an exception or returning a failed HRESULT use 
OutputDebugString() to give more information about the error. OutputDebugString 
must be guarded with #ifdef DEBUG so it is not compiled in release code.

## Exception Use:
Use exceptions when the code cannot continue do to an error. Throw STL 
exceptions or exceptions that inherit std::exception.  

Exceptions should use a strong exception guarantee.

### Exception Handling:
* Always catch exceptions by reference.
* Exceptions that allocate dynamic memory must clean up in their destructor.

## HRESULT Use:
Use HRESULTs when errors must pass between the ABI. Mark functions that return 
an HRESULT should be marked "noexcept".

### HRESULT Handling
* Always inspect a function's returned HRESULT. It can quickly be checked by 
  using FAILED(). If the HRESULT failed, the function should return it or
  throw an exception if permitted.
* If a function is allowed to throw an exception, it can check if an HRESULT 
  failed by using check_result().

## Cleanup After Error:
Whether throwing an exception or returning an HRESULT, functions must not leak
resources. These guidelines help avoid memory leaks because their destructors 
delete the resources the wrap.
* Prefer STL containers over dynamically allocated arrays.
* If dynamic allocation is required, use a smart pointer.
* Wrap COM interfaces with a pptr.
* Wrap file handles, such as a HANDLE, with a file_handle. The file is  
  automatically closed when the file_handle is destructed.
* Wrap iqgl interfaces in a unique pointer using qgl::make_unique(). STL smart
  pointer functions will not free iqgl interfaces correctly. 

## More Info:
https://en.cppreference.com/w/cpp/language/exceptions
https://docs.microsoft.com/en-us/windows/desktop/com/error-handling-in-com
https://docs.microsoft.com/en-us/cpp/cpp/errors-and-exception-handling-modern-cpp?view=vs-2019
https://docs.microsoft.com/en-us/cpp/cpp/how-to-interface-between-exceptional-and-non-exceptional-code?view=vs-2019
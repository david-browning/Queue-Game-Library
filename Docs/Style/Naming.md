## Global Rules
* Any pointer variable, whether its a raw pointer or smart pointer, must be 
  postfixed with "_p". Ex: fileHandle_p.

## File Names
* If a file contains a class, the file be named after the class.
* Header and source files developed by QGL engineers must be prefixed with
  "qgl_". Files that are not developed in house, such as "d3dx12.h", do not
  follow this convention.
* Header and source files that contain an activatable Windows Runtime component
  must be postfixed with "_rt". Ex: "qgl_window_rt.h"

## Classes
* Classes use C++ style class names. Class names describe an object in all lower
  case with words separated by underscores.
* Avoid verbs in class names.
* Minimize the number of adjectives in a class name.
* Class that are an activatable Windows Runtime component must be postfixed 
  with "_rt". Ex: "file_metadata_rt".

## Structs
* Structs use all capital letters, with words separated by underscores.

## Interfaces
* Interfaces are classes that provide at least 1 pure abstract function.
* Interface names must begin with "i" and follow class naming convention.
* Do not put an underscore between the "i" and class name. 

Ex: "icontent_store".

## Constants
* Constants must be marked "static" and "const" or "constexpr". 
* The constant must be all capitals with words separated with underscores. 

Ex: "MAX_NAME_CHARS". Constants should be descriptive and accompanied by
documentation describing their purpose.

## Global Variables
* There is no style for global variables because they are not allowed. Global 
  variables are dangerous, especially for dynamic linked libraries.
* If a global variable is absolutely necessary, it must not follow any other
  naming convention. Suggest pre and post fixing a global variable with two
  underscores.

## Macros
With few exceptions, Macros are not allowed. Macros are allowed in code 
guarding, such as DEBUG, or to abstract platform specific keywords, such as
QGL_MODEL_API.  

When using a Macro, it must be all capitals with words separated by underscores.

## Public Member Variables
Public variables are used in structs. Public member variables are not the same
as public accessors functions. Public variables use PascalCase. The purpose of
a public member variable should be obvious, but additional documentation is 
required to describe the purpose of the variable.

## Private Member Variables
*  Private members are written using lower camelCase. Private members are 
  prefixed with "m_". Ex: "m_numElements". The names should be descriptive. 
* Private member variables should not require additional documentation to 
  describe their use.

## Public Member Functions
* Public member functions use the same naming convention as classes.
* While verbs are allowed in public member functions, they should be avoided.
* Minimize the number of adjectives in a public member function.
* When developing a container-like or string-like class, use the same member 
  function that C++ strings and containers use. Use "size()" to return the 
  number of elements a container-like class has. Use "data()" if the 
  container-like class provides access to an underlying buffer.

## Private Member Functions
* Private member functions use the same naming conventions as public naming 
  conventions. 
* Private member functions may optionally be prefixed with "p_". 
  Ex: "p_construct()".

## Function Parameters

## Template Parameters

## Non-Member Functions
* Use the same naming convention as public member functions.
* Non-member functions must be prefixed with qgl_.
* Non-member functions should include a verb, describing what the function does.

Ex: "qgl_make_content_store()".
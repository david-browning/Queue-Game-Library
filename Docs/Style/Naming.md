## Global Rules
* Lines cannot be more than 80 characters. 

## Classes
Classes use C++ style class names. Class names describe an object in all lower
case with words separated by underscores.

## Structs

## Constants
Constants must be marked "static" and "const" or "constexpr". The constant 
must be all capitals with words separated with underscores. 
Ex: "MAX_NAME_CHARS". Constants should be descriptive and accompanied by
documentation describing their purpose.

## Global Variables
There is no style for global variables because they are not allowed. Global 
variables are dangerous, especially for dynamic linked libraries.

## Macros
With few exceptions, Macros are not allowed. Macros are allowed in code 
guarding, such as DEBUG, or to abstract platform specific keywords, such as
QGL_MODEL_API.  

When using a Macro, it must be all capitals with words separated by 
underscores.

## Private Member Variables
Private members are written using lower camelCase. Private members are 
prefixed with "m_". Ex: "m_numElements". The names should be descriptive. 
Private member variables should not require additional documentation to 
describe their use.

## Public Member Variables
Public variables are used in structs. Public member variables are not the same
as public accessors functions. Public variables use PascalCase. The purpose of
a public member variable should be obvious, but additional documentation is 
required to describe the purpose of the variable.

## Private Member Functions

## Public Member Functions

## Function Parameters

## Template Parameters

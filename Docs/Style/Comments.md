Commenting Code
===============

## Comment Styles:
Use inline comments with two forward slashes or block comments with /* */. 
Comments are required before any function, type, or constant declaration.

## Inline Comments:
Inline comments use two forward slashes. These comments are only allowed inside
function definitions, operator definitions, and initialization expressions.  

Source code should be inherently descriptive. Inline comments should focus on 
why a particular line of code does something, not what it does it.

## Block Comments:
Block comments begin with a /* and the comment starts on the next line. 
Comments do not start on the same line as the /*. Align the first character of 
each comment with the asterisk. Comments are terminated with a */. The */ 
should not be on the same line as a comment. See examples below.

Use block comments to annotate types, constants, member functions, member 
variables, member types, non-member functions, and operators.  

For functions, block comments should describe the parameters, return type, any 
exceptions the function may throw, and the purpose of the function.
Operators follow the same rules as function comments, but do not need to 
describe their parameters.

Operators follow the same comment rules as functions, except they do not need 
to describe their parameters or return type.

For member variables and constants, the comment should describe the variable's 
purpose.  

Types should describe the purpose of the type and what functionality it 
abstracts.

If a function or type has template parameters, those should be documented only 
if the parameters have restrictions. Ex: If a template parameter is a functor, 
the block comment must note requirements for a valid parameter.  

Definitions do not need to be commented if the declaration is commented.

Ex:

/*  
 Compiles a content project into a content file and saves it at the location  
 pointed to by handle_p.  
 Returns:  
    E_ACCESS if access is denied to the file.  
    E_UNEXPECTED if the project contains invalid data.  
    S_OK on success.  
 */  
HRESULT compile_file(const icontent_project* proj_p,  
                     const file_handle* handle_p) noexcept;  

/*  
 Number of characters in the wide string, not including null.  
 */  
size_t m_numChars;  

/*  
 Abstracts a QGL content file located on disk. Provides functions for reading   
 and writing data to the file.  
 */  
class icontent_file : public iqgl;  
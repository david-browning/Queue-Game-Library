## Indentation
Each level is indented with 3 spaces. Tabs are not allowed.

### Special Indentation Rules
* Case labels are indented.
* Case contents are indented.

## Operators
* A single space is required between binary operators. Ex: "i = x + 2;"
* Spaces are not allowed between binary operands and their operator.
  Ex: "i++;"

## Wrapping
* Lines cannot be more than 80 characters. If a statement or declaration is 
  more than 80 characters, each parameter in a functions call should be on a 
  separate line.
  Ex: 
  winrt::check_hresult(  
    qgl_create_graphics_device(  
    parameter1, 
    parameter2,  
    parameter3));
    
  Ex:  
  template<
    typename T,
    typename U,
    typename V,
    typename W>
  class xyz;  
* Another good place to separate by line is at the assignment (=), stream 
  (<<, >>), logical conjunction (&&), logical disjunction (||), and binary 
  arithmetic operators.
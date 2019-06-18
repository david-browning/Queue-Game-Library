Array Importers
===============

## Description:
Array data is prefixed with the size of the elements in the array. The first 2
bytes encode the size of an array element. The next 14 bytes are reserved. 
Following is the array of elements. The number of elements in the array can be
calculated using the dictionary entry size. Subtract 16 from the size and 
divide that by the number of bytes in an array element.  

Recommend keeping array elements 2, 4, 8, or 16 byte aligned.

## Binary Layout:
Bytes 0-1: Size, in bytes, of an array element.
Bytes 2-15: Reserved
Bytes 16+: Elements in the array.

## Importer:
Specialize qgl::content::loaders::array_importer with the element type.
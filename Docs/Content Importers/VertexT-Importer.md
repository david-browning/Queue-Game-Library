Vertex Element Importer
=======================

## Description:
Describes a part of a vertex description that gets submitted to the input 
assembler. An element stores a single vertex semantic, it's size, and the 
additional information required by the input assembler.

## Binary Layout:
Bytes 00-31: Semantic Name. Single byte characters. May or may not be 
null-terminated.  
Bytes 32-35: Semantic Index  
Bytes 36-37: DXGI Format  
Byte  38   : Input Slot  
Byte  39   : Input Slot Class  
Byte  40   : Element Index. This is the index of where to put this element in a 
Vertex Description.  
Byte  41   : Reserved 1  
Bytes 42-43: Reserved 2  
Bytes 44-47: Reserved 3  
Bytes 48-51: Reserved 4  
Bytes 52-63: Padding  

## Importer:
qgl::content::loaders::vert_elem_importer
Vertex Description Importer
===========================

## Description:
Describes a vertex that gets submitted to the input assembler. It is a 
collection of vertex elements.  

## Binary Layout:
Bytes 00   : Number of vertex elements.  
Byte  01   : Reserved 1  
Bytes 02-03: Reserved 2  
Bytes 04-07: Reserved 3  
Bytes 8+   : Array of vertex elements.  

## Importer: 
qgl::content::loaders::vert_desc_importer  
The loader will verify that each vertex element maps to a spot in the vertex 
description. If there are any duplicates, or holes, the loader will fail. This 
means its the responsibility of the programmer, or exporter to ensure the 
vertex elements are correct.
Shader Importer
===============

## Description:
A shader applies custom code to a graphics pipeline stage. The following stages 
can be customized using a shader:
* Vertex Shader
* Pixel Shader
* Geometry Shader
* Domain Shader
* Hull Shader

A pipeline state must have a vertex and pixel shader.

## Binary Layout:
Bytes 00   : Type of shader  
Byte  01   : Shader vendor  
Byte  02   : Shader version Major 
Byte  03   : Shader version Minor  
Bytes 04-07: Reserved 1  
Bytes 8+   : Shader Data  
The content loader ID determines if the shader data is source code or compiled.  

## Importer:
qgl::content::loaders::shader_importer.
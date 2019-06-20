Shader File
===========
## Description:
A shader applies custom code to a graphics pipeline stage. The following stages 
can be customized using a shader:
* Vertex Shader
* Pixel Shader
* Geometry Shader
* Domain Shader
* Hull Shader

A pipeline state must have a vertex and pixel shader.

## Header Metadata Requirements:
Resourece Type: RESOURCE_TYPE_SHADER  
Content Loader ID:  CONTENT_LOADER_ID_SHADER_SOURCE or 
CONTENT_LOADER_ID_SHADER_COMPILED  
Renderable: No  

## Dictionary Requirements:
Max dictionary entries: 1  
Supports shared entries: No  

### Entry Requirements:
* Resource Type: RESOURCE_TYPE_SHADER  
  Content Loader ID: CONTENT_LOADER_ID_SHADER_SOURCE or 
  CONTENT_LOADER_ID_SHADER_COMPILED  
  Uses shader_importer.  
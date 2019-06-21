Pipeline State File
===================
## Description: 
Configures the graphics pipeline. 

## Header Metadata Requirements:
Resource Type: RESOURCE_TYPE_
Content Loader ID: CONTENT_LOADER_ID_
Renderable: No  

## Dictionary Requirements:
Min dictionary entries:
Max dictionary entries:
Supports shared entries: Yes

### Entry Requirements:
* Vertex description  
  Resource Type: RESOURCE_TYPE_DESCRIPTION  
  Content Loader ID: CONTENT_LOADER_ID_VERTEX_DESCRIPTION  
  Uses vert_desc_importer  

* Sampler

* Rasterizer description  
  Resource Type: RESOURCE_TYPE_RASTERIZER  
  Content Loader ID: CONTENT_LOADER_ID_RASTERIZER  
  Uses rasterizer_importer.  

* Blender

* Vertex shader  
  Resource Type: RESOURCE_TYPE_SHADER  
  Content Loader ID: CONTENT_LOADER_ID_SHADER_SOURCE or 
  CONTENT_LOADER_ID_SHADER_COMPILED  
  Uses shader_importer.  

* Pixel shader  
  Resource Type: RESOURCE_TYPE_SHADER  
  Content Loader ID: CONTENT_LOADER_ID_SHADER_SOURCE or 
  CONTENT_LOADER_ID_SHADER_COMPILED  
  Uses shader_importer.  

* Optional domain shader  
  Resource Type: RESOURCE_TYPE_SHADER  
  Content Loader ID: CONTENT_LOADER_ID_SHADER_SOURCE or 
  CONTENT_LOADER_ID_SHADER_COMPILED  
  Uses shader_importer.  

* Optional hull shader  
  Resource Type: RESOURCE_TYPE_SHADER  
  Content Loader ID: CONTENT_LOADER_ID_SHADER_SOURCE or 
  CONTENT_LOADER_ID_SHADER_COMPILED  
  Uses shader_importer.  

* Optional geometry shader  
  Resource Type: RESOURCE_TYPE_SHADER  
  Content Loader ID: CONTENT_LOADER_ID_SHADER_SOURCE or 
  CONTENT_LOADER_ID_SHADER_COMPILED  
  Uses shader_importer.  
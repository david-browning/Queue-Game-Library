Content Files
=============
## Overview:
Content files are a binary representation of a content item. Content items 
are objects that exist in the QGL engine. Content can be textures, strings, 
shapes, sprites, geometry, and more. Content files can also define the means 
for content to render, update, and respond to input. Content files store 
additional metadata about the content such as a name and the type of content.

Content is loaded using an icontent_store. Content files can be built using an 
icontent_project and compiling the project using compile_content_project().

## Layout:
QGL content files begin with metadata about the content represented in the file. 
This includes the content name, what type of content it is, as well as other 
parameters. The first 4 bytes this metadata is a QGL version indicating which 
version the file was compiled for. If the size of the metadata changes in the 
future, content loaders will need to inspect the version to determine the 
metadata's layout.  

Following the metadata is a dictionary metadata buffer. The dictionary metadata 
buffer always comes after the file's main metadata. It contains the number of 
dictionary entries, the size of an entry, and flags.

Immediately after the dictionary metadata is a list of dictionary entries. The 
size of an entry is stored in the dictionary metadata. Each dictionary entry 
stores metadata about some data, the offset (in bytes) to the data, as well as 
the size (in bytes) of the data. The offset is relative to the beginning of 
the file. The offset is not relative to the dictionary entry or metadata.

After the last dictionary entry is all the content data. Content data is 
interpreted by a content loader. Content loaders are located at runtime using 
the content data's metadata, stored in the dictionary entry.

## Designing a Content File:
Content files must document what type of resource they are, what content loader 
they use, if the content is renderable, supports shared entries, is physics 
enabled, and the required dictionary entries. For example, WText is a content 
representing a string that can be rendered. Here are the requirements of a 
WText content file:

### Header Metadata Requirements:
Resource Type: RESOURCE_TYPE_STRING  
Content Loader ID: CONTENT_LOADER_ID_WTEXT  
Renderable: Yes  
Physics enabled: No

### Dictionary Requirement:
Max dictionary entries: 3
Supports shared entries: Yes

#### Entry Requirements:
* The string to be rendered.  
  Resource Type: RESOURCE_TYPE_STRING  
  Content Loader ID: CONTENT_LOADER_ID_WSTRING  
  Uses wstring_importer.  
* Coordinates, relative to the back buffer size, of where to draw the string.  
  Resource Type: RESOURCE_TYPE_SHAPE  
  Content Loader ID: CONTENT_LOADER_ID_SCREEN_RECTANGLE  
  Uses screen_rectangle_importer.  
* GUID of a component used to render the brush.  
  Resource Type: RESOURCE_TYPE_GUID    
  Content Loader ID: CONTENT_LOADER_ID_RENDER_COMPONENT  

## Implementing:
To create a new content item, a few classes will need to be extended:
* Derive a qgl::content::content_item to represent the new content item.
* Provide a function or functor that returns the new content item. The function 
  signature must match
  std::unique_ptr<content_item>(const icontent_file*, const id_t)
WText File
==========
## Description:
This content file describes renderable text. It does not define how to render
the text. It only defines the text and where to draw the text.

## Header Metadata Requirements:
Resource Type: RESOURCE_TYPE_STRING  
Content Loader ID: CONTENT_LOADER_ID_WTEXT

## Dictionary Requirement:
Max dictionary entries: 2
Supports shared entries: Yes

### Entry Requirements:
* Resource Type: RESOURCE_TYPE_STRING
  Content Loader ID: CONTENT_LOADER_ID_WSTRING
  Uses wstring_importer.

* Resource Type: RESOURCE_TYPE_SHAPE
  Content Loader ID: CONTENT_LOADER_ID_SCREEN_RECTANGLE
  Uses screen_rectangle_importer.
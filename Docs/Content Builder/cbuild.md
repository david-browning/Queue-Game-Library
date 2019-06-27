cbuild
======
## Overview:
This tool is used for creating QGL content files that can be used in the engine.
Use cbuild to specify a content item's metadata and add dictionary items to it. 
The project can be verified and compiled using this tool.

## Commands:
* help  
  Displays help.  
* select (header | entry NUMBER)  
  Sets the currently selected object.
  header: Sets the currently selected object to the content project's header.  
  entry NUMBER: Sets the currently selected object to the NUMBER'th dictionary 
  entry.  
* detail (header | entry | dict | NOTHING)   
  header: Displays the name, version, resource type, loader ID, GUID, if the 
  content is renderable, and if the content obeys physics.  
  entry: Displays the entry's name, resource type, loader ID, GUID, and data 
  file path.    
  dict: Displays the dictionary offset and number of entries.  
  NOTHING: Shows the details for the currently selected object.  
* list (dict)  
  dict: Shows each dictionary entry's index, name, resource type, loader ID, 
  GUID, and data file path.  
* remove (entry | dict)  
  dict: Clears the dictionary of all entries.  
  entry: Removes the selected entry from the dictionary.  
* new (entry)  
  entry: Prompts for the new entry's name, resource type, loader ID, and file 
  path to the entry's raw data.  
* set (name | resource | loader | renderable | physics | version | data) Data
  name: Data is a wide character string.  
  resource: A RESOURCE_TYPE. Data can be a hexadecimal string, but in the 
  future, this should support more friendly identifiers.  
  loader: A CONTENT_LOADER_ID. Data can be a hexadecimal string, but in the 
  future, this should support more friendly identifiers.  
  renderable: The object is renderable and must include a dictionary entry 
  who's resource type is RESOURCE_TYPE_GUID and loader ID is 
  CONTENT_LOADER_ID_RENDER_COMPONENT. Only applicable if the selected object is 
  a dictionary entry. Data can be yes, no, y, or n.  
  physics: The object obeys physics while in the engine. The dictionary must 
  include an entry who's resource type is RESOURCE_TYPE_GUID and loader ID is 
  CONTENT_LOADER_ID_PHYSICS_COMPONENT. Data can be yes, no, y, or n. Only 
  applicable if the selected object is a dictionary entry.  
  version: Reserved for future use.  
  data: File path to an entry's data. While Data can be a relative path, this 
  tool needs to save the file's absolute path. Only applicable if the selected 
  object is header.  
* compile [filePath]  
  Compiles the project and saves it.  
* verify  
  Verifies the content project. Displays success or an error.   
* save  
  Saves the project file.  
* exit  
  Prompts the user to save changes and exits the program.  

## Command Line Arguments:
* projectFile
  Required. File name or relative or absolute path of the content project file. 
  If the file does not exist, this tool creates a new file.
* -k scriptFile  
  Optional. File name or relative or absolute path of a script to automate this 
  tool.
* -o compiledOutputFile  
  Optional. File name or relative or absolute path of where to store the 
  compiled content file. If this parameter is not specified, the program will 
  save the compiled file in the working directory with the same name as the 
  metadata's name.
* -h
  Shows help for the program. This flag cannot be combined with any 
  other flag.
* --noprompt  
  Optional. Does not prompt the user to save the content project file when 
  exiting.
* --verify projectFile  
  Verifies the given project file. Program returns 0 if the project is valid. 
  This flag cannot be combined with any other flag.

## Scripting:

## Samples:
### Create and Compile a WText Content File:
cbuild .\main_menu_header-wtext.proj --noprompt  
select header  
set name "MainMenuHeaderWText"  
set resource String  
set renderable y
set physics n
set loader WText  
new entry "Header String" String WString .\headerString.dat  
new entry  
"Header Spawn"  
Shape  
Rectangle  
.\headerSpawnRect.dat  
new entry "Header Render Component" Guid RenderComponent .\wtextRenderGUID.dat  
verify  
compile .\MainMenuHeader.qgl  
save   
exit  
n  

### Verify an Existing Project File:
cbuild --verify .\backgroundSprite.qgl  

### Verify and Compile an Existing Project File:
cbuild .\pistol.proj -o pistol.qgl  
verify  
compile  
exit  
y  

### Create and Compile a Content File With a Script:
cbuild .\cameraConfig.proj --noprompt -o cameraConfig.qgl -k 
 .\create_camera_script.txt  

### Change a Sprite Project so it Obeys Physics:
cbuild .\rock-sprite.proj  
list dict  
select entry last  
set physics y  
exit  
y  
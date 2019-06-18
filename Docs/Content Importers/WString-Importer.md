WString Importer
================

## Description: 
A string is a list of wide characters, not including the null terminator.

## Binary Layout: 
Array of wide characters, not including a null terminator. The number
of characters in the string is the size of the entry divided by the size of a
wide character. Use the dictionary entry to lookup the size of the string.

## Importer:
qgl::content::loaders::wstring_importer
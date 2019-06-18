String Importer
===============

## Description: 
A string is a list of characters, not including the null terminator.

## Binary Layout: 
Array of single byte characters, not including a null terminator. The number
of characters in the string is the size of the entry. Use the dictionary entry 
to lookup the size of the string.

## Importer:
qgl::content::loaders::string_importer
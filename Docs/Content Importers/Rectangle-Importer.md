Rectangle Importer
==================
## Description:
Defines a 2D rectangle in 3D space. The normal for a rectangle can be computed 
by dividing the rectangle into two triangle and finding the normal for one 
triangle. 

## Binary Layout:
Bytes 00-01: Flags  
Bytes 02-03: Reserved1  
Bytes 04-07: Reserved2  
Bytes 08-15: Rational number stores the top left corner's x Position.  
Bytes 16-23: Rational number stores the top left corner's y Position.  
Bytes 24-31: Rational number stores the top left corner's z Position.  
Bytes 32-39: Rational number stores the bottom right corner's x position.   
Bytes 40-47: Rational number stores the bottom right corner's y position.   
Bytes 48-55: Rational number stores the bottom right corner's z position.   

## Importer:
qgl::content::loaders::rectangle_importer
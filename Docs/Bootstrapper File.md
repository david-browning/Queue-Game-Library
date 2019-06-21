QGL Bootstrapper File
=====================
Bootstrapper file must be named "boot.qgl".

## Binary layout:
04 Bytes: QGL Version
12 Bytes: Padding
16 Bytes: GUID magic number. Must be "00DA5445-8F97-4DA7-B6A4-3B0DA4F90874"
    in binary form.
16 Bytes: Engine GUID. Currently unused.
16 Bytes: Reserved
02 Bytes: Number of characters for the preferences directory.
14 Bytes: Reserved.
XX Bytes: Preferences directory. This can be an absolute path or a relative 
    path beginning with a valid relative specifier. The path may or may not 
    include a null terminator.
02 Bytes: Number of characters for the content directory.
14 Bytes: Reserved.
YY Bytes: Content store directory. This can be an absolute path or a relative 
    path beginning with a valid relative specifier. The path may or may not 
    include a null terminator.


## Relative Specifiers:
When evaluated, the relative specifiers create an absolute path for the 
computer. The paths end with a trailing back slash on Windows. On other 
systems, the evaluated path ends with a forward slash.
%install%   : Returns the directory where the application is installed.
              The program may not have write access to this directory.
%appdata%   : Returns a directory where the application has read and write
              permissions. This folder is usually hidden from the user.
%roaming%   : Returns a roaming folder. On Windows, the application can store
              file here so they sync across devices.
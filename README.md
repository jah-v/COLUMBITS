# COLUMBITS

## A lightweight, progressive, header-only software-rendering library in C.

The idea behind this library is to create somewhat of a graphics framework that simplifies software-rendering. My distaste for APIs like OpenGL has drove me to build a simpler alternative. 

As a newbie programmer, I was confused about linker and compilation errors in my duct-tape graphics framework. 

The resulting library is a series of header-files that can simply be dropped into a project source directory, combined with a much less verbose function vocabulary. Simple to debug, compile, link, and build.

Each file has a distinct function to accomodate a graphical application's needs.

* COLUMBITS.h: The "Core" file; contains functions for the creation and modification of bitmaps and drawing primitives. Intermediary between device-specific and high-level functions. Only dependencies are in the C standard library.
  
* COLUMBITS_WINDOW.h: The "Window" file; contains functions for the creation and modification of device-specific windows. Deals with mouse input, keyboard input, window size, window name, etc. Depends on COLUMBITS.h.

* COLUMBITS_2D.h: The "2D Geometry" file; contains functions to create and rasterize complex geometries. Depends on COLUMBITS.h.
  
* COLUMBITS_3D.h: The "3D Geometry" file; contains functions to create and rasterize complex 3-dimensional geometries. Depends on COLUMBITS_2D.h, which depends on COLUMBITS.h.

* COLUMBITS_GUI.h: The "GUI" file; contains functions for creating rudimentary Graphical User Interfaces that connect with events. Depends on COLUMBITS.h, and COLUMBITS_WINDOw.h unless using a custom window implementation.
  
* COLUMBITS_APP.h: The "Front-end" file; contains functions that greatly abstract GUI functions, in order to build apps with multiple pages easily. Depends on COLUMBITS_GUI.h, which depends on COLUMBITS.h.





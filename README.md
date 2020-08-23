Raster is another attempt to write a complete graphic functions stack from
scratch. This includes certain image formats loaders/savers, basic drawing
functionality, filters and scalers. The idea is to learn all the little nice
details by actually doing it. So this is basically a big playground for all
kind of graphics related issues.

This code is GPLv3 and is made for a C++20 compiler (gcc 10 prefered) and
links to libstdc++ and the X11/Xorg server. But beyond that there are no
other dependencies, except for cmake which helps to deal with gnu make to
compile all this.

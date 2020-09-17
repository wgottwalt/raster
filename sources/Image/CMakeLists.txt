ADD_LIBRARY             (Image Base.cxx Farbfeld.cxx PPM.cxx Simple00.cxx Simple01.cxx Simple02.cxx
                               Targa.cxx)
TARGET_LINK_LIBRARIES   (Image Color Common Compression X11)

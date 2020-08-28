ADD_EXECUTABLE          (Viewer Viewer.cxx)
TARGET_LINK_LIBRARIES   (Viewer Color Common Image X11)

ADD_EXECUTABLE          (Converter Converter.cxx)
TARGET_LINK_LIBRARIES   (Converter Color Common Image X11)

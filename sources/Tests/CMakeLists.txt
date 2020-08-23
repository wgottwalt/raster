ADD_EXECUTABLE          (Test_Endianess Test_Endianess.cxx)
ADD_EXECUTABLE          (Test_Vector2 Test_Vector2.cxx)
ADD_EXECUTABLE          (Test_Vector3 Test_Vector3.cxx)
ADD_EXECUTABLE          (Test_Vector4 Test_Vector4.cxx)

ADD_EXECUTABLE          (Test_PPM Test_PPM.cxx)
TARGET_LINK_LIBRARIES   (Test_PPM Color Image X11)

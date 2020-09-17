ADD_LIBRARY             (TestCases TestCases.cxx)

ADD_EXECUTABLE          (Test_Endianess Test_Endianess.cxx)
ADD_EXECUTABLE          (Test_Vector2 Test_Vector2.cxx)
ADD_EXECUTABLE          (Test_Vector3 Test_Vector3.cxx)
ADD_EXECUTABLE          (Test_Vector4 Test_Vector4.cxx)

ADD_EXECUTABLE          (Test_LZW16 Test_LZW16.cxx)
TARGET_LINK_LIBRARIES   (Test_LZW16 Compression)
ADD_EXECUTABLE          (Test_PPM Test_PPM.cxx)
TARGET_LINK_LIBRARIES   (Test_PPM Color Image TestCases X11)
ADD_EXECUTABLE          (Test_Simple Test_Simple.cxx)
TARGET_LINK_LIBRARIES   (Test_Simple Color Image TestCases X11)

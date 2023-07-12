#--------------------------------------------------------
# The CMakeLists.txt for:                       pYOLO
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  YOLO.cpp
  YOLO_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pYOLO ${SRC})

TARGET_LINK_LIBRARIES(pYOLO
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)


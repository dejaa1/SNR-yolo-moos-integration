#--------------------------------------------------------
# The CMakeLists.txt for:                       pLocationCalculation
# Author(s):                              Shaurya Sen
#--------------------------------------------------------

SET(SRC
  LocationCalculation.cpp
  LocationCalculation_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pLocationCalculation ${SRC})

TARGET_LINK_LIBRARIES(pLocationCalculation
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)


/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: YOLO.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef YOLO_HEADER
#define YOLO_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"


class YOLO : public AppCastingMOOSApp
{
 public:
   YOLO();
   ~YOLO();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 

 private: // State variables
  bool found;
  int mob_box_x;
  int mob_box_y;
  float confidence;
  int class_label;

};

#endif 

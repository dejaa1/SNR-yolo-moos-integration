/************************************************************/
/*    NAME: Shaurya Sen                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: LocationCalculation.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef LocationCalculation_HEADER
#define LocationCalculation_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class LocationCalculation : public AppCastingMOOSApp
{
 public:
   LocationCalculation();
   ~LocationCalculation();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   std::string exec(const char* cmd);
   
   

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
 

 private: // State variables
  double current_x;
  double current_y;
  double current_heading;
  double mob_box_y;
  double mob_box_x;
  double prev_mob_box_x;
  double prev_mob_box_y;
};

#endif 

/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: YOLO.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef YOLO_HEADER
#define YOLO_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

#include <chrono>

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;
using DT = TimePoint::duration;
using DTSeconds = std::chrono::duration<double, std::ratio<1, 1>>;

class YOLOBox 
{
private:
  TimePoint _time;
  int _class_label;
  int _x1, _y1;
  int _x2, _y2;

public:
  YOLOBox() 
    : _time{Clock::now()}
    , _class_label{-1}
    , _x1{-1}, _y1{-1}
    , _x2{-1}, _y2{-1} 
    {}

  YOLOBox(int label, int x1, int y1, int x2, int y2) 
    : _time{Clock::now()}
    , _class_label{label}
    , _x1{x1}, _y1{y1}
    , _x2{x2}, _y2{y2} 
    {}

public:
  int GetLabel() { return _class_label; }

  int BoxX() { return (_x1 + _x2) / 2; }
  int BoxY() { return _y2; }

  double GetAgeSeconds() { return std::chrono::duration_cast<DTSeconds>(Clock::now() - _time).count(); }
};

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

 private: // Configuration variables
  int class_label;

 private: // State variables
  bool found;
  YOLOBox mob_box;
  double confidence;
};

#endif 

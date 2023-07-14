/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: YOLO.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "YOLO.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>
#include <future>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>


using namespace std;
std::atomic<int> g_mob_box_x(-1);
std::atomic<int> g_mob_box_y(-1);
std::atomic<bool> g_mob_detected_bool(false);

//---------------------------------------------------------
// Constructor()
#define MOB_CLASS 0
#define PERSON_CLASS 0
#define LIFEVEST_CLASS 0

YOLO::YOLO()
{
  mob_box_x = -1;
  mob_box_y= -1;
  class_label = -1;
  confidence = -1;
  found =false;
}

//---------------------------------------------------------
// Destructor

YOLO::~YOLO()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool YOLO::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool YOLO::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second



void getCoords() {
    std::string fifoPath = "yolo/pipe/yolo_fifo";
    std::ifstream fifo_file(fifoPath);
    std::string input;

    if (!fifo_file.is_open())
    {
        std::cerr << "Failed to open the named pipe (FIFO).\n";

        return;
    }
    if (std::getline(fifo_file, input)) {
        // Process the input using the parser
        // Update the global variables directly
        std::istringstream iss(input);
        int temp_class_label_val, temp_x, temp_y;
        iss >> temp_class_label_val >> temp_x >> temp_y;
        if (temp_class_label_val == MOB_CLASS) {
        g_mob_box_x = temp_x;
        g_mob_box_y = temp_y;
        g_mob_detected_bool = true;
        }
    }
    fifo_file.close();
}


bool YOLO::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
    getCoords();

    mob_box_x = g_mob_box_x;
    mob_box_y = g_mob_box_y;
    found = g_mob_detected_bool;

    Notify("MOB_X",  mob_box_x);
    Notify("MOB_Y",  mob_box_y);
    //Notify("CLASS_LABEL", class_label);
    //Notify("CONFIDENCE", confidence);
    Notify("MOB_DETECTED", found);

    

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool YOLO::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();
  std::string input_source = "0";

  // std::string command = "mkfifo mypipe; (python detect.py --weights yolov5s.pt --img 640 --conf 0.25 --source " + input_source + " --save-txt | tee mypipe > /dev/null &) && ./YOLO < mypipe; rm mypipe";
  // std::thread commandThread([command]() {
  //       system(command.c_str());
  //   });

  //   commandThread.detach();

  //   std::thread getCoordsThread(getCoords);
  //   getCoordsThread.detach();

pid_t childPid = fork();

    // Check if it's the child process
    if (childPid == 0)
    {
        // Run the Python script in a separate terminal
        std::string command = "gnome-terminal -- python detect.py --weights yolov5s.pt --img 640 --conf 0.25 --source " + input_source + " --save-txt";  // Replace with the actual path to your Python script

        // Execute the command using exec
        if (execl("/bin/sh", "sh", "-c", command, NULL) == -1)
        {
            std::cerr << "Failed to start the Python script." << std::endl;
            return 1;
        }
    }
    else if (childPid < 0)
    {
        // Forking failed
        std::cerr << "Failed to fork the process." << std::endl;
        return 1;
    }

  return(true);
  
  // system("python3 yolo_main.py");
  
}

//---------------------------------------------------------
// Procedure: registerVariables()

void YOLO::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  

  
}


//------------------------------------------------------------
// Procedure: buildReport()

bool YOLO::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}





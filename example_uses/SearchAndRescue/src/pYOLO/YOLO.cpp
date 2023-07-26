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
#include <array>
#include <cstdlib>
#include <mutex>


using namespace std;


//---------------------------------------------------------
// Constructor()
#define MOB_CLASS 0
#define PERSON_CLASS 0
#define LIFEVEST_CLASS 0

#define MAX_FIFO_AGE 0.5

YOLO::YOLO()
{
  mob_box = YOLOBox();
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
  for (p = NewMail.begin(); p != NewMail.end(); p++)
  {
    CMOOSMsg &msg = *p;
    string key = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    if (key == "FOO")
      cout << "great!";

    else if (key != "APPCAST_REQ") // handled by AppCastingMOOSApp
      reportRunWarning("Unhandled Mail: " + key);
  }

  return (true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool YOLO::OnConnectToServer()
{
  registerVariables();
  return (true);
}

//---------------------------------------------------------
// Procedure: CoordsFIFOThread()
//            handle the incoming data from the python YOLO detector
static std::mutex g_mob_box_mutex;

static YOLOBox g_mob_box{};

static void CoordsFIFOThread()
{
  const std::string input_source = "0";
  const std::string weights = "src/pYOLO/yolo/custom_models/best.pt";
  const std::string python = "/Users/shauryasen/moos-ivp-shaurya/.venv/bin/python3";
  const std::string dir = "src/pYOLO/yolo";
  const std::string python_script = dir + "detect.py";
  const std::string requirements = dir + "requirements.txt";

  std::system((python + " -m pip install -r " + requirements).c_str());

  const std::string command = python + " " + python_script + " --weights " + weights + " --img 640 --conf 0.25 --source " + input_source + " --save-txt";
  FILE* python_program = popen(command.c_str(), "r");

  if (python_program == nullptr) {
    std::cerr << "popen() failed! 😅😅" << std::endl;
    return;
  }

  std::array<char, 256> buffer{'\0'};
  while (fgets(buffer.data(), buffer.size(), python_program) != nullptr)
  {
    const std::string data = std::string(buffer.data());
    buffer.fill('\0');

    std::istringstream fifo_data = std::istringstream(data);

    int label, x1, y1, x2, y2;
    if (fifo_data >> label >> x1 >> y1 >> x2 >> y2) {
        YOLOBox fifo_box = YOLOBox(label, x1, y1, x2, y2);

        switch (fifo_box.GetLabel())
        {
        case MOB_CLASS:
          const std::lock_guard<std::mutex> lock{g_mob_box_mutex};
          g_mob_box = fifo_box;
          break;
        default:
          std::cerr << "Unknown Classification Label: " << fifo_box.GetLabel() << std::endl;
          break;
      }
    } else {
      std::cerr << "Incorrect Formatting from FIFO Data: " << data << std::endl;
    }
  }
  
  pclose(python_program);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool YOLO::Iterate()
{
  AppCastingMOOSApp::Iterate();

  // Do your thing here!
  const std::lock_guard<std::mutex> lock{g_mob_box_mutex};
  mob_box = g_mob_box;
  
  bool detected = mob_box.GetAgeSeconds() < MAX_FIFO_AGE;
  Notify("MOB_BOX_X", mob_box.BoxX());
  Notify("MOB_BOX_Y", mob_box.BoxY());
  Notify("MOB_DETECTED", detected);
  Notify("MOB_AGE", mob_box.GetAgeSeconds());

  // AppCastingMOOSApp::PostReport(); 
  return (true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool YOLO::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if (!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for (p = sParams.begin(); p != sParams.end(); p++)
  {
    string orig = *p;
    string line = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if (param == "foo")
    {
      handled = true;
    }
    else if (param == "bar")
    {
      handled = true;
    }

    if (!handled)
      reportUnhandledConfigWarning(orig);
  }

  registerVariables();

  std::thread fifo_thread = std::thread(CoordsFIFOThread);
  fifo_thread.detach();

  return (true);

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
  // m_msgs << "============================================" << endl;
  // m_msgs << "File:                                       " << endl;
  // m_msgs << "============================================" << endl;

  // ACTable actab(4);
  // actab << "Alpha | Bravo | Charlie | Delta";
  // actab.addHeaderLines();
  // actab << "one" << "two" << "three" << "four";
  // m_msgs << actab.getFormattedString();

  return (true);
}

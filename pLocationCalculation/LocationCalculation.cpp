/************************************************************/
/*    NAME: Shaurya Sen                                              */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: LocationCalculation.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "LocationCalculation.h"

using namespace std;

//---------------------------------------------------------
// Constructor()

LocationCalculation::LocationCalculation()
{
  current_x = 0;
  current_y = 0;
}

//---------------------------------------------------------
// Destructor

LocationCalculation::~LocationCalculation()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool LocationCalculation::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
    // update x
    if (key== "NAV_X") {
      current_x = msg.GetDouble();
    }
    // update y
    if (key=="NAV_Y") {
      current_y = msg.GetDouble();
    }
    // update heading
    if (key=="NAV_HEADING") {
      current_heading = msg.GetDouble();
    }

    // if MOB (Man Over Board) is detected
    // MOB_PIXEL is the pixel coords of the middle of the bottom side of the bounding box in the image with the MOB
    if (msg.GetKey() == "MOB_PIXEL") {

      // 1. Calculate distance from camera to human 
      std::string command = "python homography.py " + std::to_string(x) + " " + std::to_string(y);
			std::string result = exec(command.c_str());

      // 2. Get GPS coords of robot

			// 3. Find coords of human

			// 4. Publish MOOS variable with estimated distance and human coords


    }


     
  }
    
}

std::string exec(const char* cmd) {

	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
	return result;
}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool LocationCalculation::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool LocationCalculation::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool LocationCalculation::OnStartUp()
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
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables()

void LocationCalculation::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_HEADING", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool LocationCalculation::buildReport() 
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





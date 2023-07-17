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
#include <array>

using namespace std;

//---------------------------------------------------------
// Constructor()

LocationCalculation::LocationCalculation()
{
  current_x = 0;
  current_y = 0;
  mob_box_x = -1;
  mob_box_y = -1;
  prev_mob_box_x = -1;
  prev_mob_box_y = -1;
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

    // if MOB (Man Over Board) is detected then MOB_BOX_X and Y will be posted
    // MOB_BOX_X and Y are the coords of the bottom middle of the bounding box in the image with a MOB detected.
    if (msg.GetKey() == "MOB_BOX_X") {
        mob_box_x = msg.GetDouble();
        cout << "mob_box_x: " << mob_box_x << endl;
        
    }

    if (msg.GetKey() == "MOB_BOX_Y") {
        mob_box_y = msg.GetDouble();
        cout << "mob_box_x: " << mob_box_x << endl;
        
    }

    // if new mail for both MOB_BOX_X and MOB_BOX_Y came then proceed with calculations
    if(! ( (mob_box_y == prev_mob_box_y) && (mob_box_x == prev_mob_box_x) ) ) {
        
        /// Calculate distance from camera to human ///
        std::string command = "/Users/shauryasen/moos-ivp-shaurya/bin/homography_sys " + std::to_string(mob_box_x) + " " + std::to_string(mob_box_y);
	      cout << "Python command assembled: " << command << endl;
        std::string result = exec(command.c_str()); 
        
        std::istringstream ss(result);
        std::string token;
        
        std::getline(ss, token, ',');
        double x = std::stod(token);

        std::getline(ss, token, ',');
        double y = std::stod(token);

        cout << "parsed x: " << x << endl;
        cout << "parsed y: " << y << endl;
        

        // publish MOOS variables with relative distance to MOB when MOB is scene ///
        // x and y is from robot pov. Y is how far out it is, and x is how left and right it is
        // yolo app will publish a boolean regarding whether MOB is found or not
        Notify("RELATIVE_MOB_X",  x);
        Notify ("RELATIVE_MOB_Y", y);

        prev_mob_box_x = mob_box_x;
        prev_mob_box_y = mob_box_y;
    }
   }
   return (true);
     
  }

// for parsing python script output
std::string LocationCalculation::exec(const char* cmd) {
  
	std::array<char, 128> buffer;
	std::string result;
	std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

	if (!pipe) {
		throw std::runtime_error("popen() failed!");
	}
	while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
		result += buffer.data();
	}
  // Notify ("RELATIVE_MOB_X", 5);
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
  // AppCastingMOOSApp::PostReport();
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
  Register("MOB_BOX_X", 0 );
  Register("MOB_BOX_Y", 0 );
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





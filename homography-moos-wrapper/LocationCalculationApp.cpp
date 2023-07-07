#include "LocationCalculationApp.h"

bool LocationCalculationApp::Iterate()
{
    
    return true;
}

bool LocationCalculationApp::OnNewMail(MOOSMSG_LIST &NewMail) override
     //mail: HumanOverboardDetected?
    {
        // If I get mail with human overboard detected:
        //   1. calculate distance from camera to human
        //   2. get gps coords of robot
        //   3. find coords of human
        // Publish MOOS variable with estimated distance
    }

bool LocationCalculationApp::OnStartUp()
{
    
    return true;
}

void LocationCalculationApp::RegisterVariables()
{
    
}
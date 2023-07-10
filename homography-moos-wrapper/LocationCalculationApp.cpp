#include "LocationCalculationApp.h"
#include <cstdlib> 
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

bool LocationCalculationApp::Iterate()
{
    
    return true;
}

bool LocationCalculationApp::OnNewMail(MOOSMSG_LIST &NewMail)
{

	
	MOOSMG_LIST::iterator p;


	for(p = NewMail.begin(); p != NewMail.end(); p++) {
		CMOOSMsg &msg = *p;

		if(msg.GetKey() == "HumanOverboardDetected") {

			// 1. Calculate distance from camera to human
			double x = msg.GetDouble();
			double y = msg.getDouble();
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

bool LocationCalculationApp::OnStartUp()
{
    RegisterVariables();
    return true;
}

void LocationCalculationApp::RegisterVariables()
{
    m_Comms.Register("HumanOverboardDetected", 0);
}
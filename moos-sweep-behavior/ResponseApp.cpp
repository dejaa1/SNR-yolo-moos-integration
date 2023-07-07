#include "ResponseApp.h"

bool SweepApp::Iterate()
{
    return true;
}

bool SweepApp::OnNewMail(MOOSMSG_LIST &NewMail) override
    {
        // If I get mail with position of the bot, send position to shoreside computers
    }

bool SweepApp::OnStartUp()
{
    
    return true;
}

void SweepApp::RegisterVariables()
{
    
}
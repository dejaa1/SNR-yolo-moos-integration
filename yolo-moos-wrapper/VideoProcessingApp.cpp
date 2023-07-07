#include "VideoProcessingApp.h"

bool VideoProcessingApp::Iterate()
{
    // Capture video frame
    // Process frame to detect humans
    // If human detected, publish MOOS variable with location in image
    return true;
}

bool VideoProcessingApp::OnNewMail(MOOSMSG_LIST &NewMail)
{
    
    return true;
}

bool VideoProcessingApp::OnStartUp()
{
    // startup
    return true;
}

void VideoProcessingApp::RegisterVariables()
{
    
}
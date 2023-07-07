#pragma once
#include "MOOS/libMOOS/MOOSLib.h"

class VideoProcessingApp : public CMOOSApp
{
public:
	VideoProcessingApp();
	virtual ~VideoProcessingApp();
	

    bool Iterate() override;
    bool OnNewMail(MOOSMSG_LIST &NewMail) override;
    bool OnStartUp() override;
    void RegisterVariables() override;
};
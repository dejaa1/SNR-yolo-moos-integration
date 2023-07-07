#include "MOOS/libMOOS/MOOSLib.h"

class ResponseApp : public CMOOSApp
{
public:

	ResponseApp();
	virtual ~ResponseApp();
	
    bool Iterate() override;
    bool OnNewMail(MOOSMSG_LIST &NewMail) override;
    bool OnStartUp() override;
    void RegisterVariables() override;
};
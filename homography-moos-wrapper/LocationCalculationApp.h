#include "MOOS/libMOOS/MOOSLib.h"

class LocationCalculationApp : public CMOOSApp
{
public:
	LocationCalculationApp();
	virtual ~LocationCalculationApp();
    bool Iterate() override;
    bool OnNewMail(MOOSMSG_LIST &NewMail) override;
    bool OnStartUp() override;
    void RegisterVariables() override;
};
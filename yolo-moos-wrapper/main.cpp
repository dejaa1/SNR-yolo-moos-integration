#include "VideoProcessingApp.h"
#include "LocationProcessingApp.h"
#include "ResponseApp.h"

int main(int argc, char *argv[])
{
    // Create instances of your applications
    VideoProcessingApp videoProcessingApp;
    LocationCalculationApp locationCalculationApp;
    ResponseApp responseApp;

    // Start the MOOS application
    videoProcessingApp.Run("VideoProcessingApp", argc, argv);
    locationProcessingApp.Run("DistanceCalculationApp", argc, argv);
    responseApp.Run("ResponseApp", argc, argv);

    return 0;
}

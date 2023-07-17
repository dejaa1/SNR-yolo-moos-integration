/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT, Cambridge MA                               */
/*    FILE: YOLO.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "YOLO.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <thread>
#include <future>
#include <atomic>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>


using namespace std;
// std::atomic<int> g_mob_box_x(-1);
// std::atomic<int> g_mob_box_y(-1);
// std::atomic<bool> g_mob_detected_bool(false);
std::string running_check = "Not Running";
std::string fifo_open = "Not Open";
std::string left_newmail = "Did not leave NewMail";
std::chrono::steady_clock::time_point lastTrueTime;

//---------------------------------------------------------
// Constructor()
#define MOB_CLASS 0
#define PERSON_CLASS 0
#define LIFEVEST_CLASS 0

YOLO::YOLO()
{
  mob_box_x = -1;
  mob_box_y= -1;
  class_label = -1;
  confidence = 0.0f;
  found =false;
}

//---------------------------------------------------------
// Destructor

YOLO::~YOLO()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail()

bool YOLO::OnNewMail(MOOSMSG_LIST &NewMail)
{
  left_newmail = "Did not leave NewMail";
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
    left_newmail = "Left NewMail";

   return(true);

}

//---------------------------------------------------------
// Procedure: OnConnectToServer()

bool YOLO::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

void NewGetCoords(bool &found_bool, int & class_val, float & conf, int & x, int &y)
{

    std::string fifoPath = "src/pYOLO/yolo/pipe/yolo_fifo";
    std::ifstream fifo_file(fifoPath);
    std::string input;

    if (!fifo_file.is_open())
    {
        std::cerr << "Failed to open the named pipe (FIFO).\n";

        return;
    }
    if (std::getline(fifo_file, input)) {
        // Process the input using the parser
        // Update the global variables directly
        fifo_open = "working";
        cout << input << endl;

        std::istringstream iss(input);
        
        iss >> class_val >> conf >> x >> y;
        if (class_val == LIFEVEST_CLASS) {
        found_bool = true;
        cout << "BRUHHHHH WE IN THE BOOL CHSANGER NOW" << endl;
        
        //std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        }
        else{
          
          fifo_open = "Not Open";
        }
    }
    fifo_file.close();

}
// void getCoords() {
//     std::string fifoPath = "src/pYOLO/yolo/pipe/yolo_fifo";
//     std::ifstream fifo_file(fifoPath);
//     std::string input;

//     if (!fifo_file.is_open())
//     {
//         std::cerr << "Failed to open the named pipe (FIFO).\n";

//         return;
//     }
//     if (std::getline(fifo_file, input)) {
//         // Process the input using the parser
//         // Update the global variables directly
//         fifo_open = "working";
//         // cout << input << endl;

//         std::istringstream iss(input);
//         int temp_class_label_val, temp_x, temp_y;
//         iss >> temp_class_label_val >> temp_x >> temp_y;
//         if (temp_class_label_val == MOB_CLASS) {
//         g_mob_box_x = temp_x;
//         g_mob_box_y = temp_y;
//         g_mob_detected_bool = true;
//         }
//         else{
//           g_mob_detected_bool = false;
//           fifo_open = "Not Open";
//         }
//     }
//     fifo_file.close();
// }


bool YOLO::Iterate()
{
  AppCastingMOOSApp::Iterate(); 
  // Do your thing here!
    lastTrueTime = std::chrono::steady_clock::now();
    NewGetCoords(found, class_label, confidence, mob_box_x, mob_box_y);
    /// if difff btwen tlast and now > 500 moiloise then detected = flase
std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
  std::chrono::duration<double> duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - lastTrueTime);
  double elapsedSeconds = duration.count();


    cout<< duration.count() << endl;

    if (found && elapsedSeconds >= 0.5) {

      found = false;
    }

    left_newmail = "Left NewMail";


    Notify("MOB_BOX_X",  mob_box_x);
    Notify("MOB_BOX_Y",  mob_box_y);
    //Notify("CLASS_LABEL", class_label);
    //Notify("CONFIDENCE", confidence);
    Notify("MOB_DETECTED", found);
    Notify("MOB_CONF", confidence);
    Notify("CLASS_LABEL", class_label);


  AppCastingMOOSApp::PostReport();

  return(true);
}

void runPythonScript()
{
    pid_t childPid = fork();

    if (childPid == 0)
    {
        // Run the Python script in a separate process
        const char* command = "/usr/bin/python";
        const char* scriptPath = "src/pYOLO/yolo/detect.py";

        const char* args[] = {
            command,
            scriptPath,
            "--weights",
            "src/pYOLO/yolo/custom_model/best.pt",
            "--img",
            "640",
            "--conf",
            "0.25",
            "--source",
            "0",
            "--output-moos",
            NULL
        };

        execvp(args[0], const_cast<char* const*>(args));
        std::cerr << "Failed to start the Python script." << std::endl;
        exit(1);
    }
    else if (childPid < 0)
    {
        std::cerr << "Failed to fork the process." << std::endl;
        exit(1);
    }

 
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool YOLO::OnStartUp()
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
  
  // std::string command = "mkfifo mypipe; (python detect.py --weights yolov5s.pt --img 640 --conf 0.25 --source " + input_source + " --save-txt | tee mypipe > /dev/null &) && ./YOLO < mypipe; rm mypipe";
  // std::thread commandThread([command]() {
  //       system(command.c_str());
  //   });

  //   commandThread.detach();

  //   std::thread getCoordsThread(getCoords);
  //   getCoordsThread.detach();

// pid_t childPid = fork();
    //  std::string input_source = "0";

//     // Check if it's the child process
//     if (childPid == 0)
//     {   
//         // Run the Python script in a separate terminal
//         std::string command = "gnome-terminal -- python src/pYOLO/yolo/detect.py --weights src/pYOLO/yolo/yolov5s.pt --img 640 --conf 0.25 --source " + input_source + " --save-txt"; 

//         // Execute the command using exec
//         if (execl("/bin/sh", "sh", "-c", command, NULL) == -1)
//         {
//             std::cerr << "Failed to start the Python script." << std::endl;
//             return 1;
//         }
//     }
//     else if (childPid < 0)
//     {
//         // Forking failed
//         std::cerr << "Failed to fork the process." << std::endl;
//         return 1;
//     }


  
  ///python src/pYOLO/yolo/detect.py --weights src/pYOLO/yolo/yolov5s.pt --img 640 --conf 0.25 --source 0 --save-txt
 
  runPythonScript();

  sleep(7);

  return(true);
  
  // system("python3 yolo_main.py");
  
}

//---------------------------------------------------------
// Procedure: registerVariables()

void YOLO::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  

  
}


//------------------------------------------------------------
// Procedure: buildReport()

bool YOLO::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File: Yolo.cpp                                      " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  
  actab << "RUNNING? | FIFO OPEN? | leftNEWMAIL | Delta";
  
  actab.addHeaderLines();
  actab << running_check << fifo_open << left_newmail << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}





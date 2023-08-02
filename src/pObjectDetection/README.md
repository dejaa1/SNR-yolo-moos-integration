# Object Detection App
This repository contains the source code for the Object Detection App, a component of an autonomous marine system that uses the YOLO object detection algorithm to identify objects of interest in the environment.<br>

## Overview
The Object Detection App is responsible for operating the robot's camera and executing the YOLO detection script to scan the surroundings for a Man Overboard (MOB). Upon detecting an MOB, the app publishes the pixel coordinates of the MOB within the image to the MOOSDB. Other components of the system can then retrieve this data to find the relative position of the MOB.

## Code Structure
The main file of the Object Detection App is ObjectDetection.cpp. This file contains the implementation of the ObjectDetection class, which is responsible for the main functionality of the app.

### The Object Detection App publishes:
- `MOB_BOX_X`: The x-coordinate of the bottom middle of the bounding box in the
image where a MOB (Man Overboard) is detected.
- `MOB_BOX_Y`: The y-coordinate of the bottom middle of the bounding box in the
image where a MOB is detected.
- `MOB_DETECTED`: A boolean value indicates whether a MOB is detected (true) or not
(false).
- `MOB_AGE`: How long in seconds it has been since a MOB has been detected.

To use the Object Detection App, you need to have a running MOOSDB and a robot with a camera that the app can control.


## Contributing
Contributions to the Object Detection App are welcome. Please submit a pull request or open an issue if you have any improvements or bug fixes.

## License
The Object Detection App is open source and free to use under the MIT license.

# Homography MOOS App

This repository contains the source code for the Homography MOOS App, a critical component in an autonomous marine system that calculates the relative position of detected objects.

## Overview

The Homography MOOS App is designed to work in conjunction with the YOLO MOOS App. It subscribes to the pixel coordinates published by the YOLO app when a Man Overboard (MOB) is detected. Upon receiving these coordinates, the app runs the homography script to calculate the relative x and y coordinates of the MOB. By combining this information with the robot's current position, heading, and the relative x and y coordinates, the app determines the position of the MOB in relation to the field's origin.

## Code Structure

The main file of the Homography MOOS App is `LocationCalculation.cpp`. This file contains the implementation of the location calculation logic.

### Subscribes to:

- `MOB_BOX_X`: The x-coordinate of the detected MOB in the image.
- `MOB_BOX_Y`: The y-coordinate of the detected MOB in the image.
- `NAV_X`: field relative x position of the bot.
- `NAV_Y`: field relative y position of the bot.
- `NAV_HEADING`: Heading of the bot.

### Publishes:

- `RELATIVE_MOB_X`: The relative x-coordinate of the MOB in the field.
- `RELATIVE_MOB_Y`: The relative y-coordinate of the MOB in the field.
- `MOB_POSITION_X`: The calculated X position of the MOB in relation to the field's origin.
- `MOB_POSITION_Y`: The calculated Y position of the MOB in relation to the field's origin.

## Contributing

Contributions to the Homography MOOS App are welcome. Please submit a pull request or open an issue if you have any improvements or bug fixes.

## License

The Homography MOOS App is open source and free to use under the MIT license.

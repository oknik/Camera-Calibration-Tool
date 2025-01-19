# Camera Calibration Tool

The course project of Computer Vision, 2024 fall.

## Overview

This is a camera calibration tool developed using Qt 5.15 and C++. It supports both Linux and Windows operating systems and helps users calculate and correct camera parameters.

## Structure

- **Qt-Project**: Contains the Qt project configuration files and other project-related files. The Qt project runs based on this structure.
- **Resources**: Contains class diagrams and other design materials describing the project structure and functionality.
- **Source-Code**: Contains the core function code. These functions are compiled into DLL and SO files, which are then used within the Qt project.
- **UnitTest**: Contains unit tests used to validate the correctness and functionality of the code.

## Notes
- Due to the inclusion of large DLL and SO files, they cannot be uploaded to GitHub, so the project cannot be run directly. To run the project, the code in the `Source-Code` folder need to be compiled into DLL (on Windows) and SO (on Linux) files, which should then be added to the Qt project. 
- OpenCV 4.10.0 library is required and should be added to the project for image processing functionalities. 


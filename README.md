# Port of undistortFunctions
Updated version of [undistortFunctions](https://sites.google.com/site/scarabotix/ocamcalib-toolbox/ocamcalib-toolbox-download-page).

This project ports the code to OpenCV 4 and includes a Dockerfile for building and testing the code.

# References
Cited from the [homepage](https://sites.google.com/site/scarabotix/ocamcalib-toolbox/ocamcalib-toolbox-download-page) of the project:

Scaramuzza, D., Martinelli, A. and Siegwart, R.. "A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion", Proceedings of IEEE International Conference of Vision Systems (ICVS'06), New York, January 5-7, 2006.[PDF]

Scaramuzza, D., Martinelli, A. and Siegwart, R.. "A Toolbox for Easy Calibrating Omnidirectional Cameras", Proceedings to IEEE International Conference on Intelligent Robots and Systems (IROS 2006), Beijing China, October 7-15, 2006. [PDF]

Rufli, M., Scaramuzza, D., and Siegwart, R., Automatic Detection of Checkerboards on Blurred and Distorted Images,Proceedings of the IEEE/RSJ International Conference on Intelligent Robots and Systems (IROS 2008), Nice, France, September 2008.[PDF]

## Original README.txt

******************************************************************************************
  Example code that shows the use of the 'cam2world" and 'world2cam" functions.
  It shows also how to undistort images into perspective or panoramic images.

  This code requires the OpenCV library installed.

    Copyright (C) 2009 DAVIDE SCARAMUZZA, ETH Zurich
    Author: Davide Scaramuzza - email: davide.scaramuzza@ieee.org
******************************************************************************************

This codes implements in C some useful routines that can be used after having calibrated your omnidirectional camera using the OCamalib Toolbox for Matlab.

Compile all including the OpenCV library (http://sourceforge.net/projects/opencvlibrary/ and download the last version of the OpenCV library (it is free!))
Then, run the executable. This will read the two test images and will generate the two undistorted images (perspective and panoramic respectively)

For using the C functions with your omnidirectional camera, you first have to calibrate the camera using the OCamCalib Toolbox for Matlab.
Then, you need to export your ocam_model structure (in Matlab) into a TXT file
For doing this within MATLAB, type:
>> export_data(ocam_model);
This function will generate the calibr_results.txt file which can be read with my C/C++ routines.

ocam_functions.h and ocam_functions.cpp implement several C functions:
Among the others, the most important ones are:
1. cam2world (same use as the Matlab function): back-projects a pixel point onto the unit sphere
2. world2cam (same use as the Matlab function): projects a 3D scene point onto the image

There are also a few other functions for undistorting you image into a perspetive one or a panoramic one.

To facilitate the undertanding, the main.cpp is organized in the form of a tutorial which explains step by step how to use all the available functions.

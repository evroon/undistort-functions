#!/bin/bash

docker run -v $(pwd):/undistort $USER/undistort-functions:latest ./build.sh


docker run -ti --rm \
       -e DISPLAY=$DISPLAY \
       -v /tmp/.X11-unix:/tmp/.X11-unix \
       -v $(pwd):/undistort \
       $USER/undistort-functions:latest \
       ./build/undistort

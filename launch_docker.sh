#!/bin/bash

docker run -it -v $(pwd):/undistort $USER/undistort-functions:latest ./build.sh
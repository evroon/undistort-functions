#!/bin/bash

docker run -v $(pwd):/undistort $USER/undistort-functions:latest ./build.sh

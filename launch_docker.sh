#!/bin/bash

COMMAND="/bin/bash"

while test $# -gt 0
do
    case "$1" in
        --run)
			input_model="$2"
			input_img="$3"
			output_img="$4"
            COMMAND="build/undistort $input_model $input_img $output_img"
            shift
            shift
            shift
        ;;
        --run-example)
            COMMAND="./build.sh && build/undistort examples/calib_results_fisheye.txt examples/test_fisheye.jpg examples/undistorted_perspective.jpg"
        ;;
        --build)
            COMMAND="./build.sh"
        ;;
    esac
    shift
done

docker run --rm \
    -v $(pwd):/undistort \
    $USER/undistort-functions:latest \
    bash -c "$COMMAND"

#!/bin/bash
cd "$(dirname "$0")"

COMMAND="/bin/bash"
volumes="-v $(pwd):/undistort"
interactive_options="-it"

common_prefix() {
  local n=0
  while [[ "${1:n:1}" == "${2:n:1}" ]]; do
    ((n++))
  done
  echo "${1:0:n}"
}

while test $# -gt 0
do
    case "$1" in
        --run)
            interactive_options=""
			input_model="$2"
			input_img="$3"
			output_img="$4"
            COMMAND="build/undistort $input_model $input_img $output_img"

            # Determine overlapping part of input paths so that we can bind that directory as volume.
            volume=`common_prefix $input_img $output_img`

            echo "Adding Docker volume:" $volume
            volumes="$volumes -v $volume:$volume"

            shift
            shift
            shift
        ;;
        --run-example)
            interactive_options=""
            COMMAND="./build.sh && build/undistort examples/calib_results_fisheye.txt examples/test_fisheye.jpg examples/undistorted_perspective.jpg"
        ;;
        --build)
            interactive_options=""
            COMMAND="./build.sh"
        ;;
    esac
    shift
done

docker run $interactive_options --rm \
    $volumes \
    $USER/undistort-functions:latest \
    bash -c "$COMMAND"

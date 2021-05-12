FROM dkimg/opencv:4.5.2-ubuntu
LABEL maintainer="erik.vroon22@gmail.com"

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
  build-essential \
  cmake \
  curl \
  gcc \
  git \
  libglew-dev \
  libgtk2.0-dev \
  pkg-config \
  libavcodec-dev \
  libavformat-dev \
  libswscale-dev \
  python3-dev \
  python3-numpy \
  unzip

WORKDIR /undistort

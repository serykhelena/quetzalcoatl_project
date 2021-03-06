#!/bin/bash
clear

############ CONFIGURATION ############
# dev_name=/dev/video2
dev_name=/dev/v4l/by-id/usb-Sonix_Technology_Co.__Ltd._USB_2.0_Camera-video-index0

client_port=4000

cam_width=640
cam_height=480

bitrate_kbps=1024
keyframe_period_frames=20
############ CONFIGURATION ############

echo "Starting stream..."
gst-launch-1.0 -v v4l2src device=$dev_name ! \
    video/x-raw,width=$cam_width, height=$cam_height, framerate=30/1 ! \
    videoconvert ! tee name=h \
    h. ! queue ! \
        x264enc tune=zerolatency bitrate=$bitrate_kbps speed-preset=superfast \
            key-int-max=$keyframe_period_frames ! \
        tcpserversink host=0.0.0.0 port=$client_port \
            recover-policy=keyframe sync=false sync-method=next-keyframe \
    h. ! queue ! xvimagesink sync=false


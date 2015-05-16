#ifndef VIDEOCAP_H
#define VIDEOCAP_H

// import
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <linux/types.h>
// #include <linux/videodev.h>
#include <libv4l1-videodev.h>
#include <libv4lconvert.h>
#include <sys/mman.h>

#define VIDEO_DEVICE      "/dev/video1"
#define VIDEO_WIDTH       160
#define VIDEO_HEIGHT      120

#include <string>
#include <vector>
#include <iostream>

#include "img.h"

using namespace std;

class Videocap
{
	struct video_mbuf vid_bufv4l;

	public:
		static Videocap* Instance();

		void grab();
// 		void copyTo( vector<float>& img );
		char* image_new;

// 		char* image_current;
// 		vector<float> image_cur;
		Img image_cur;

	protected:
		Videocap();
		~Videocap();
	private:
		static Videocap* _instance;

		// import
		int video_width;
		int video_height;
		
		int video_input;
		int video_norm;
		

		int video_contrast;
		int video_brightness;
		int video_color;
		int video_hue;
		

		char* image_base;
		
		int vid_dev;
		char* vid_mmap;
		char* vid_alloc_map;
		int vid_alloc_size;
		int vid_mmap_size;

		int v4l_OpenDev( char *device );
		void v4l_CloseDev( void );
		int v4l_Settings ( void );
		char* v4l_DoCapture ( int dev, int width, int height );
		char* v4l_SetDevice ( int dev,  int width,  int height,  int input,  int norm );
		
		
		/* prototype */
/*		void v4l_CloseDev(
		  void
		);
		
		int v4l_OpenDev(
		  char *device
		);
		

		char *v4l_SetDevice (
		  int dev, 
		  int width, 
		  int height, 
		  int input, 
		  int norm
		);
		
		char *v4l_DoCapture (
		  int dev,
		  int width,
		  int height
		);
		
		int v4l_Settings (
		  void
		);*/
		
};

#endif

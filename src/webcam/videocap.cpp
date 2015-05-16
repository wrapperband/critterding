#include "videocap.h"

Videocap* Videocap::Instance () 
{
	static Videocap t;
	return &t;
}

Videocap::Videocap()
{
	video_width = VIDEO_WIDTH;
	video_height = VIDEO_HEIGHT;
	video_input = 1;
	video_norm = 6;
	video_contrast = 32768;
	video_brightness = 32768;
	video_color = 32768;
	video_hue = 32768;
	video_contrast = 0;
	video_brightness = 0;
	video_color = 0;
	video_hue = 0;
	image_new = 0;
	image_base = 0;
	
	vid_dev = -1;
	vid_mmap = (char *)-1;
	vid_alloc_map = 0;
	vid_alloc_size = 0;
	vid_mmap_size = 0;

	vid_dev = (int)v4l_OpenDev((char*)VIDEO_DEVICE);

	// init image_current float vector
	for ( unsigned int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT*3; i++ )
		image_cur.v.push_back(0.0f);
}

void Videocap::grab()
{
	if ( vid_dev>-1 )
	{
		image_new = v4l_DoCapture(vid_dev, video_width, video_height);
		
		// copy to image_current
		for ( int i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT*3; i++ )
		{
			image_cur.v[i] = (float)image_new[i] / 256.0f;
			if ( image_cur.v[i] < 0.0f )
				image_cur.v[i] += 1.0f;
		}
	}
}






/* v4l Functions */
char* Videocap::v4l_SetDevice ( int dev,  int width,  int height,  int input,  int norm )
{
	struct video_capability vid_caps;
	struct video_channel vid_chnl;
	struct video_window vid_win;

	if (ioctl (dev, VIDIOCGCAP, &vid_caps) == -1)
	{
		fprintf(stderr, "ioctl (VIDIOCGCAP) failed\n");
		return NULL;
	}
 
/*	   if (input || norm) {
	      vid_chnl.channel = -1;
	      if (ioctl (dev, VIDIOCGCHAN, &vid_chnl) == -1) {
		fprintf(stderr, "ioctl (VIDIOCGCHAN) failed\n");
	      } else {
		vid_chnl.channel = input;
		vid_chnl.norm    = norm;
		if (ioctl (dev, VIDIOCSCHAN, &vid_chnl) == -1) {
		    fprintf (stderr, "ioctl (VIDIOCSCHAN) failed\n");
		    return NULL;
		}
	      }
	  }*/
 
	v4l_Settings();
 
	if (ioctl (dev, VIDIOCGMBUF, &vid_bufv4l) == -1)
	{
		fprintf(stderr, "No memory-mapping available.\n");
		if (ioctl (dev, VIDIOCGWIN, &vid_win)== -1) {
			fprintf(stderr, "ioctl (VIDIOCGWIN) failed\n");
			return NULL;
		}
		vid_win.width=width;
		vid_win.height=height;
		if (ioctl (dev, VIDIOCSWIN, &vid_win)== -1) {
			fprintf(stderr, "ioctl (VIDIOCSWIN) failed\n");
			return NULL;
		}
		vid_alloc_map = (char*)malloc(width*height*3);
		vid_alloc_size = width*height*3;
		return vid_alloc_map;
	}
	else
	{
		vid_mmap=(char*)mmap(0, vid_bufv4l.size, PROT_READ|PROT_WRITE, MAP_SHARED, dev, 0);
      
		if ((unsigned char *)-1 == (unsigned char *) vid_mmap)
			return NULL;
		vid_mmap_size = vid_bufv4l.size;
      
		return vid_mmap;
	}
}
 

char* Videocap::v4l_DoCapture ( int dev, int width, int height )
{
	struct video_mmap t_mmap;
	int frame = 0;
      
	if (vid_alloc_map)
	{
		if (read(dev, vid_alloc_map, vid_alloc_size) != vid_alloc_size)
			return NULL;
      
		return vid_alloc_map;
	}
	else
	{
		t_mmap.format=VIDEO_PALETTE_RGB24;
		t_mmap.frame=frame;
	  
		t_mmap.width=width;
		t_mmap.height=height;
	      
		if (ioctl(dev, VIDIOCMCAPTURE, &t_mmap) == -1) {
			return NULL;
		}
	  
		if (ioctl(dev, VIDIOCSYNC, &t_mmap) == -1) {
			fprintf(stderr, "Error with sync!\n");
			return NULL;
		}
	  
		return vid_mmap + vid_bufv4l.offsets[frame];
	}
}
 

int Videocap::v4l_Settings ( void )
{
	struct video_picture vp;
      
	if (vid_dev == -1)
		return -1;
      
	if (ioctl (vid_dev, VIDIOCGPICT, &vp) == -1)
	{
		fprintf(stderr, "Error getting vp\n");
		return 1;
	}
      
	vp.contrast = video_contrast;
	vp.brightness = video_brightness;
	vp.colour = video_color;
	vp.hue = video_hue;
      //  vp.whiteness = video_whiteness;
      
	if (ioctl(vid_dev, VIDIOCSPICT, &vp) == -1)
	{
		fprintf(stderr, "Error getting vp\n");
		return 2;
	}
      
	return 0;
}
 

void Videocap::v4l_CloseDev( void )
{
	if (vid_mmap > 0)
		munmap(vid_mmap, vid_mmap_size);
	vid_mmap = 0;
      
	if (vid_alloc_map)
		free(vid_alloc_map);
	vid_alloc_map = 0;
      
	close(vid_dev);
	vid_dev = -1;
}
 

int Videocap::v4l_OpenDev( char *device )
{
	if (vid_dev)
	{
		v4l_CloseDev();
// 		sleep(1);
	}
      
	vid_dev=open(device, O_RDONLY);
      
	if (vid_dev < 0)
	{
		std::cout << "Open device error" << device << std::endl;
		fprintf(stderr, "Open device error\n");
		return -1;
	}
      
	/* Allow videodevice to settle in */
// 	sleep(1);
      
	image_base = v4l_SetDevice (vid_dev, video_width, video_height, video_input, video_norm);
      
	if (!image_base)
		return -2;
      
	return vid_dev;
}
 
Videocap::~Videocap()
{
	v4l_CloseDev();
// 	free(image_current);
}
 
/* End */




#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
 
#include <linux/types.h>
// #include <linux/videodev.h>
#include <./libv4l1-videodev.h>
 
#include <sys/mman.h>
 
/* Stole from calib.h */
#define VIDEO_DEVICE            "/dev/video1"
 
#define VIDEO_WIDTH       320
#define VIDEO_HEIGHT      240
 
/* Stole from v4l.c */
char video_device[50] = VIDEO_DEVICE;
 
int video_width = VIDEO_WIDTH;
int video_height = VIDEO_HEIGHT;
 
int video_input = 1;
int video_norm = 1;
 

int video_contrast = 32768;
int video_brightness = 32768;
int video_color = 32768;
int video_hue = 32768;
 

char *image_new = 0;
char *image_base = 0;
 
int vid_dev = -1;
char *vid_mmap = (char *)-1;
char *vid_alloc_map = 0;
int vid_alloc_size = 0;
int vid_mmap_size = 0;
 
struct video_mbuf vid_buf;
 
/* prototype */
void v4l_CloseDev(
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
);
 
/* Functions */
char *v4l_SetDevice (
   int dev, 
   int width, 
   int height, 
   int input, 
   int norm
)
{
   struct video_capability vid_caps;
   struct video_channel vid_chnl;
   struct video_window vid_win;
 
   if (ioctl (dev, VIDIOCGCAP, &vid_caps) == -1) {
      fprintf(stderr, "ioctl (VIDIOCGCAP) failed");
      return NULL;
   }
 
/*   if (input || norm) {
      vid_chnl.channel = -1;
      if (ioctl (dev, VIDIOCGCHAN, &vid_chnl) == -1) {
         fprintf(stderr, "ioctl (VIDIOCGCHAN) failed");
      } else {
         vid_chnl.channel = input;
         vid_chnl.norm    = norm;
         if (ioctl (dev, VIDIOCSCHAN, &vid_chnl) == -1) {
            fprintf (stderr, "ioctl (VIDIOCSCHAN) failed");
            return NULL;
         }
      }
   }*/
 
   v4l_Settings();
 
   if (ioctl (dev, VIDIOCGMBUF, &vid_buf) == -1) {
      fprintf(stderr, "No memory-mapping available.\n");
 
      if (ioctl (dev, VIDIOCGWIN, &vid_win)== -1) {
         fprintf(stderr, "ioctl (VIDIOCGWIN) failed");
         return NULL;
      }
      vid_win.width=width;
      vid_win.height=height;
      if (ioctl (dev, VIDIOCSWIN, &vid_win)== -1) {
         fprintf(stderr, "ioctl (VIDIOCSWIN) failed");
         return NULL;
      }
      vid_alloc_map = (char*)malloc(width*height*3);
      vid_alloc_size = width*height*3;
 
      return vid_alloc_map;
   } else {
      vid_mmap=(char*)mmap(0, vid_buf.size, PROT_READ|PROT_WRITE, MAP_SHARED, dev, 0);
 
      if ((unsigned char *)-1 == (unsigned char *) vid_mmap)
         return NULL;
 
      vid_mmap_size = vid_buf.size;
 
      return vid_mmap;
   }
}
 

char *v4l_DoCapture (
   int dev, 
   int width, 
   int height
)
{
   struct video_mmap t_mmap;
   int frame = 0;
 
   if (vid_alloc_map) {
      if (read(dev, vid_alloc_map, vid_alloc_size) != vid_alloc_size)
         return NULL;
 
      return vid_alloc_map;
   } else {
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
 
      return vid_mmap + vid_buf.offsets[frame];
   }
}
 

int v4l_Settings (
   void
)
{
   struct video_picture vp;
 
   if (vid_dev == -1)
      return -1;
 
   if (ioctl (vid_dev, VIDIOCGPICT, &vp) == -1) {
      fprintf(stderr, "Error getting vp\n");
      return 1;
   }
 
   vp.contrast = video_contrast;
   vp.brightness = video_brightness;
   vp.colour = video_color;
   vp.hue = video_hue;
//  vp.whiteness = video_whiteness;
 
   if (ioctl(vid_dev, VIDIOCSPICT, &vp) == -1) {
      fprintf(stderr, "Error getting vp\n");
      return 2;
   }
 
   return 0;
}
 

void v4l_CloseDev(
   void
)
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
 

int v4l_OpenDev(
  char *device
)
{
  if (vid_dev) { v4l_CloseDev(); sleep(1); }
 
  vid_dev=open(device, O_RDWR);
 
  if (vid_dev < 0) {
     fprintf(stderr, "Open device error\n");
     return -1;
  }
 
  /* Allow videodevice to settle in */
  sleep(1);
 
  image_base=v4l_SetDevice (vid_dev, video_width, video_height,
       video_input, video_norm);
 
  if (!image_base)
    return -2;
 
  return vid_dev;
}
 

/* End */
 

int main(int argc, char ** argv)
{
 /* new version */
 long i;
 FILE* im_file;
 
 im_file=fopen("imdata.txt","w");
 if (im_file==NULL) {
  fprintf(stderr, "cannot open imdata.txt\n");
  exit(1);
 }
 
 vid_dev = (int)v4l_OpenDev(VIDEO_DEVICE);
 

while (1)
{
	image_new = v4l_DoCapture(vid_dev, video_width, video_height);
	for (i = 0; i < VIDEO_WIDTH * VIDEO_HEIGHT*3; i++) {
		fprintf(im_file, "%x", image_new[i]);
	}
}
    
	fclose(im_file);
	v4l_CloseDev();
 
  return 0;
}
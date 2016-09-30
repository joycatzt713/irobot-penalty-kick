#include <pthread.h>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include "tracker.h"
#include <createoi.h>
#include <math.h>
//color to track

Color targetColor = {0,0,0,0,0,0};	     
// indicates whether a color to track has been chosen
byte color_chosen = 0; 

// variables needed for the windows where the images are displayed
extern char* capWin;                  
extern char* trackWin;
int w1=0,w2=0,w=0,wo=0,wg=0,n=0;
int main(int argc, char* argv[])
{   float f,h,f1;
    int N, w5=0;
    

    // these not needed when only the camera is used... 
    startOI_MT("/dev/ttyUSB0");
	
    // initialize the camera stream
    initialize_camera();
    
    // this variable will hold the image that we read from the camera
    IplImage* image = cvCreateImage (cvSize (IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 3);
 
    //set up mouse callback function
    cvSetMouseCallback (capWin, mouseCallback, (void *)(&image));
    while (1)
{
      
       // get the latest image 
       get_image (&image);   

       // show the image
       cvShowImage (capWin, image);  
       // this is similar to a "sleep", in this case. Gives time for the window to display the image.
       cvWaitKey (5);
       // find the color we are looking for
	 
	wo=extractColor(image, &targetColor);
	N=160-wo;
	//printf("%d\n",N);
	//f=52*N/320;	
        f=atan(((2.0*N)/160)*tan(26.0/180*M_PI));
	f=f/M_PI*180;
	printf("%f\n",f); 
	if(wo==-1)
	{
	turn(100,1,10,1);
	printf("7\n");
	}
	/*else if(wo==-3)
	{
	driveDistance(100, 0, 80,1);	
	}*/


       else if(f>0)
	{
        printf("2\n");
	turn(70, 1, f, 1);

	}
      else if(f<0)
	{
	printf("3\n");	
	turn(70, -1, f, 1);
	}
	if(wo>157 && wo<167)
	{	
	//driveDistance(100,0,400,1);
	
        //N=160-w2;	
        //f=atan(((2.0*N)/160)*tan(26.0/180*M_PI));
	//h=abs(300/tan(f))+800;
        //printf("%f\n",h);	//driveDistance(100,0,h,1);
	printf("%d\n",n);
	if(n<100000)
	{
	driveDistance(100,0,400,1); 
	}
	wg=extractColorg(image, &targetColor);
	printf("%d %d\n",wg,wo);
	if(wg==-2)
	{
	driveDistance(-70,0,-50,1);
	printf("6\n");
	}
	else if(wg>wo+3)
	{
		turn(70,-1,-60,1);
		driveDistance(100,0,50,1);
		printf("4\n");
	}
	else if(wg<wo-3)
	{
		turn(70,1,60,1);
		driveDistance(100,0,50,1);
		printf("5\n");
	}
	else if(wg>=wo-3 && wg<=wo+3)
	{
		break;
	}
	}
}
	while(1)
	{
		// get the latest image 
       		get_image (&image);   

       		// show the image
       		cvShowImage (capWin, image);  
       		// this is similar to a "sleep", in this case. Gives time for the window to display the image.
       		cvWaitKey (5);
       		// find the color we are looking for
		wg=extractColorg(image, &targetColor);
		printf("%d\n",wg);
		if(wg==-1||wg==-2)
		{
			
			wo=extractColor(image, &targetColor);
			N=160-wo;	
        		f=atan(((2.0*N)/160)*tan(26.0/180*M_PI));
			f=f/M_PI*180;
			printf("%f\n",f); 
			if(wo==-1)
			{
				turn(100,1,10,1);
				
			}


       			else if(f>0)
			{
        			
				turn(70, 1, f, 1);

			}
      			else if(f<0)
			{
				
				turn(70, -1, f, 1);
			}
			if(wo>157 && wo<163)
			{
				printf("9\n");
				driveDistance(100, 0, 300, 1);
				//driveDistance(100, 0, 3000, 1);
				//driveDistance(-100, 0,-20,0);
				break;
			}
			
		}
		else
		{
			printf("10\n");			
			driveDistance(100, 0, 100, 1);
		}
	}
	/*while(1)
	{
	w5=extractColory(image, &targetColor);
			N=160-w5;	
        		f=atan(((2.0*N)/160)*tan(26.0/180*M_PI));
			f=f/M_PI*180;
			printf("%f\n",f); 
			if(w5==-1||w5==-2)
			{
				turn(100,1,10,1);
			}


       			else if(f>0)
			{
        			printf("2\n");
				turn(50, 1, f, 1);

			}
      			else if(f<0)
			{
				printf("3\n");	
				turn(50, -1, f, 1);
			}
			if(w5>155 && w5<165)
			{	
				break;
			}
    
	}
	N=160-w2;	
        f=atan(((2.0*N)/160)*tan(26.0/180*M_PI));
	printf("%f\n",f);
	h=abs(250/tan(f));
        printf("%f\n",h);
	driveDistance(100,0,h,1);
	//driveDistance(100, 0, 3000, 1);
	//driveDistance(-100, 0,-20,0);*/
    // End-of-program bookkeeping  (in fact we never get here, since the above runs in a while(1) loop)
   
    // not needed when only camera is used
     stopOI_MT();

    // close the windows that we use to display images
    cvDestroyAllWindows();
    // stop the camera
    stop_camera();
     
    return 0;
}

 

/**	\brief	Callback function for GUI
* 
* 	Mouse callback function used to allow the GUI to respond to mouse clicks.  In this case, a left
* 	button click will change the target color. This function is not meant to be called directly.
* 
* 	\param	event	mouse event
* 	\param	x		x pos of mouse
* 	\param 	y		y pos of mouse
* 	\param	flags	modifier keys held with mouse button
* 	\param	image_pointer_location	the location of the pointer that contains the current image
*/
void mouseCallback (int event, int x, int y, int flags, void* image_pointer_location)
{

     // get the pointer to the current image
     IplImage* current_image = *((IplImage**) image_pointer_location);

	if (CV_EVENT_LBUTTONDOWN == event)
	{
		// create a new image, identical to the first, but in HSV colorspace
		IplImage* imageHSV = cvCloneImage (current_image);
		cvCvtColor (imageHSV, imageHSV, CV_BGR2HSV);
		
		// get the HSV and RGB values of the pixel we clicked on
		unsigned char* pixel_index = &((unsigned char*)(imageHSV->imageData + imageHSV->widthStep*y))[x*3];
		targetColor.hue 	= pixel_index[0];
		targetColor.saturation 	= pixel_index[1];
		targetColor.value	= pixel_index[2];

		pixel_index = &((unsigned char*)(current_image->imageData + current_image->widthStep*y))[x*3];
		targetColor.blue 	= pixel_index[0];
		targetColor.green 	= pixel_index[1];
		targetColor.red		= pixel_index[2];
		
		printf( " Clicked color: H:%d S:%d V:%d ---  R:%d G:%d B:%d \n",
			targetColor.hue ,targetColor.saturation,targetColor.value,
			targetColor.red, targetColor.green, targetColor.blue);
		
		// release the image to get the memory back
		cvReleaseImage (&imageHSV);
		color_chosen = 1;
	}
	return;
}



/** \brief	Gets new color and finds the pixels that are close to it
* 
* 	This function returns the average X position of the color in the image, which is needed for the
*	Create to move.  The center of the image has an X equal to 0.  Also searches through the given
*	image for the given target color and paints that color into the tracking image.  This allows you
*	to see where in the image the target color is located. 
* 
* 	\param	img		the image to search through
* 	\param	clr		the color to track
* 
* 	\return		the average X position of the given color or NO_COLOR if it is not in the image
*/
double extractColor(IplImage* img, Color* clr)
{
        
	// clone the image, and transform to HSV colorspace
	IplImage* imgHSV = cvCloneImage (img);
	cvCvtColor (imgHSV, imgHSV, CV_BGR2HSV);
	
	// this is the image that will be displayed
	IplImage* track  = cvCreateImage (cvSize (img->width, img->height), IPL_DEPTH_8U, 3);
	
	// these will be used for accessing the image pixels
	unsigned char *img_ptr, *HSVimg_ptr, *out_ptr;
	
        // declare the variables we'll need
	int x, y, current_h, current_s, current_v, current_r, current_g, current_b, hue_tol,val_tol,sat_tol, count=0, s=0, wo=0, N=0,count1=0, s1=0,count2=0, s2=0, m=0;
	double f;

	// tolerances for HSV values of the pixels  
	hue_tol = 5;
	val_tol = 100;
	sat_tol = 90;	//We change the HSV values several times and this is the best one.
	//go through all image pixels, and check if their color matches the selected one
	for (y = 0; y < img->height; y++)
	{
		//move along column through the rows
		for (x = 0; x < img->width; x++)
		{
			//get the HSV values of the currently examined pixel
			HSVimg_ptr = &((unsigned char*)(imgHSV->imageData + imgHSV->widthStep*y))[x*3];
			current_h = HSVimg_ptr[0];
			current_s = HSVimg_ptr[1];
			current_v = HSVimg_ptr[2];
			
			// get the RGB values of the currenty examined pixel 
			img_ptr = &((unsigned char*)(img->imageData + img->widthStep*y))[x*3];
			current_b = img_ptr[0];
			current_g = img_ptr[1];
			current_r = img_ptr[2];
			
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( (abs(current_h-178)<hue_tol  || abs(current_h-178)>180-hue_tol)
				&&   abs(current_s -145)<sat_tol 
				&&   abs(current_v -255)<val_tol )
				
			{
				// if the pixel agrees with the selcted color, set the output pixels to be white
				// the output image is RGB, so white is (255, 255, 255)
				out_ptr[0] = 255; 
				out_ptr[1] = 255; 
				out_ptr[2] = 255; 
				count=count+1;
				s=s+x;	
			}
			else
			{
				out_ptr[0] = 0; 
				out_ptr[1] = 0; 
				out_ptr[2] = 0; 
			}
			
		}
		m=m+count;
	}
	n=m;
	if(count<30)
	{
	   return-1;
	}
	wo=s/count;
	
	
       // show the results of color tracking 
       cvShowImage (trackWin, track);   
       cvWaitKey(5);

       // release images to get back the memory
       cvReleaseImage (&track);     
       cvReleaseImage (&imgHSV);
	
       // this will need to be changed for the 3rd Assignment....
       return (wo);
}double extractColorg(IplImage* img, Color* clr)
{
        
	// clone the image, and transform to HSV colorspace
	IplImage* imgHSV = cvCloneImage (img);
	cvCvtColor (imgHSV, imgHSV, CV_BGR2HSV);
	
	// this is the image that will be displayed
	IplImage* track  = cvCreateImage (cvSize (img->width, img->height), IPL_DEPTH_8U, 3);
	
	// these will be used for accessing the image pixels
	unsigned char *img_ptr, *HSVimg_ptr, *out_ptr;
	
        // declare the variables we'll need
	int x, y, current_h, current_s, current_v, current_r, current_g, current_b, hue_tol,val_tol,sat_tol, count=0, s=0, w=0, N=0,count1=0, s1=0,count2=0, s2=0, wg=0;
	double f;

	// tolerances for HSV values of the pixels  
	hue_tol = 5;
	val_tol = 100;
	sat_tol = 90;	//We change the HSV values several times and this is the best one.
	//go through all image pixels, and check if their color matches the selected one
	for (y = 0; y < img->height; y++)
	{
		//move along column through the rows
		for (x = 0; x < img->width; x++)
		{
			//get the HSV values of the currently examined pixel
			HSVimg_ptr = &((unsigned char*)(imgHSV->imageData + imgHSV->widthStep*y))[x*3];
			current_h = HSVimg_ptr[0];
			current_s = HSVimg_ptr[1];
			current_v = HSVimg_ptr[2];
			
			// get the RGB values of the currenty examined pixel 
			img_ptr = &((unsigned char*)(img->imageData + img->widthStep*y))[x*3];
			current_b = img_ptr[0];
			current_g = img_ptr[1];
			current_r = img_ptr[2];
			
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( (abs(current_h-65)<hue_tol  || abs(current_h-65)>180-hue_tol)
				&&   abs(current_s -94)<sat_tol 
				&&   abs(current_v -239)<val_tol )
				
			{
				// if the pixel agrees with the selcted color, set the output pixels to be white
				// the output image is RGB, so white is (255, 255, 255)
				out_ptr[0] = 255; 
				out_ptr[1] = 255; 
				out_ptr[2] = 255; 
				count=count+1;
				s=s+x;	
			}
			else
			{
				out_ptr[0] = 0; 
				out_ptr[1] = 0; 
				out_ptr[2] = 0; 
			}
			
		}
	}
	if(count<30)
	{
	return -1;
	}
	w=s/count;
	count=0;
	for (y = 0; y < img->height; y++)
	{
		out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[w*3];
		if ( out_ptr[0]==255 )
		{
			count=count+1;
		}
	}
	/*if(count>15)//one cylinder
	{ if(w>wo)
         turn(50,-1,-60,1);
         if(w<wo)
         turn(50,1,60,1);
        }*/
	if(count>15)
	{
	return -2;
        }
for (y = 0; y < img->height; y++)
{
	for(x=0;x<=w;x++)
	{	
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( out_ptr[0]==255 )
				
			{
				 
				count1=count1+1;
				s1=s1+x;	
			}
			
	}
}
w1=s1/count1;

for (y = 0; y < img->height; y++)
{
	for(x=w;x<=320;x++)
	{
	
			
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( out_ptr[0]==255 )
				
			{
				count2=count2+1;
				s2=s2+x;	
			}
			
			
	}
}
w2=s2/count2;

wg=(w1+w2)/2;
//printf("%d %d %d\n",w1,w2,w3);
	
       // show the results of color tracking 
       cvShowImage (trackWin, track);   
       cvWaitKey(5);

       // release images to get back the memory
       cvReleaseImage (&track);     
       cvReleaseImage (&imgHSV);
	
       // this will need to be changed for the 3rd Assignment....
       return (wg);
}double extractColory(IplImage* img, Color* clr)
{         
	// clone the image, and transform to HSV colorspace
	IplImage* imgHSV = cvCloneImage (img);
	cvCvtColor (imgHSV, imgHSV, CV_BGR2HSV);
	
	// this is the image that will be displayed
	IplImage* track  = cvCreateImage (cvSize (img->width, img->height), IPL_DEPTH_8U, 3);
	
	// these will be used for accessing the image pixels
	unsigned char *img_ptr, *HSVimg_ptr, *out_ptr;
	
        // declare the variables we'll need
	int x, y, current_h, current_s, current_v, current_r, current_g, current_b, hue_tol,val_tol,sat_tol, count=0, s=0, w4=0, N=0,count1=0, s1=0,count2=0, s2=0, w3=0;
	double f;

	// tolerances for HSV values of the pixels  
	hue_tol = 5;
	val_tol = 100;
	sat_tol = 90;	//We change the HSV values several times and this is the best one.
	//go through all image pixels, and check if their color matches the selected one
	for (y = 0; y < img->height; y++)
	{
		//move along column through the rows
		for (x = 0; x < img->width; x++)
		{
			//get the HSV values of the currently examined pixel
			HSVimg_ptr = &((unsigned char*)(imgHSV->imageData + imgHSV->widthStep*y))[x*3];
			current_h = HSVimg_ptr[0];
			current_s = HSVimg_ptr[1];
			current_v = HSVimg_ptr[2];
			
			// get the RGB values of the currenty examined pixel 
			img_ptr = &((unsigned char*)(img->imageData + img->widthStep*y))[x*3];
			current_b = img_ptr[0];
			current_g = img_ptr[1];
			current_r = img_ptr[2];
			
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( (abs(current_h-27)<hue_tol  || abs(current_h-27)>180-hue_tol)
				&&   abs(current_s -92)<sat_tol 
				&&   abs(current_v -206)<val_tol )
				
			{
				// if the pixel agrees with the selcted color, set the output pixels to be white
				// the output image is RGB, so white is (255, 255, 255)
				out_ptr[0] = 255; 
				out_ptr[1] = 255; 
				out_ptr[2] = 255; 
				count=count+1;
				s=s+x;	
			}
			else
			{
				out_ptr[0] = 0; 
				out_ptr[1] = 0; 
				out_ptr[2] = 0; 
			}
			
		}
	}
	if(count<50)
	{
	return -1;
	}
	w4=s/count;
	count=0;
	for (y = 0; y < img->height; y++)
	{
		out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[w4*3];
		if ( out_ptr[0]==255 )
		{
			count=count+1;
		}
	}
	if(count>30)
	{
         return -2;
        }
        
for (y = 0; y < img->height; y++)
{
	for(x=0;x<=w;x++)
	{	
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( out_ptr[0]==255 )
				
			{
				 
				count1=count1+1;
				s1=s1+x;	
			}
			
	}
}
w1=s1/count1;

for (y = 0; y < img->height; y++)
{
	for(x=w4;x<=320;x++)
	{
	
			
			// pointer to the (x,y) pixel in the output image (track)
			out_ptr = &((unsigned char*)(track->imageData + track->widthStep*y))[x*3];
			
			// compare the pixel value vs. the selected color values
			if ( out_ptr[0]==255 )
				
			{
				count2=count2+1;
				s2=s2+x;	
			}
			
			
	}
}
w2=s2/count2;

w3=(w1+w2)/2;
printf("%d %d %d\n",w1,w2,w3);
	
       // show the results of color tracking 
       cvShowImage (trackWin, track);   
       cvWaitKey(5);

       // release images to get back the memory
       cvReleaseImage (&track);     
       cvReleaseImage (&imgHSV);

       return (w3);
}

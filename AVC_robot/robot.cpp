
#include "robot.hpp"

#define WHITE_THRESHOLD 240
#define CENTRE_OFFSET 15 // allow some offset, as white line takes up a fair amount of pixels
#define LINE_WIDTH 4 // from looking at some test output, it looks like the line takes up 4 pixels

/* IDEA: if the pixel is not a straight line, we use the pixel with the maximum
*  offset from the centre of the image (i.e. where line should be) to determine
* the strength of its motors

* NB: Function returns different values indicating which direction the robot should move
*/
int analyse_image(ImagePPM image) {
	int robot_centre_view = image.width/2; // we want the line to fall within a range of the robots centre view
	// testing statements
	printf("CENTRE OF ROBOTS VIEW: %i\n", image.width/2 );
	//count total number of white pixels considered to be 'straight' (i.e. within center vision)
	int num_straight_pixels = 0;
	// iterate through all the rows and columns in the image, get each pixel
	for (int row = 0; row < image.height; row++) {
		for (int column = 0; column < image.width; column++) {
			// get each colour component for the given pixel, to evaluate whether it is white
			int red = get_pixel(image, row, column, 0);
			int green = get_pixel(image, row, column, 1);
			int blue = get_pixel(image, row, column, 2);
			int luminocity = get_pixel(image, row, column, 3);
			if (red >= WHITE_THRESHOLD && green >= WHITE_THRESHOLD && blue >= WHITE_THRESHOLD && luminocity >= WHITE_THRESHOLD) {
				// have found white pixel, note: white threshold value is just a test, may need to change
				if (column >= (robot_centre_view - CENTRE_OFFSET) && column <= (robot_centre_view + CENTRE_OFFSET)) {
					// we have found a centre pixel
					num_straight_pixels++;
				}
			}
		}
	}
	// should add offset (i.e. range), as even if the line is not 100% straight, the robot should still move straight until
	// the line is considerably 'non-straight'
	printf("num straight pixels: %i\n",num_straight_pixels);
	if ((num_straight_pixels/4) > image.height-20) {
		printf("robot should move straight\n");
		return 0; // indicates robot should move straight
	}
	else { // to be implemented, if the offset indicates left (i.e. closer to 0, move left, closer to image.width, move right)
		printf("robot should do something else\n");
		return 1; 
	}
	return -1; // indicates error
}

int main(){
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
    double vLeft = 0.0;
    double vRight = 0.0;

    while(1){
			ImagePPM image;
			takePicture();
	    SavePPMFile("i0.ppm",cameraView);
			OpenPPMFile("i0.ppm", image);
			int direction = analyse_image(image);
			if (!direction) { // if direction == 0, move straight
					vLeft= 20.0;
					vRight = 20.0;
			}
			else { // spin until we can move straight!
				vLeft = 0.0;
				vRight = 10.0;
			}
      setMotors(vLeft,vRight);
      std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
       usleep(10000);
  } //while

} // main

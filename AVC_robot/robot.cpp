
#include "robot.hpp"

#define WHITE_THRESHOLD 240

int analyse_image(ImagePPM image) {
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
			}
		}
	}
	return 0;
}

int main(){
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
    double vLeft = 40.0;
    double vRight = 30.0;

    while(1){
			ImagePPM image;
			takePicture();
	    SavePPMFile("i0.ppm",cameraView);
			OpenPPMFile("i0.ppm", image);
			int colour = analyse_image(image);
      setMotors(vLeft,vRight);
      std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
       usleep(10000);
			 break;
  } //while

} // main


#include "robot.hpp"

#define WHITE_THRESHOLD 240 // threshold for something being 'white'
#define CENTRE_OFFSET 10 // allow some offset, accounting for scenarios where we may not make the right analysis of pixel pos
#define LINE_WIDTH 4 // it appears that the line takes up 4 pixels (using output to make that assumption)
/**
 * Analyse image, and determine whether there is enough 'straight' pixels within the robot's centre
 * view. Image is only analysed from the last x amount of rows (currently image.height/2 + 20), to ensure
 * robot does not make a movement decision based on 'far away' pixel (will result in incorrect movement).
 *
 * If the pixel is not considered to be straight (in robot's centre view), record the pixel with the max_centre_offset
 * from the centre view. Record its column index too, so we can determine whether robot should move left/right.
 *
 * IMPORTANT: if you change the 'ratio' of pixels analysed (which is currently 5*(image.height)/6) you must
 *						also change the ratio of how many 'straight' pixels there must be for the robot to move straight
 *
 * @param ImagePPM image (current screen shot of the robots view)
 * @return 0 for straight move, 1 for left move, 2 for right move, -1 for failure (never condition)
 */
int analyse_image(ImagePPM image) {
	int robot_centre_view = image.width/2; // we want the line to fall within a range of the robots centre view
	// testing statements
	printf("CENTRE OF ROBOTS VIEW: %i\n", image.width/2);
	// var setting
	int num_straight_pixels = 0; // total num straight pixels (recorded throughout offset)
	int max_centre_offset = 0; // current max pixel offset from centre
	int col_of_offset = -1; // column of pixel that currently has largest offset from centre
	int start_row = ((5*(image.height))/6)+10; // row that the analysis should start from
	// iterate through all the rows and columns in the image, get each pixel
	for (int row = start_row; row < image.height; row++) {
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
				else {
					int pixel_centre_offset = robot_centre_view - column; // calculate offset from CENTRE
					if (pixel_centre_offset < 0 ) { pixel_centre_offset *= -1; } // getting abs. values (non neg)
					// if current pixel has a higher offset than current max (from centre), set it as new max offset
					if (pixel_centre_offset > max_centre_offset) {
						max_centre_offset = pixel_centre_offset;
						col_of_offset = column;
					}
				}
			}
		}
	}
	// test stmts
	printf("num straight pixels: %i\n", num_straight_pixels/LINE_WIDTH);
	printf("num rows analysed: %i\n", image.height-start_row);
	// for analysis to consider line straight, all pixels straight ahead must be within centre vision
	if (((num_straight_pixels/LINE_WIDTH)) >= (image.height-start_row)) {
		printf("robot should move straight\n");
		return 0; // indicates robot should move straight
	}
	// if the column of offset is larger than the robots centre view, robot should turn left
	else if (col_of_offset != -1 && col_of_offset > robot_centre_view){
		printf("robot should turn left\n");
		return 1;
	}
	// if the column of offset is smaller than the robots centre view, robot should turn right
	else if (col_of_offset < robot_centre_view) {
		printf("robot should turn right\n");
		return 2;
	}
	printf("REACHING ERROR STATE - SHOULD NEVER REACH THIS PART OF CODE\n");
	return -1; // indicates error
}

int main(){
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
<<<<<<<    double vLeft = 40.0;
    double vRight = 30.0;
    takePicture();
    SavePPMFile("i0.ppm",cameraView);
    while(1){
      setMotors(vLeft,vRight);
      std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
       usleep(10000);
  } //while

} // main

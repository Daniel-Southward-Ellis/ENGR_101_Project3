
#include "robot.hpp"

#define WHITE_THRESHOLD 250 // threshold for something being 'white'
#define CENTRE_OFFSET 2 // allow some offset, accounting for scenarios where we may not make the right analysis of pixel pos
#define LINE_WIDTH 4 // it appears that the line takes up 4 pixels (using output to make that assumption)

enum Direction { forward, left, right, invalid };

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
Direction analyse_image(ImagePPM image) {
	int robot_centre_view = (image.width/2); // we want the line to fall within a range of the robots centre view
	// testing statements
	printf("CENTRE OF ROBOTS VIEW: %i\n", image.width/2);
	// var setting
	int num_white_pixels = 0; // total num white pixels
	int avg_offset = 0; // average white pixel offset from centre
	int start_row = image.height - 10; // row that the analysis should start from
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
				avg_offset += column - robot_centre_view;
				++num_white_pixels;
			}
		}
	}
	if (num_white_pixels > 0){
		avg_offset = avg_offset / num_white_pixels;
	}
	// test stmts
	printf("average pixel offset: %i\n", avg_offset);
	printf("num rows analysed: %i\n", image.height-start_row);
	// for analysis to consider line straight, all pixels straight ahead must be within centre vision
	if (avg_offset >= -CENTRE_OFFSET && avg_offset <= CENTRE_OFFSET) {
		printf("robot should move straight\n");
		return forward; // indicates robot should move straight
	}
	// if the average offset is larger than the robots centre view, robot should turn left
	else if (avg_offset < -CENTRE_OFFSET) {
		printf("robot should turn left\n");
		return left;
	}
	// if the average offset is smaller than the robots centre view, robot should turn right
	else if (avg_offset > CENTRE_OFFSET) {
		printf("robot should turn right\n");
		return right;
	}
	printf("REACHING ERROR STATE - SHOULD NEVER REACH THIS PART OF CODE\n");
	return invalid; // indicates error
}

int main(){
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
    double vLeft = 0.0;
    double vRight = 0.0;

		int rotation_count_left = 0; // count num rotations to determine direction of robot
		int rotation_count_right = 0; // NB: haven't done anything with these rotation counts, my idea
		//																	was to detect if the robot did a 180, and correct it accordingly
    while(1){
			takePicture();
	    	SavePPMFile("i0.ppm",cameraView);
			Direction direction = analyse_image(cameraView);
			if (direction == forward) { // if direction == 0, move straight
					rotation_count_left = 0;
					rotation_count_right = 0;
					vLeft= 10.0;
					vRight = 10.0;
			}
			else if (direction == left) { // spin left until we can move straight!
				rotation_count_left = 0;
				rotation_count_right++;
				vLeft = 0.0;
				vRight = 10.0;
			}
			else if (direction == right) {
				rotation_count_right = 0;
				rotation_count_left++;
				vLeft = 10.0;
				vRight = 0.0;
			}
      setMotors(vLeft,vRight);
      std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
       usleep(10000);
  } //while

} // main

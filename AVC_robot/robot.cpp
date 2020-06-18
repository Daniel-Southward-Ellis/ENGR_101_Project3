#include "robot.hpp"

#define WHITE_THRESHOLD 250 // threshold for something being 'white'
#define BLACK_THRESHOLD 5 // threshold for something being 'black'
#define CENTRE_OFFSET 2 // allow some offset, accounting for scenarios where we may not make the right analysis of pixel pos
#define LINE_WIDTH 4 // it appears that the line takes up 4 pixels (using output to make that assumption)
#define STRAIGHT_MOVES_SHARP_TURN 25 // best value for num straight moves before turning; discovered in testing
#define STRAIGHT_MOVES_TURN 4
#define STRAIGHT_MOVES_IGNORE_TURN 10
#define STRAIGHT_MOVES_HALF_ROTATION 40
#define REDNESS_FRACTION 1.5
#define WALL_SIZE 25
#define FLAG_THRESHOLD 20

enum Direction { forward, left, hard_left, right, hard_right, stop, turn_around, invalid};

bool hasTurnedAround = false; // global
int offWallGracePeriod = 0; // Allows for a short grace period when following walls before turning around

/**
* Move straight before turning sharply, as robot has a delay between what it sees,
* and when it should move
*/
void movesBeforeTurn(int num_turns) {
	for (int i = 0; i < num_turns; i++) {
		setMotors(10.0, 10.0);
		usleep(10000);
	}
}

// Method used for challenge, hugs the left wall to make it's way through a maze
Direction analyseRedPixels(ImagePPM image) {
	offWallGracePeriod = 10;
	int num_red_pixels = 0; // total red pixels
	int left_wall_pixels = 0; // red pixels along the left side
	int right_wall_pixels = 0; // red pixels along the right side
	int front_pixels = 0; // red pixels at the front but not the left side

	for (int row = 0; row < image.height; row++) {
		for (int column = 0; column < image.width; column++) {
			int red = get_pixel(image, row, column, 0);
			int green = get_pixel(image, row, column, 1);
			int blue = get_pixel(image, row, column, 2);
			double red_fraction = red / REDNESS_FRACTION;
			if (green < red_fraction && blue < red_fraction) {
				++num_red_pixels;
				if (column < WALL_SIZE) {
					++left_wall_pixels;
				}else if (column > image.width - WALL_SIZE) {
					++right_wall_pixels;
				}
				
				if (row < WALL_SIZE && column >= WALL_SIZE && column <= image.width - WALL_SIZE) {
					++front_pixels;
				}
			}
		}
	}

	//Try to hug the left hand wall
	if (left_wall_pixels > 0 && front_pixels == 0) {
		return forward;
	}else if (left_wall_pixels == 0) {
		return left;
	}else if (front_pixels > 0) {
		return right;
	}

	//Lets try and find something to follow
	return right;
}

// determine if there are any black pixels in the image, if so, stop the robot
Direction analyseBlackPixels(ImagePPM image) {
	int num_black_pixels = 0; // total black pixels

	for (int row = 0; row < image.height; row++) {
		for (int column = 0; column < image.width; column++) {
			int red = get_pixel(image, row, column, 0);
			int green = get_pixel(image, row, column, 1);
			int blue = get_pixel(image, row, column, 2);
			if (red <= BLACK_THRESHOLD && green <= BLACK_THRESHOLD && blue <= BLACK_THRESHOLD) {
				++num_black_pixels;
			}
		}
	}
	// if the number of black pixels exceeds the threshold, stop. We've (hopefully) found the flag
	if (num_black_pixels > FLAG_THRESHOLD && offWallGracePeriod == 0) {
		return stop;
	}
	// if black pixels don't exceed the threshold and no grace period left - turn around
	if (offWallGracePeriod == 0) {
		return turn_around;
	}else{
		//Otherwise go looking for the wall we want to hug
		return right;
	}
}

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
Direction analyse_image(ImagePPM image, Direction recent_move) {
	int robot_centre_view = (image.width/2); // we want the line to fall within a range of the robots centre view
	bool accelerate = false;
	// testing statements
	printf("CENTRE OF ROBOTS VIEW: %i\n", image.width/2);
	// var setting
	int num_white_pixels = 0; // total white pixels
	int num_red_pixels = 0; // total red pixels
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
			}else{
				double red_fraction = red / REDNESS_FRACTION;
				if (green < red_fraction && blue < red_fraction) {
					++num_red_pixels;
				}
			}
		}
	}
	if (num_white_pixels > 0){
		avg_offset = avg_offset / num_white_pixels;
	}
	// test stmts
	std::cout<<"has turned around: "<<hasTurnedAround<<std::endl;
	std::cout<<"direction of RM: "<<recent_move<<std::endl;
	printf("num white pixels: %i\n", num_white_pixels);
	printf("average pixel offset: %i\n", avg_offset);
	printf("num rows analysed: %i\n", image.height-start_row);

	// SPECIAL MOVES - NO WHITE PIXELS
	if (num_white_pixels == 0) { // no white pixels
		if (num_red_pixels == 0) {
			return analyseBlackPixels(image); // will return turn_around, or stop
		} else {
			return analyseRedPixels(image); // will follow the wall
		}
	}

	// LINE ANALYSIS - we know there is at least 1 white pixel.

	// for analysis to consider line straight, all pixels straight ahead must be within centre vision
	if (avg_offset >= -CENTRE_OFFSET && avg_offset <= CENTRE_OFFSET) {
		return forward; // indicates robot should move straight
	}
	// if the average offset is larger than the robots centre view, robot should turn left
	if (avg_offset < -CENTRE_OFFSET) {
		if (num_white_pixels >= LINE_WIDTH*(image.width/5) && (!hasTurnedAround || (hasTurnedAround && recent_move != hard_right))) {
			return hard_left;
		}
		else if (num_white_pixels < LINE_WIDTH*(image.width/5)) {
			return left;
		}
		// if we have encounted another hard turn of same type - now erase this memory
		if (num_white_pixels >= LINE_WIDTH*(image.width/5) && hasTurnedAround && recent_move == hard_right) {
			hasTurnedAround = false;
			accelerate = true;
		}
	}
	// if the average offset is smaller than the robots centre view, robot should turn right
	if (avg_offset > CENTRE_OFFSET) {
		if (num_white_pixels >= LINE_WIDTH*(image.width/5) && (!hasTurnedAround || (hasTurnedAround && recent_move != hard_left))) {
			return hard_right;
		}
		else if (num_white_pixels < LINE_WIDTH*(image.width/5)) {
			return right;
		}
		// if we have encounted another hard turn of same type - now erase this memory
		if (num_white_pixels >= LINE_WIDTH*(image.width/5) && hasTurnedAround && recent_move == hard_left) {
			hasTurnedAround = false;
			accelerate = true; // must accelerate to get robot to ignore firstmost identical turn after turning around
		}
	}

	if (accelerate) {
		movesBeforeTurn(STRAIGHT_MOVES_IGNORE_TURN);
	}

	printf("REACHING ERROR STATE - SHOULD NEVER REACH THIS PART OF CODE\n");
	return invalid; // indicates error
}

void turnAround() {
	movesBeforeTurn(STRAIGHT_MOVES_HALF_ROTATION);
	for (int i = 0; i < 4; i++) {
		setMotors(180.0, 0);
		usleep(10000);
	}
}

int main(){
	if (initClientRobot() !=0){
		std::cout<<" Error initializing robot"<<std::endl;
	}
    double vLeft = 0.0;
    double vRight = 0.0;
	bool take_pic = false;
	Direction mostRecentMove = forward;
    while(1){
			takePicture();
			SavePPMFile("i0.ppm",cameraView);
			if (take_pic) {
				SavePPMFile("lily.ppm",cameraView);
			}
			if (offWallGracePeriod > 0) {
				--offWallGracePeriod;
			}
			Direction direction = analyse_image(cameraView, mostRecentMove);
			switch (direction) {
				case forward:
					printf("MOVING FORWARD\n");
					vLeft= 15.0;
					vRight = 15.0;
					break;
				case left:
					printf("MOVING LEFT\n");
					movesBeforeTurn(STRAIGHT_MOVES_TURN);
					vLeft = 0.0;
					vRight = 10.0;
					break;
				case right:
					printf("MOVING RIGHT\n");
					movesBeforeTurn(STRAIGHT_MOVES_TURN);
					vLeft = 10.0;
					vRight = 0.0;
					break;
				case hard_right:
					printf("MOVING HARD LEFT\n");
					movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					vLeft = 160.0;
					vRight = 0.0;
					mostRecentMove = hard_right;
					break;
				case hard_left:
					printf("MOVING HARD RIGHT\n");
					movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					vLeft = 0.0;
					vRight = 160.0;
					mostRecentMove = hard_left;
					break;
				case stop:
					printf("STOPPING - FOUND FLAG");
					vLeft = 0.0;
					vRight = 0.0;
					break;
				case turn_around:
					printf("DOING a 180 - no line in sight");
					turnAround();
					vLeft = 0.0;
					vRight = 0.0;
					hasTurnedAround = true;
				default:
					printf("default case\n");
					vLeft = 15.0;
					vRight = 15.0;
					break;
			}
      setMotors(vLeft,vRight);
      std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
      usleep(10000);
  } //while

} // main

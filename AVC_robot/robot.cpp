#include "robot.hpp"

#define WHITE_THRESHOLD 250 // threshold for something being 'white'
#define BLACK_THRESHOLD 5 // threshold for something being 'black'
#define CENTRE_OFFSET 2 // allow some offset, accounting for scenarios where we may not make the right analysis of pixel pos
#define LINE_WIDTH 4 // it appears that the line takes up 4 pixels (using output to make that assumption)
#define STRAIGHT_MOVES_SHARP_TURN 25 // best value for num straight moves before turning; discovered in testing
#define STRAIGHT_MOVES_TURN 4
#define STRAIGHT_MOVES_IGNORE_TURN 10
#define STRAIGHT_MOVES_HALF_ROTATION 40
#define RED_RATIO 0.3
#define COLUMN_WIDTH 14
#define CORIDOR_ROW_OFFSET 4


enum Direction { forward, left, hard_left, maze_left, emergency_left, right, hard_right, maze_right, emergency_right, stop, turn_around, invalid};
enum Mode {white, red};
enum Wall {left_wall, right_wall};

bool hasTurnedAround = false; // global

Mode movement_type = white;

Wall current_wall = left_wall;

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

/**
* Analyse red pixels when robot leaves the white line; if no red pixels, then turn around
*/
Direction analyseRedPixels(ImagePPM image) {
	int num_red_pixels = 0;
	int avg_pixel_col = 0;
	int avg_pixel_row = 0;
	int num_pixels_top = 0; // num red pixels top of img (i.e. row == 0)
	int num_pixels_bottom = 0; // num red pixels bottom of image (i.e. imgheight -1)
	int num_black_pixels = 0;

	int centre_row = image.height / 2;
	for (int row = 0; row < image.height; row++) {
		for (int column = 0; column < image.width; column++) {
			int red = get_pixel(image, row, column, 0);
			int green = get_pixel(image, row, column, 1);
			int blue = get_pixel(image, row, column, 2);
			int minRedRatio = red * RED_RATIO; // maximum value a red or green pixel may be for the pixel to be red
			// if red, perform analysis - record red pixel positions
			if (green <= minRedRatio && blue <= minRedRatio) {
				++num_red_pixels;
				avg_pixel_col += column;
				avg_pixel_row += row;
				if (row == 0) {
					num_pixels_top++;
				}
				if (row == image.height-COLUMN_WIDTH-10) {
					num_pixels_bottom++;
				}
			}
			// checking black pixels in bottom row, if so - stop robot
			else if ((red <= BLACK_THRESHOLD && green <= BLACK_THRESHOLD && blue <= BLACK_THRESHOLD) && row == image.height-1) {
				++num_black_pixels;
			}
		}
	}

	// DOCUMENTATION OF TESTING STATEMENTS
	// printf("tot red pixels: %i\n", num_red_pixels);
	// printf("num pixels top %i\n", num_pixels_top);
	// printf("num pixels bottom %i\n", num_pixels_bottom);

	if (num_black_pixels > 0) {
		return stop; // any black pixels stop immediately
	}

	if (num_red_pixels > 0) {
		// only set if not being divided by 0 (otherwise float error)
		avg_pixel_col = avg_pixel_col/num_red_pixels;
		avg_pixel_row = avg_pixel_row/num_red_pixels;
		// DOCUMENTATION OF TESTING STATEMENTS
		// printf("av red pixel col: %i\n", avg_pixel_col);
		// printf("av red pixel row: %i\n", avg_pixel_row);
		// set type of robot movement to red (i.e. following maze not white line)
		movement_type = red;
	}

	// if there is col either side of robot, go forward (record current wall for ref for blind turns)
	if (num_red_pixels > 0 && avg_pixel_row >= centre_row-CORIDOR_ROW_OFFSET && avg_pixel_row <= centre_row+CORIDOR_ROW_OFFSET) {
		if (avg_pixel_col > (image.width/2)) {
			current_wall = right_wall;
		}
		else {
			current_wall = left_wall;
		}
		return forward;
	}
	// if col on left side and top of maze in front (maze wall), turn a sharp right
	if (num_red_pixels > 0 && avg_pixel_col < (image.width/2) && avg_pixel_row  && num_pixels_top > (2*image.width)/3) {
		movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
		return maze_right;
	}
	// if col on right size and maze wall in front, turn a sharp left
	if (num_red_pixels > 0 && avg_pixel_col > (image.width/2) && num_pixels_top > (2*image.width)/3) {
		movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
		return maze_left;
	}
	// either no pixels in sight, or maze wall ahead with no maze wall on left or right; if prev wall was left, turn left
	if ((num_pixels_top > (2*image.width)/3 || (num_red_pixels == 0 && movement_type == red)) && current_wall == left_wall) {
		movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
		return maze_left;
	}
	// either no pixels in sight, or maze wall ahead with no maze wall on left or right; if prev wall was right, turn right
	if ((num_pixels_top > (2*image.width)/3 || (num_red_pixels == 0 && movement_type == red)) && current_wall == right_wall
	) {
		movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
		return maze_right;
	}
	// maze wall STRAIGHT AHEAD do a left sharp turn NOW (avg pixel pos)
	if ((num_pixels_bottom > (2*image.width)/3) && avg_pixel_col > (image.width/2)) {
		return emergency_left; // emergency turn!
	}
	// maze wall STRAIGHT AHEAD do a right sharp turn NOW (avg pixel pos)
	if ((num_pixels_bottom > (2*image.width)/3) && avg_pixel_col < (image.width/2)) {
		return emergency_right; // emergency turn!
	}
	// default move forward if movement type is red; accounts for situations where there is NO walls anywhere
	if (movement_type == red) {
		if (avg_pixel_col > (image.width/2) && num_red_pixels > 0) {
			current_wall = right_wall;
		}
		else if (num_red_pixels > 0){
			current_wall = left_wall;
		}
		return forward;
	}

	return turn_around; // must be end of white line, no black AND no red corridors
}

/**
	* determine if there are any black pixels in the image, if so, stop the robot (white line mode only)
	* No black? Then check for red corridors (fn. is called inside this fn.) to account for when robot should switch from
	* white line mode to red.
* */
Direction analyseBlackPixels(ImagePPM image) {
	int num_black_pixels = 0; // total black pixels
	// analyse image for BLACK pixels
	for (int row = 0; row < image.height; row++) {
		// don't analyse if maze, as this will result in robot stopping too short of the flag
		if (movement_type == red) {
			break;
		}
		for (int column = 0; column < image.width; column++) {
			int red = get_pixel(image, row, column, 0);
			int green = get_pixel(image, row, column, 1);
			int blue = get_pixel(image, row, column, 2);
			// if black threshold is met for each R, G, B component, increment black pixels
			if (red <= BLACK_THRESHOLD && green <= BLACK_THRESHOLD && blue <= BLACK_THRESHOLD) {
				++num_black_pixels;
			}
		}
	}
	// if there are any black pixels - stop, it is the flag (working under assumption our robot will NEVER encounter screen edge)
	if (num_black_pixels > 0) {
		return stop;
	}
	// if no black pixels in entire image - must be end of line, turn around (completion)
	return analyseRedPixels(image);
}

/**
 * Analyse image, and determine whether there is enough 'straight' pixels within the robot's centre
 * view. Image is only analysed from the last x amount of rows, to ensure
 * robot does not make a movement decision based on 'far away' pixel (will result in incorrect movement).
 *
 * If the pixel is not considered to be straight (in robot's centre view), record the pixel with the max_centre_offset
 * from the centre view. Record its column index too, so we can determine whether robot should move left/right.
 *
 * Will call black pixel analysis if no white line in sight -> checking for BLACK flag
 * Black pixel analysis fn. (ref above) will call red pixel analysis if no black pixel in sight
 *
 * returns enum Direction - ref above enum.
 */
Direction analyse_image(ImagePPM image, Direction recent_move) {
	int robot_centre_view = (image.width/2); // we want the line to fall within a range of the robots centre view
	bool accelerate = false;
	// testing statements
	// printf("CENTRE OF ROBOTS VIEW: %i\n", image.width/2);
	// var setting
	int num_white_pixels = 0; // total white pixels
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
	// DOCUMENTATION OF TESTING STATEMENTS
	// std::cout<<"has turned around: "<<hasTurnedAround<<std::endl;
	// std::cout<<"direction of RM: "<<recent_move<<std::endl;
	// printf("num white pixels: %i\n", num_white_pixels);
	// printf("average pixel offset: %i\n", avg_offset);
	// printf("num rows analysed: %i\n", image.height-start_row);

	if (num_white_pixels > 0) {
		movement_type = white;
	}
	// SPECIAL MOVES - NO WHITE PIXELS

	if (num_white_pixels == 0) { // no black pixels, no white pixels (must have reached end of line)
		return analyseBlackPixels(image); // will return turn_around, or stop
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
		// if we have encounted another hard turn of same type - now erase memory of prev turn so next turn of same type will not be avoided
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

	// accelerate robot to IGNORE/AVOID turn (only a case for when robot has turned around and is trying to take same path)
	if (accelerate) {
		movesBeforeTurn(STRAIGHT_MOVES_IGNORE_TURN);
	}

	return invalid; // indicates error
}

/**
* Turn this robot around 180! It has encountered the end of a line
*/
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
		// INIT VARS
    double vLeft = 0.0;
    double vRight = 0.0;
		bool isStopped = false;
		Direction mostRecentMove = forward;

    while(1){
			takePicture();
	    SavePPMFile("i0.ppm",cameraView);
			Direction direction = analyse_image(cameraView, mostRecentMove);
			// SWITCH STATMENT FOR DIFFERENT DIRECTION TYPES -> DO DIFFERENT MOVEMENTS
			switch (direction) {
				case forward:
					// printf("MOVING FORWARD\n");
					vLeft= 15.0;
					vRight = 15.0;
					break;
				case left:
					// printf("MOVING LEFT\n");
					movesBeforeTurn(STRAIGHT_MOVES_TURN);
					vLeft = 0.0;
					vRight = 10.0;
					break;
				case right:
					// printf("MOVING RIGHT\n");
					movesBeforeTurn(STRAIGHT_MOVES_TURN);
					vLeft = 10.0;
					vRight = 0.0;
					break;
				case hard_right:
					// printf("MOVING HARD RIGHT\n");
					movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					vLeft = 165.0;
					vRight = 0.0;
					mostRecentMove = hard_right;
					break;
				case hard_left:
					// printf("MOVING HARD LEFT\n");
					movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					vLeft = 0.0;
					vRight = 165.0;
					mostRecentMove = hard_left;
					break;
				case stop:
					// printf("STOPPING - FOUND FLAG");
					if (!isStopped) {
						movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					}
					vLeft = 0.0;
					vRight = 0.0;
					isStopped = true;
					break;
				case turn_around:
					// printf("DOING a 180 - no line in sight");
					turnAround();
					vLeft = 0.0;
					vRight = 0.0;
					hasTurnedAround = true;
				case maze_left:
					// printf("MAZE LEFT\n");
					movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					vLeft = 0.0;
					vRight = 169.0;
					break;
				case maze_right:
					// printf("MAZE RIGHT\n");
					movesBeforeTurn(STRAIGHT_MOVES_SHARP_TURN);
					vLeft = 169.0;
					vRight = 0.0;
					break;
				case emergency_left:
					// printf("EMERGENCY LEFT\n\n");
					vLeft = 0.0;
					vRight = 169.0;
					break;
				case emergency_right:
					// printf("EMERGENCY RIGHT\n\n");
					vLeft = 169.0;
					vRight = 0.0;
					break;
				default:
					// printf("default case\n");
					vLeft = 15.0;
					vRight = 15.0;
					break;
			}
      setMotors(vLeft,vRight);
      std::cout<<" vLeft="<<vLeft<<"  vRight="<<vRight<<std::endl;
      usleep(10000);
  } //while

} // main

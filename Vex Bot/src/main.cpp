#include "main.h"
using namespace pros;
// Motor & Ports
// Fork
Motor fork_left(14);
Motor fork_right(4);
Motor lock(11);
Motor lock_lift(12);
// Movement
Motor move_right_top(3);
Motor move_right_bottom(1);
Motor move_left_bottom(15);
Motor move_left_top(13);
ADIDigitalIn sensor(8);
ADIDigitalIn sensor2(2);

// Variables
int control_dir = 0;
int pos_up = 0;
int pos_down = 0;
int move = 0;
int forward_move = 0;
int backward_move = 0;
int pos_2 = 0;
int lock_pos = 0;
int countdown = 3000;

int left = 0;
int right = 0;
int org1 = 0;
int org2 = 0;
int org3 = 0;
int org4 = 0;

int move_right_top_lock = 0;
int move_right_bottom_lock = 0;
int move_left_bottom_lock = 0;
int move_left_top_lock = 0;
// Controller
Controller master(E_CONTROLLER_MASTER);

// Initialize Event
void initialize() {
	lcd::initialize();
	delay(100);
}
/** Other Events
void disabled() {}
**/
void competition_initialize() {
	lcd::initialize();
	delay(1);
	lcd::print(1, "  @@@@@    @@@      @@@    @@@");
	delay(1);
	lcd::print(2," @@       @   @    @  @   @   ");
	delay(1);
	lcd::print(3," @@@@@   @     @  @@@@@  @");
	delay(1);
	lcd::print(4," @@   @   @   @       @   @   ");
	delay(1);
	lcd::print(5,"  @@@@     @@@        @    @@@");
	delay(1);
	lcd::print(6,"   Alan, Eric, Nick (L EZ BAD)");
	delay(1);
	lcd::print(7,"        Competition Mode");
	delay(1);
}

void lowerlift() {
	while (sensor.get_value() == false) {
		fork_right = 127 / 2;
		fork_left = -127 / 2;
		pos_up = fork_right.get_position();
		pos_down = fork_left.get_position();
	}
}

void update_pos() {
	left = move_right_top.get_position();
	right = move_left_top.get_position();
}

void moveforward() {
	move_right_bottom = -40;
	move_right_top = -40;
	move_left_bottom = 40;
	move_left_top = 40;
}

void go_to_pos_wo(int leftpos = 0, int rightpos = 0, int lift = 0) {
	double errorRT = rightpos - move_right_top.get_position();
	double errorLT = rightpos - move_left_top.get_position();

	if (leftpos != 0 && rightpos != 0) {
		move_right_bottom.tare_position();
		move_right_top.tare_position();
		move_left_bottom.tare_position();
		move_left_top.tare_position();

		while (fabs(errorRT) > 5 || fabs(errorRT) < -5 && fabs(errorLT) > 5 || fabs(errorLT) < -5) {

			move_right_bottom = rightpos - move_right_bottom.get_position();
			move_right_top = rightpos - move_right_top.get_position();
			move_left_bottom = leftpos - move_left_bottom.get_position();
			move_left_top = leftpos - move_left_top.get_position();

			errorRT = rightpos - move_right_top.get_position();
			errorLT =  rightpos - move_left_top.get_position();
		}
		move_right_bottom = 0;
		move_right_top = 0;
		move_left_bottom = 0;
		move_left_top = 0;
	}
}

void turn(int degrees = 0) {
	go_to_pos_wo(degrees * 9, degrees * 9);
}
void movefb(int steps = 0, int lift = 0) {
	go_to_pos_wo(steps * 9, steps * -9);
}
void autonomous() {
	countdown = 800;
	while (countdown != 0) {
		move_right_bottom = 100;
		move_right_top = 100;
		move_left_bottom = -100;
		move_left_top = -100;
		countdown -= 1;
	}
	lock = -127;
	delay(1000);
	lock = 0;
	countdown = 200;
	while (countdown != 0) {
		move_right_bottom = -100;
		move_right_top = -100;
		move_left_bottom = 100;
		move_left_top = 100;
		countdown -= 1;
	}
	lock = 127;
	delay(700);
	lock = 0;
	countdown = 400;
	while (countdown != 0) {
		move_right_bottom = -100;
		move_right_top = -100;
		move_left_bottom = 100;
		move_left_top = 100;
		countdown -= 1;
	}
	turn(-45);
}
// Controls
void opcontrol() {
	move_right_bottom_lock = move_right_bottom.get_position();
	move_right_top_lock = move_right_top.get_position();
	move_left_bottom_lock = move_left_bottom.get_position();
	move_left_top_lock = move_left_top.get_position();
	lcd::print(1, "  @@@@@    @@@      @@@    @@@");
	delay(1);
	lcd::print(2," @@       @   @    @  @   @   ");
	delay(1);
	lcd::print(3," @@@@@   @     @  @@@@@  @");
	delay(1);
	lcd::print(4," @@   @   @   @       @   @   ");
	delay(1);
	lcd::print(5,"  @@@@     @@@        @    @@@");
	delay(1);
	lcd::print(6,"        Alan, Eric, Nick");
	delay(1);
	while (true) {
		// Basic Movement
		if (control_dir == 0) {
		move_right_bottom = -master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X);
		move_right_top = -master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X);
		move_left_bottom = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X);
		move_left_top = master.get_analog(ANALOG_LEFT_Y) + master.get_analog(ANALOG_LEFT_X);

		}
		else {
			move_right_bottom = -1 * (-master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X));
			move_right_top = -1 * (-master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X));
			move_left_bottom = -1 *(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X));
			move_left_top = -1 *(master.get_analog(ANALOG_LEFT_Y) - master.get_analog(ANALOG_LEFT_X));

		}
		// Basic Fork Controls
		// Upward Movement
		if (master.get_analog(ANALOG_LEFT_Y) < 5 && master.get_analog(ANALOG_LEFT_Y) > -5) {
			if (master.get_analog(ANALOG_LEFT_X) < 5 && master.get_analog(ANALOG_LEFT_X) > -5) {
				move_right_bottom = move_right_bottom_lock - move_right_bottom.get_position();
				move_right_top = move_right_top_lock - move_right_top.get_position();
				move_left_bottom = move_left_bottom_lock - move_left_bottom.get_position();
				move_left_top = move_left_top_lock - move_left_top.get_position();
			}
			else {
				move_right_bottom_lock = move_right_bottom.get_position();
				move_right_top_lock = move_right_top.get_position();
				move_left_bottom_lock = move_left_bottom.get_position();
				move_left_top_lock = move_left_top.get_position();
			}
		}
		else {
			move_right_bottom_lock = move_right_bottom.get_position();
			move_right_top_lock = move_right_top.get_position();
			move_left_bottom_lock = move_left_bottom.get_position();
			move_left_top_lock = move_left_top.get_position();
		}

		if (master.get_digital(DIGITAL_R1)) {
			fork_right = -100;
			fork_left = 100;
			pos_up = fork_right.get_position();
			pos_down = fork_left.get_position();
		}
		// Downwards Movement
		else if (master.get_digital(DIGITAL_R2)) {
			if (sensor.get_value() == false) {
			    fork_right = 100 / 2;
				fork_left = -100 / 2;
				pos_up = fork_right.get_position();
				pos_down = fork_left.get_position();
			}
		}
			else {
				fork_right = 0;
				fork_left = 0;
		 		fork_right = (pos_up - fork_right.get_position());
				fork_left = (pos_down - fork_left.get_position());
			}

			// CODE
			if (master.get_digital(DIGITAL_L1)) {
				lock_lift = 110;
				pos_2 = lock_lift.get_position();
			}
			// Downwards Movement
			else if (master.get_digital(DIGITAL_L2)) {
				if (sensor2.get_value() == false) {
						lock_lift = -110;
						pos_2 = lock_lift.get_position();
				}
				else {
					lock_lift = 0;
				}
			}
				else {
					lock_lift = 0;
					lock_lift = (pos_2 - lock_lift.get_position());
				}
			// END

			if (master.get_digital(DIGITAL_X)) {
				lock = 60;
				lock_pos = lock.get_position();
			}
			// Downwards Movement
			else if (master.get_digital(DIGITAL_B)) {
				lock = -60;
				lock_pos = lock.get_position();
			}
			else {
				lock = 0;
				lock = (lock_pos - lock.get_position());
			}


		// Movement Direction Enable/Disable

		if (master.get_digital_new_press(DIGITAL_A) == 1) {
			if (control_dir == 1) {
				control_dir = 0;
			}
			else {
				control_dir = 1;
			}
		}
	}

}
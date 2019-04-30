	#include <stdio.h>
	#include <wallaby/wallaby.h>

	#define CLOSED 1000			//Claw closing
	#define OPEN 2047			//Claw opening
	#define CLAW 0				//Grabs Objects
	#define UP 1200				//Makes claw rise
	#define UP_MAX 2047			//Makes claw go rise al the way up
	#define DOWN 600			//Lowers claw 
	#define ARM 2				//Holds the claw and allows it to rise or lower
	#define RIGHT 1				//Right Motor/Wheel
	#define LEFT 3				//Left Motor/Wheel

	void stop() {								//Stop Wallaby/program
		motor(LEFT, 0);
		motor(RIGHT, 0);
	}

	void drive_forwards(power, time){			// Wallaby dirve forwards
		motor(RIGHT, power);
		motor(LEFT, power);
		msleep(time);
		motor(RIGHT, 0);
		motor(LEFT, 0);	
	}

	void turn_left(power, time) {				//Makes Wallaby turn in the left dirrection
		motor(RIGHT, power);
		motor(LEFT, -power);
		msleep(time);
	}

	void turn_right(power, time) {				//Makes Wallaby turn in the right dirrection
		motor(RIGHT, -power);
		motor(LEFT, power);
		msleep(time);
	}

	void change_servo(int servo, int val, int decrease) {	//Setting servo positions for beginning
		while ((!decrease && get_servo_position(servo) < val) || (decrease && get_servo_position(servo) > val)) {
			set_servo_position(servo, get_servo_position(servo) + 8 * (decrease ? -1 : 1));
			msleep(25);
		}
	}

	void close_claw () {	//Closes Claw to hold object(s) and/or to close claw to move foward without bumping into anything
		change_servo(CLAW, CLOSED, 1);
	}

	void open_claw () {		//Opens claw to grab/release objects
		change_servo(CLAW, OPEN, 0);
	}

	void raise_arm () { 	//To higher the claw
		change_servo(ARM, DOWN, 0);
	}

	void lower_arm_beginning () {	//One way to lower claw
		change_servo(ARM, UP, 1);
	}

	void drop_arm () {				//Lower claw all way down
		change_servo(ARM, UP, 1);
	}

	int drive_backwards(int power, int time) {	//Moving Wallaby Backwards
		int temp = 0;
		motor(LEFT, power);
		motor(RIGHT, power);
		while(temp < time) {
			msleep(20);
			temp += 20;
			if(analog(2) > 3800) {
				stop();
				return 1;
			}
		}
		stop();
		return 0;
	}

	int main(){
        wait_for_light(0);
        shut_down_in(119);  
		set_servo_position(ARM, UP_MAX);
		enable_servos(); // initialization
		set_servo_position(CLAW,CLOSED);

		set_servo_position(CLAW,CLOSED);
		lower_arm_beginning(); //Set initial servo positions
		msleep(200);
		
		//Drive to object
		drive_forwards(50,1700);
		turn_left(40,1500);
		drive_forwards(50,6650);
        raise_arm();
        open_claw();
        drive_forwards(25,2050);
        close_claw();
        set_servo_position(CLAW,CLOSED);
      
        //Return to base
      
        drive_backwards(-50,3500);
        drop_arm();
        turn_left(50,1750);
        drive_forwards(50,5450);
        drop_arm();
        open_claw();
        set_servo_position(CLAW,OPEN);
        drive_backwards(-25,2000);
        close_claw();
        set_servo_position(CLAW,CLOSED);
        raise_arm();
        
        //Drive to object again
      
        turn_left(50,1750);
        drive_forwards(50,5450);
        raise_arm();
        open_claw();
        drive_forwards(25,2050);
        close_claw();
        
        //Return to base
      
        drive_backwards(-50,1025);
        drop_arm();
        turn_left(50,1750);
        drive_forwards(50,5450);
        raise_arm();
        open_claw();
        drive_backwards(-25,2000);
        close_claw();
        

		msleep(200);

		return 0;
	}
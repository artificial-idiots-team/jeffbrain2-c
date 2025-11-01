#include "main.h"
#include "pros/apix.h"
#include "globals.hpp"
#include "interface.hpp"


// Initial function
void initialize() {
	drivebase_left.set_gearing(pros::v5::MotorGears::green);
    drivebase_right.set_gearing(pros::v5::MotorGears::green);
    int_motor.set_gearing(pros::v5::MotorGears::green);
    chn_motor.set_gearing(pros::v5::MotorGears::green);

    // arm_motor.set_gearing(pros::v5::MotorGears::red);
    // claw_motor.set_gearing(pros::v5::MotorGears::green);

    drivebase_left.set_reversed(false);
    drivebase_right.set_reversed(false);
    int_motor.set_reversed(false);
    chn_motor.set_reversed(false);

    // arm_motor.set_reversed(false);
    // claw_motor.set_reversed(false);

    initialize_interface();
}

// DISABLED mode
void disabled() {}

// Runs after INITIALIZE when connected to VEX Competition
void competition_initialize() {
	
}

void autonomous() {}


void opcontrol() {
    extern int control_mode;

	while (true) {
        if (control_mode == 0) {
            int dir = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int turn = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
            drivebase_left.move(((turn * 0.6) + dir) * 0.8);
            drivebase_right.move(((turn * 0.6) - dir) * 0.8);
        } else if (control_mode == 1) {
            int leftdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
            int rightdrive = master_controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
            drivebase_left.move(leftdrive);
            drivebase_right.move(-rightdrive);
        }

        if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
            int_motor.move(127);
            chn_motor.move(127);
        } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
            int_motor.move(-127);
            chn_motor.move(-127);
        } else {
            int_motor.brake();
            chn_motor.brake();
        }

        // if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)) {
        //     claw_motor.move(127);
        // } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
        //     claw_motor.move(-127);
        // } else {
        //     claw_motor.brake();
        // }
        //
        // if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) {
        //     arm_motor.move(127);
        // } else if (master_controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
        //     arm_motor.move(-127);
        // } else {
        //     arm_motor.brake();
        // }

        pros::delay(5);
	}
}
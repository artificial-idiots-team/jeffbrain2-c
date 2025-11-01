#include "globals.hpp"

pros::Motor drivebase_left(1);
pros::Motor drivebase_right(2);

pros::Motor int_motor();
pros::Motor chn_motor();

// Old motors from Clawbot Jeff 2.
// pros::Motor arm_motor(4);
// pros::Motor claw_motor(7);

pros::Controller master_controller(pros::E_CONTROLLER_MASTER);
pros::Controller partner_controller(pros::E_CONTROLLER_PARTNER);
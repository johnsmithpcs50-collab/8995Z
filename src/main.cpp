#include "main.h"
#include "robotconfig.h"
#include "lemlib/api.hpp"

const double LIFT_BOTTOM = 0;
const double LIFT_CARRY = 180;
const double LIFT_SCORE = 360;
pros::Controller controller(pros::E_CONTROLLER_MASTER);

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button()
{
    static bool pressed = false;
    pressed = !pressed;
    if (pressed)
    {
        pros::lcd::set_text(2, "I was pressed!");
    }
    else
    {
        pros::lcd::clear_line(2);
    }
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
    pros::lcd::initialize();
    pros::lcd::set_text(1, "Hello PROS User!");

    pros::lcd::register_btn1_cb(on_center_button);

    chassis.calibrate();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
    // do nothing
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off
 */

void TestMovement() {
    chassis.setPose(0, 0, 0);
    chassis.moveToPoint(47, 0, 1000, {.forwards = true});
    pros::delay(3000);
    chassis.turnToHeading(90, 1000);
}

void TestMotors() {
    Lift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    Lift.set_zero_position(LIFT_BOTTOM);
    Lift.move_absolute(LIFT_SCORE, 1000);
    pros::delay(3000);
    Lift.move_absolute(LIFT_CARRY, 1000);
}

void TwoPin() {
    chassis.moveToPoint(-63.5, 0, 1000, {.forwards = false});
    chassis.moveToPoint(-47, 0, 1000, {.forwards = true});

    chassis.turnToHeading(270, 1000);
    chassis.moveToPoint(-47, -16, 1000, {.forwards = true});

    // score
    Lift.move_absolute(LIFT_SCORE, 1000);
    // Claw.extend();

    // raise claw and matchload
    Lift.move_absolute(LIFT_CARRY, 1000);

    // possible change to swing to heading
    chassis.turnToHeading(210, 1000);

    // go to 2nd goal and score
    chassis.moveToPoint(-62, -23.5, 1000, {.forwards = true});
    chassis.turnToHeading(180, 1000);

    Lift.move_absolute(LIFT_BOTTOM, 1000);
    //Claw.retract();

    chassis.moveToPoint(-55, -23.5, 1000, {.forwards = false});
    chassis.turnToHeading(0, 1000);

    Lift.move_absolute(LIFT_SCORE, 1000);
    // Claw.extend();
}

void autonomous() {

    TestMovement();

    TestMotors();

    TwoPin();


    
    
//Full real autonomous code
    // go to goal
    
}

void opcontrol() {
    while (true) {
        // get left y and right y positions
        int leftY = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
        int rightY = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

        // move the robot
        chassis.tank(leftY, rightY);

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
        {
            RachetClaw.move(100);
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
        {
            RachetClaw.move(-100);
        }
        else
        {
            RachetClaw.move(0);
        }

        if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)) //Down 
        {
            Lift.move(100);
        }
        else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) //Up
        {
            Lift.move(-100);
        }
        else
        {
            Lift.move(0);
        }

        // delay to save resources
        pros::delay(25);
    }
}


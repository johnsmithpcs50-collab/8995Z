#include "main.h"
#include "robotconfig.h"
// left motor group
pros::MotorGroup left_motor_group({-1, -2}, pros::MotorGears::green); // motors fixed
// right motor group
pros::MotorGroup right_motor_group({3, 4}, pros::MotorGears::green); //motors ok

pros::MotorGroup Lift({7, -6}, pros::MotorGears::red);

pros::Motor RachetClaw({8}, pros::MotorGears::green); 



// drivetrain settings
lemlib::Drivetrain drivetrain(&left_motor_group, // left motor group
                              &right_motor_group, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 3.25" omnis
                              200, // drivetrain rpm is 200
                              2 // horizontal drift is 2 (for now)
);

// imu
pros::Imu imu(11);
// horizontal tracking wheel encoder
pros::Rotation horizontal_encoder(20);
// vertical tracking wheel encoder
pros::adi::Encoder vertical_encoder('C', 'D', true);
// horizontal tracking wheel
lemlib::TrackingWheel horizontal_tracking_wheel(&horizontal_encoder, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel
lemlib::TrackingWheel vertical_tracking_wheel(&vertical_encoder, lemlib::Omniwheel::NEW_275, -2.5);

// odometry settings
// Use drivetrain motor encoders and the IMU.
// No dedicated tracking wheels.
lemlib::OdomSensors sensors(
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    &imu
);

// lateral PID controller
lemlib::ControllerSettings lateral_controller(
    0, // proportional gain (kP)
    0, // integral gain (kI)
    0, // derivative gain (kD)
    0, // anti windup
    0, // small error range, in inches
    0, // small error range timeout, in milliseconds
    0, // large error range, in inches
    0, // large error range timeout, in milliseconds
    0  // maximum acceleration (slew)
);

// angular PID controller
lemlib::ControllerSettings angular_controller(
    0, // proportional gain (kP)
    0, // integral gain (kI)
    0, // derivative gain (kD)
    0, // anti windup
    0, // small error range, in degrees
    0, // small error range timeout, in milliseconds
    0, // large error range, in degrees
    0, // large error range timeout, in milliseconds
    0  // maximum acceleration (slew)
);

// create the chassis
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

// initialize function. Runs on program startup

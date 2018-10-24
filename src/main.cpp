#include "robot-config.h"
/*---------------------------------------------------------------------------                                        
        Description: Team 2585's VEX Control Software for 2018-19          

Robot Configuration:
[Smart Port]    [Name]        [Type]           [Description]       [Reversed]
Motor Port 1    LeftMotor     V5 Smart Motor    Left side motor     false
Motor Port 10   RightMotor    V5 Smart Motor    Right side motor    true
Motor Port 8    ArmMotor      V5 Smart Motor    Arm motor           false
Motor Port 3    IntakeMotor   V5 Smart Motor    Intake motor        false
Motor Port 5    ShooterMotor  V5 Smart Motor    Shooter motor       false
---------------------------------------------------------------------------*/

//Creates a competition object that allows access to Competition methods.
vex::competition    Competition;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function. You must return from this function    */
/*  or the autonomous and usercontrol tasks will not be started. This        */
/*  function is only called once after the cortex has been powered on and    */ 
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton( void ) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
// Robot measurements
const float wheelDiameter = 4.125; // inches
const float turningDiameter = 25.0; //inches (top left wheel-bottom right wheel)

// Robot starting position
const bool isBlue = false;
const bool isRight = false;

void driveForward( float inches ) { // distance in inches
    float wheelCircumference = wheelDiameter * 3.14159;
    float inchesPerDegree = wheelCircumference / 360;

    float degrees = inches / inchesPerDegree;
    // don't wait for completion so that other wheel can turn at same time
    LeftMotor.startRotateFor(degrees, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightMotor.rotateFor(degrees, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void turn( float degrees ) {
    float turningRatio = turningDiameter / wheelDiameter;

    LeftMotor.startRotateFor(turningRatio * degrees, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightMotor.rotateFor(-turningRatio * degrees, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void shoot( void ) {
    ShooterMotor.rotateFor(2.0, vex::timeUnits::sec, 100, vex::velocityUnits::pct);
}

void autonomous( void ) {
    driveForward( 1.2 * 12 );
    if (isBlue) {
        turn(90);
    } else {
        turn (-90);
    }
    shoot();
    // Flagside: blue, right or red, left
    if (isBlue && isRight || !isBlue && !isRight) {
        driveForward( 4.0 * 12 );
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol( void ) {
    //Use these variables to set the speed of the arm and claw.
    int armSpeedPCT = 50;
    int intakeSpeedPCT = 100;
    int shooterSpeedPCT = 100;
    // User control code here, inside the loop
    while (1) {
        // This is the main execution loop for the user control program.
        // Each time through the loop your program should update motor + servo 
        // values based on feedback from the joysticks.

        // Quick Turn 90 degrees
        if(Controller1.ButtonLeft.pressing()) {
            turn(-90);
        } else if (Controller1.ButtonRight.pressing()) {
            turn(90);
        }

        //Drive Control
        //Set the left and right motor to spin forward using the controller Axis values as the velocity value.
        //Since we are using a single joystick, we will need to calculate the final volicity for each motor.
        int power = Controller1.Axis3.value();
        int rotation = Controller1.Axis1.value()/2;
        LeftMotor.spin(vex::directionType::fwd, power + rotation, vex::velocityUnits::pct);
        RightMotor.spin(vex::directionType::fwd, power - rotation, vex::velocityUnits::pct);
        
        //Arm Control
        if(Controller1.ButtonUp.pressing()) { //If button up is pressed...
            //...Spin the arm motor forward.
            ArmMotor.spin(vex::directionType::fwd, armSpeedPCT, vex::velocityUnits::pct);
        }
        else if(Controller1.ButtonDown.pressing()) { //If the down button is pressed...
            //...Spin the arm motor backward.
            ArmMotor.spin(vex::directionType::rev, armSpeedPCT, vex::velocityUnits::pct);
        }
        else { //If the the up or down button is not pressed...
            //...Stop the arm motor.
            ArmMotor.stop(vex::brakeType::brake);
        }
        
        // Intake Control
        if(Controller1.ButtonL1.pressing()) { //If the upper left trigger is pressed...
            //...Spin the claw motor forward.
            IntakeMotor.spin(vex::directionType::fwd, intakeSpeedPCT, vex::velocityUnits::pct);
        } else if(Controller1.ButtonL2.pressing()) {
            IntakeMotor.spin(vex::directionType::fwd, -intakeSpeedPCT, vex::velocityUnits::pct);
        } else {
            //...Stop the arm motor.
            IntakeMotor.spin(vex::directionType::fwd, 0, vex::velocityUnits::pct);
        }
        
        // TODO: stop intake momentarily when limit switch is hit

        // Shooter Control
        if(Controller1.ButtonR1.pressing()) { //If the upper left trigger is pressed...
            //...Spin the claw motor forward.
            ShooterMotor.spin(vex::directionType::fwd, shooterSpeedPCT, vex::velocityUnits::pct);
        } else {
            //...Stop the arm motor.
            ShooterMotor.spin(vex::directionType::fwd, 0, vex::velocityUnits::pct);
        }
        
        vex::task::sleep(20); //Sleep the task for a short amount of time to prevent wasted resources. 
    }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
    //Run the pre-autonomous function. 
    pre_auton();
    
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );

    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }
}
#include "robot-config.h"
/*---------------------------------------------------------------------------                                        
        Description: Team 2585's VEX Control Software for 2018-19          

Robot Configuration:
[Smart Port]    [Name]        [Type]           [Description]       [Reversed]
Motor Port 10   LeftMotor     V5 Smart Motor    Left side motor     true
Motor Port 1    RightMotor    V5 Smart Motor    Right side motor    false
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
const float turningDiameter = 19.0; //inches (top left wheel-bottom right wheel)
const float gearRatio = 0.5; // 0.5 turn of motor -> 1 turn of wheel

// Robot starting position
const bool isBlue = false;
const bool isRight = false;

void driveForward( float inches ) { // distance in inches
    float wheelCircumference = wheelDiameter * 3.141592653589;
    float inchesPerDegree = wheelCircumference / 360;

    float degrees = inches / inchesPerDegree;
    // don't wait for completion so that other wheel can turn at same time
    LeftMotor.startRotateFor(degrees * gearRatio, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightMotor.rotateFor(degrees * gearRatio, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void turn( float degrees ) {
    // Note: +90 degrees is a right turn
    float turningRatio = turningDiameter / wheelDiameter;

    // Divide by two because each wheel provides half the rotation
    LeftMotor.startRotateFor(turningRatio * degrees * gearRatio / 2, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightMotor.rotateFor(-turningRatio * degrees * gearRatio / 2, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void shoot( float power ) {
    ShooterMotor.rotateFor(2.0, vex::timeUnits::sec, power, vex::velocityUnits::pct);
}

void moveArm(float armSpeed){
    if(armSpeed > 0){
        ArmMotor.spin(vex::directionType::fwd, armSpeed, vex::velocityUnits::pct);
    } else if (armSpeed < 0){
        ArmMotor.spin(vex::directionType::rev, armSpeed, vex::velocityUnits::pct);
    }
}

void intake(float intakeSpeed){
    if(intakeSpeed > 0){
        IntakeMotor.spin(vex::directionType::fwd, intakeSpeed, vex::velocityUnits::pct);
    } else if (intakeSpeed < 0){
        IntakeMotor.spin(vex::directionType::rev, intakeSpeed, vex::velocityUnits::pct);
    }
}

void autonomous( void ) {
    driveForward( 1.2 * 12 );

    if (isBlue) {
        turn(90);
    } else {
        turn (-90);
    }

    // Flagside: blue, right or red, left
    if ((isBlue && isRight) || (!isBlue && !isRight)) {
        float power = 100;
        shoot(power);
        driveForward( 4.0 * 12 );
    } else { // Not flagside, shoot far and don't drive
        float power = 120;
        shoot(power);
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
    int armSpeedPCT = 25;
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
        int power = Controller1.Axis3.value();
        int rotation = Controller1.Axis1.value()/2;
        LeftMotor.spin(vex::directionType::fwd, power + rotation, vex::velocityUnits::pct);
        RightMotor.spin(vex::directionType::fwd, power - rotation, vex::velocityUnits::pct);
        
        //Arm Control
        if(Controller1.ButtonUp.pressing()) { //If button up is pressed...
            //...Spin the arm motor forward.
            moveArm(armSpeedPCT);
        }
        else if(Controller1.ButtonDown.pressing()) { //If the down button is pressed...
            //...Spin the arm motor backward.
            moveArm(-armSpeedPCT);
        }
        else { //If the the up or down button is not pressed...
            //...Stop the arm motor.
            ArmMotor.stop(vex::brakeType::brake);
        }
        
        // Intake Control
        if(Controller1.ButtonL1.pressing()) { //If the upper left trigger is pressed...
            //...Spin the claw motor forward.
            intake(intakeSpeedPCT);
        } else if(Controller1.ButtonL2.pressing()) {
            //...Spin the claw motor backward.
            intake(-intakeSpeedPCT);
        } else {
            //...Stop the arm motor.
            IntakeMotor.spin(vex::directionType::fwd, 0, vex::velocityUnits::pct);
        }
        
        // TODO: stop intake momentarily when limit switch is hit

        // Shooter Control
        if(Controller1.ButtonR1.pressing()) { //If the upper left trigger is pressed...
            //...Spin the claw motor forward.
            shoot(shooterSpeedPCT);
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
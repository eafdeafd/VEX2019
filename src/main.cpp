#include "robot-config.h"
// Robot starting position
const bool isBlue = false;
const bool isRight = false;
/*---------------------------------------------------------------------------                                        
        Description: Team 2585's VEX Control Software for 2018-19          

Robot Configuration:
[Smart Port]   [Name]          [Type]            [Description]       [Reversed]
Motor Port 1   LeftBackMotor   V5 Smart Motor    Left side motor     false
Motor Port 2   LeftFrontMotor  V5 Smart Motor    Left side motor     false
Motor Port 10  RightBackMotor  V5 Smart Motor    Right side motor    true
Motor Port 9   RightFrontMotor V5 Smart Motor    Right side motor    true
Motor Port 7   ArmMotor        V5 Smart Motor    Arm motor           false
Motor Port 3   IntakeMotor     V5 Smart Motor    Intake motor        false
Motor Port 5   ShooterMotor    V5 Smart Motor    Shooter motor       false

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
const float turningDiameter = 21.5; //inches (top left wheel-bottom right wheel)
const float gearRatio = 0.5; // 0.5 turn of motor -> 1 turn of wheel


void driveForward( float inches ) { // distance in inches
    float wheelCircumference = wheelDiameter * 3.141592653589;
    float inchesPerDegree = wheelCircumference / 360;

    float degreesTurn = inches / inchesPerDegree * gearRatio;
    // don't wait for completion so that other wheel can turn at same time
    LeftBackMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    LeftFrontMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    RightBackMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.rotateFor(degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void turn( float degrees ) {
    // Note: +90 degrees is a right turn
    float turningRatio = turningDiameter / wheelDiameter;

    float degreesTurn = turningRatio * degrees * gearRatio / 2;
    // Divide by two because each wheel provides half the rotation
    LeftBackMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    LeftFrontMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    RightBackMotor.startRotateFor(-degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.rotateFor(-degreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void shoot( float power, bool isAuton) {
    if(isAuton){
        ShooterMotor.rotateFor(2.0, vex::timeUnits::sec, power, vex::velocityUnits::pct);
    } else {
        ShooterMotor.spin(vex::directionType::fwd, power, vex::velocityUnits::pct);
    }
}

void moveArm(float armSpeed){
    if (armSpeed == 0) {
        ArmMotor.stop(vex::brakeType::brake);
    } else {
        ArmMotor.spin(vex::directionType::fwd, armSpeed, vex::velocityUnits::pct);
    }
}

void intake(float intakeSpeed){
    IntakeMotor.spin(vex::directionType::fwd, intakeSpeed, vex::velocityUnits::pct);
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
        shoot(power, true);
        driveForward( 4.0 * 12 );
    } else { // Not flagside, shoot far and don't drive
        float power = 120;
        shoot(power, true);
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
bool isLimitSwitchPressed( void ) {
    return IntakeLimitSwitch.value() == 0;
}

void usercontrol( void ) {
    //Use these variables to set the speed of the arm and claw.
    int armSpeedPCT = 25;
    int intakeSpeedPCT = 127;
    int shooterSpeedPCT = 100;

    bool isReversed = false;
    Controller1.Screen.print("Welcome Abort Captain");
    Controller1.Screen.newLine();
    Controller1.Screen.print("Get ready to rumble!!!");
    Controller1.Screen.newLine();

    Controller1.Screen.print("FORWARD MODE!");
    Controller1.Screen.newLine();
    Controller1.rumble("--..-");

    bool isIntakeDisabled = false;

    bool wasIntakePressed = false;
    bool wasLimitSwitchPressed = false;
    bool wasXPressed = false;

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

        if (isReversed) {
            power *= -1;
        }
        LeftBackMotor.spin(vex::directionType::fwd, power + rotation, vex::velocityUnits::pct);
        LeftFrontMotor.spin(vex::directionType::fwd, power + rotation, vex::velocityUnits::pct);

        RightBackMotor.spin(vex::directionType::fwd, power - rotation, vex::velocityUnits::pct);
        RightFrontMotor.spin(vex::directionType::fwd, power - rotation, vex::velocityUnits::pct);

        if (Controller1.ButtonX.pressing() && !wasXPressed) {
            // Just started pressing X
            // Switch modes
            if (!isReversed) { // Wasn't reversed
                // Change to reverse
                isReversed = true;
                Controller1.Screen.print("REVERSE MODE!");
                Controller1.Screen.newLine();
                Controller1.rumble("...");
            } else { // Was reversed
                // Change to forward
                isReversed = false;
                Controller1.Screen.print("FORWARD MODE!");
                Controller1.Screen.newLine();
                Controller1.rumble("...");
            }
        }
        wasXPressed = Controller1.ButtonX.pressing();

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
            //...Brake the arm motor.
            moveArm(0);
        }
        
        // Intake Control
        if(isIntakeDisabled) {
            intake(0);
        } else {
            if(Controller1.ButtonL1.pressing()) { //If the upper left trigger is pressed...
                //...Spin the intake motor forward.
                intake(intakeSpeedPCT);
            } else if(Controller1.ButtonL2.pressing()) {
                //...Spin the intake motor backward.
                intake(-intakeSpeedPCT);
            } else {
                //...Stop spinning intake motor.
                intake(0);
            }
        }

        if (isLimitSwitchPressed() && !wasIntakePressed) {
            // limit switch not pressed -> pressed
            if (!Controller1.ButtonR1.pressing()) {
                // Not running shooter, disable the intake
                isIntakeDisabled = true;
                Controller1.rumble("..");
                Controller1.Screen.print("Ball intaken");
            }
        }
        if (Controller1.ButtonL1.pressing() && !wasIntakePressed) {
            // button not pressed -> pressed
            isIntakeDisabled = false;
        }
        wasLimitSwitchPressed = isLimitSwitchPressed();
        wasIntakePressed = Controller1.ButtonL1.pressing();

        // Shooter Control
        if(Controller1.ButtonR1.pressing()) {
            //...Spin the shooter motor forward.
            shoot(shooterSpeedPCT, false);
        } else {
            //...Stop the shooter motor.
            shoot(0, false);
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
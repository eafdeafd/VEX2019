#include "robot-config.h"
/*---------------------------------------------------------------------------                                        
        Description: Team 2585's VEX Control Software for 2018-19          

Robot Configuration:
[Smart Port]   [Name]          [Type]            [Description]       [Reversed]
Motor Port 1   LeftBackMotor   V5 Smart Motor    Left side motor     false
Motor Port 2   LeftFrontMotor  V5 Smart Motor    Left side motor     false
Motor Port 10  RightBackMotor  V5 Smart Motor    Right side motor    true
Motor Port 9   RightFrontMotor V5 Smart Motor    Right side motor    true
Motor Port 11  ArmMotor        V5 Smart Motor    Arm motor           false
Motor Port 20  Indexer         V5 Smart Motor    Indexer             false
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
const float gearRatio = 0.5; // 1 turn of motor -> 2 turns of wheel


void driveForward( float inches, float power ) { // distance in inches
    float wheelCircumference = wheelDiameter * 3.141592653589;
    float inchesPerDegree = wheelCircumference / 360.0;

    float degreesTurn = inches / inchesPerDegree * gearRatio;
    // don't wait for completion so that other wheel can turn at same time
    LeftFrontMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);
    LeftBackMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);

    RightFrontMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);
    RightBackMotor.rotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);
}

void turn( float degrees ) {
    // Note: +90 degrees is a right turn
    float turningRatio = turningDiameter / wheelDiameter;

    float wheelDegreesTurn = turningRatio * degrees;
    // Shouldn't need to be multiplied by 2. TODO: figure this out...
    float motorDegreesTurn = wheelDegreesTurn * gearRatio;

    LeftBackMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    LeftFrontMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    RightBackMotor.startRotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.rotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    // V1: Only power front motors
    //LeftFrontMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 100, vex::velocityUnits::pct);
    //RightFrontMotor.rotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 100, vex::velocityUnits::pct);

 /*
    // V3: only count degrees on one of the motors
    RightFrontMotor.spin(vex::directionType::fwd, 50, vex::velocityUnits::pct);
    LeftFrontMotor.rotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.stop();

    // VGarbage: power all motors
    LeftBackMotor.startRotateFor(motorDegreesTurn * 2, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    LeftFrontMotor.startRotateFor(motorDegreesTurn * 2, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    RightBackMotor.startRotateFor(-motorDegreesTurn * 2, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.rotateFor(-motorDegreesTurn * 2, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
*/
}

void intake(float intakeSpeed){
    IntakeMotor.spin(vex::directionType::fwd, intakeSpeed, vex::velocityUnits::pct);
}

void shoot(float power) {
    // Spin up the shooter
    ShooterMotor.rotateFor(2.0, vex::timeUnits::sec, power, vex::velocityUnits::pct);
    // Run the intake to bring the ball up
    // At the same time, run the shooter to shoot the ball
    intake(power);
    ShooterMotor.rotateFor(3.0, vex::timeUnits::sec, power, vex::velocityUnits::pct);
    intake(0);
}

void powerDownShooter(float velocityPCT){
    ShooterMotor.setVelocity(velocityPCT - 1, vex::velocityUnits::pct);
}          

void powerUpShooter(float velocityPCT){
    ShooterMotor.setVelocity(velocityPCT + 5, vex::velocityUnits::pct);

}

void moveArm(float armSpeed) {
    if (armSpeed == 0) {
        ArmMotor.stop(vex::brakeType::brake);
    } else {
        ArmMotor.spin(vex::directionType::fwd, armSpeed, vex::velocityUnits::pct);
    }
}

void turnIndexer(float indexSpeed){
    if(indexSpeed == 0){
        Indexer.stop(vex::brakeType::brake);
    } else {
        Indexer.spin(vex::directionType::fwd, indexSpeed, vex::velocityUnits::pct);
    }
}

void autonomous( void ) {
    if (!shouldRunAuton) {
        return;
    }

    if (justDriveStraight) {
        intake(127.0);        
        driveForward( 3.3 * 12, 50.0 ); // was 2.4
        IntakeMotor.rotateFor(0.5, vex::timeUnits::sec, 127.0, vex::velocityUnits::pct);
        intake(0.0);
        return;
    } else {
        driveForward( 1.2 * 12, 110.0 ); // 1.2 ft * 12 in/ft
    }

    if (isBlue) {
        turn(90);
    } else {
        turn (-90);
    }

    // Flagside: blue, right or red, left
    if ((isBlue && isRight) || (!isBlue && !isRight)) {
        float power = 100;
        shoot(power);
        driveForward( 4.0 * 12, 90.0 );
    } else { // Not flagside, shoot far and don't drive
        float power = 110;
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
bool isLimitSwitchPressed( void ) {
    return IntakeLimitSwitch.value() == 0;
}

void usercontrol( void ) {
    //Use these variables to set the speed of the arm, intake, and shooter.
    int armSpeedPCT = 20;
    int intakeSpeedPCT = 127;
    int indexSpeed = 60;
    int currentShooterSpeedPCT = 0;
    
    bool isReversed = false;
    Controller1.Screen.print("Welcome Aboard!");
    Controller1.Screen.newLine();
    Controller1.Screen.print("Get ready to rumble!!!");
    Controller1.Screen.newLine();

    Controller1.Screen.print("FORWARD MODE!");
    Controller1.Screen.newLine();
    Controller1.rumble("--..-");

    bool isIntakeDisabled = false;

    // Keep track of past button state to detect inital presses
    // TODO: change to callback functions
    bool wasIntakePressed = false;
    bool wasLimitSwitchPressed = false;
    bool wasUpPressed = false;
    bool wasDownPressed = false;
    bool wasLeftPressed = false;
    bool wasRightPressed = false;

    while (1) {
        // This is the main execution loop for the user control program.
        // Each time through the loop your program should update motor + servo 
    
        // Quick Turn 90 degrees
        if(Controller1.ButtonLeft.pressing() && !wasLeftPressed) {
            //turn(-90);
            // turn(-180);
            //driveForward(-12);
            //shoot(shooterSpeedPCT, true);
        } else if (Controller1.ButtonRight.pressing() && !wasRightPressed) {
            //turn(90);
            // turn(180);
            //driveForward(12);
            //shoot(shooterSpeedPCT, true);
        }
        wasLeftPressed = Controller1.ButtonLeft.pressing();
        wasRightPressed = Controller1.ButtonRight.pressing();

        //Drive Control
        int powerPCT = Controller1.Axis3.value();
        int rotationPCT = Controller1.Axis1.value() * 0.4;

        if (isReversed) {
            powerPCT *= -1;
        }
        LeftBackMotor.spin(vex::directionType::fwd, powerPCT + rotationPCT, vex::velocityUnits::pct);
        LeftFrontMotor.spin(vex::directionType::fwd, powerPCT + rotationPCT, vex::velocityUnits::pct);

        RightBackMotor.spin(vex::directionType::fwd, powerPCT - rotationPCT, vex::velocityUnits::pct);
        RightFrontMotor.spin(vex::directionType::fwd, powerPCT - rotationPCT, vex::velocityUnits::pct);

        if (Controller1.ButtonUp.pressing() && !wasUpPressed) {
            // Change to forward
            isReversed = false;
            Controller1.Screen.print("FORWARD MODE!");
            Controller1.Screen.newLine();
            Controller1.rumble("...");
        } else if (Controller1.ButtonDown.pressing() && !wasDownPressed) {
            // Change to reverse
            isReversed = true;
            Controller1.Screen.print("REVERSE MODE!");
            Controller1.Screen.newLine();
            Controller1.rumble("...");
        }

        wasUpPressed = Controller1.ButtonUp.pressing();
        wasDownPressed = Controller1.ButtonDown.pressing();

        //Arm Control: X is up, Y is down
        if(Controller1.ButtonY.pressing()) { //If button up is pressed...
            //...Spin the arm motor forward.
            moveArm(armSpeedPCT);
        } else if(Controller1.ButtonX.pressing()) { //If the down button is pressed...
            //...Spin the arm motor backward.
            moveArm(-armSpeedPCT);
        } else { //If the the up or down button is not pressed...
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
                // Not running the shooter, disable the intake
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

        //Indexer Control: to-do(once we get more info on how it will work)
        if(Controller1.ButtonA.pressing()) { //If button up is pressed...
            //...Spin the arm motor forward.
            turnIndexer(indexSpeed);
        } else if(Controller1.ButtonB.pressing()) { //If the down button is pressed...
            //...Spin the arm motor backward.
            turnIndexer(-indexSpeed);
        } else { //If the the up or down button is not pressed...
            //...Brake the arm motor.
            turnIndexer(0);
        }
        
        // Shooter Control
        currentShooterSpeedPCT = ShooterMotor.velocity(vex::velocityUnits::pct);
        if(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()) {
            //...Spin the shooter motor forward.
            if(ShooterMotor.velocity(vex::velocityUnits::pct) < 120){
                powerUpShooter(currentShooterSpeedPCT);
            } else {
               Controller1.rumble(".-.");
               Controller1.Screen.print("FULL POWER REACHED");
               ShooterMotor.spin(vex::directionType::fwd, 127, vex::velocityUnits::pct);
           }    
        } else {
            //...Stop the shooter motor.
            if(ShooterMotor.velocity(vex::velocityUnits::pct) > 5){
               powerDownShooter(currentShooterSpeedPCT);
            } else {
               ShooterMotor.spin(vex::directionType::fwd, 0, vex::velocityUnits::pct);
            }
        }
        vex::task::sleep(30); //Sleep the task for a short amount of time to prevent wasted resources. 
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
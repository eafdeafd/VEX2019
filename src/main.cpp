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
Motor Port 20  Feeder          V5 Smart Motor    Feeder              false
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
const float turningDiameter = 17.5; //inches (top left wheel-bottom right wheel)
const float gearRatio = 1.0; // 1 turns of motor/turns of wheel


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

    float motorDegreesTurn = wheelDegreesTurn * gearRatio;

    LeftBackMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    LeftFrontMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    RightBackMotor.startRotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.rotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
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

void runFeeder(float feederSpeed){
    if(feederSpeed == 0){
        Feeder.stop(vex::brakeType::brake);
    } else {
        Feeder.spin(vex::directionType::fwd, feederSpeed, vex::velocityUnits::pct);
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
    int feederSpeed = 60;
    int currentShooterSpeedPCT = 0;
    
    bool isReversed = false;
    Controller1.Screen.print("Welcome Aboard!");
    Controller1.Screen.newLine();
    Controller1.Screen.print("Get ready to rumble!!!");
    Controller1.Screen.newLine();

    Controller1.Screen.print("FORWARD MODE!");
    Controller1.Screen.newLine();
    Controller1.rumble("--..-");


    // Keep track of past button state to detect inital presses
    bool wasIntakePressed = false;
    bool wasUpPressed = false;
    bool wasDownPressed = false;

    while (1) {
        // This is the main execution loop for the user control program.
        // Each time through the loop your program should update motor + servo

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
        wasIntakePressed = Controller1.ButtonL1.pressing();

        //Feeder Control
        if(Controller1.ButtonA.pressing()) {
            runFeeder(feederSpeed);
        } else if(Controller1.ButtonB.pressing()) {
            runFeeder(-feederSpeed);
        } else {
            runFeeder(0);
        }

        // Shooter Control
        currentShooterSpeedPCT = ShooterMotor.velocity(vex::velocityUnits::pct);
        if(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()) {
            //...Spin the shooter motor forward.
            if(ShooterMotor.velocity(vex::velocityUnits::pct) < 100){
                powerUpShooter(currentShooterSpeedPCT);
            } else {
               Controller1.rumble(".-.");
               Controller1.Screen.print("FULL POWER REACHED");
               ShooterMotor.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
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
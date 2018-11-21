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
Motor Port 7   ShooterMotor    V5 Smart Motor    Shooter motor       false
Motor Port 5   VisionSensor    V5 Vision Sensor  Vision sensor       N/A

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

// Robot measurements
const float wheelDiameter = 4.125; // inches
const float turningDiameter = 17.5; //inches (top left wheel-bottom right wheel)
const float gearRatio = 1.0; // 1 turns of motor/turns of wheel

void runIntake(float intakeSpeed){
    IntakeMotor.spin(vex::directionType::fwd, intakeSpeed, vex::velocityUnits::pct);
}

void runArm(float armSpeed) {
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

void runDrive(float powerPCT, float rotationPCT) {
    //positive rotation --> turning right
    //negative rotation --> turning left
    LeftBackMotor.spin(vex::directionType::fwd, powerPCT + rotationPCT, vex::velocityUnits::pct);
    LeftFrontMotor.spin(vex::directionType::fwd, powerPCT + rotationPCT, vex::velocityUnits::pct);

    RightBackMotor.spin(vex::directionType::fwd, powerPCT - rotationPCT, vex::velocityUnits::pct);
    RightFrontMotor.spin(vex::directionType::fwd, powerPCT - rotationPCT, vex::velocityUnits::pct);
}

void powerDownShooter(float velocityPCT){
    ShooterMotor.setVelocity(velocityPCT - 0.5, vex::velocityUnits::pct);
}

void powerUpShooter(float velocityPCT){
    ShooterMotor.setVelocity(velocityPCT + 8, vex::velocityUnits::pct);
}

void autoDriveForward( float inches, float power ) { // distance in inches
    float wheelCircumference = wheelDiameter * 3.141592653589;
    float inchesPerDegree = wheelCircumference / 360.0;

    float degreesTurn = inches / inchesPerDegree * gearRatio;
    // don't wait for completion so that other wheel can turn at same time
    LeftFrontMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);
    LeftBackMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);

    RightFrontMotor.startRotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);
    RightBackMotor.rotateFor(degreesTurn, vex::rotationUnits::deg, power, vex::velocityUnits::pct);
}

void autoTurn( float degrees ) {
    // Note: +90 degrees is a right turn
    float turningRatio = turningDiameter / wheelDiameter;
    float wheelDegreesTurn = turningRatio * degrees;
    float motorDegreesTurn = wheelDegreesTurn * gearRatio;

    LeftBackMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    LeftFrontMotor.startRotateFor(motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);

    RightBackMotor.startRotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
    RightFrontMotor.rotateFor(-motorDegreesTurn, vex::rotationUnits::deg, 50, vex::velocityUnits::pct);
}

void autoPowerUpShooter(float power) {
    int shooterVelocity = ShooterMotor.velocity(vex::velocityUnits::pct);
    while (shooterVelocity < power - 10) {
        powerUpShooter(power);
        shooterVelocity = ShooterMotor.velocity(vex::velocityUnits::pct);
        vex::task::sleep(10);
    }
    ShooterMotor.spin(vex::directionType::fwd, power, vex::velocityUnits::pct);
}

void autoPowerDownShooter() {
    int shooterVelocity = ShooterMotor.velocity(vex::velocityUnits::pct);
    while (shooterVelocity > 10) {
        powerDownShooter(0);
        shooterVelocity = ShooterMotor.velocity(vex::velocityUnits::pct);
    }
    ShooterMotor.stop(vex::brakeType::coast);
}

void autoShoot(int shootPower) {
    int intakePower = 100;
    int feederPower = 60;
    // Spin up the shooter
    autoPowerUpShooter(shootPower);
    vex::task::sleep(500);
    // Run the intake and feeder to bring the ball up
    runIntake(intakePower);
    runFeeder(feederPower);
    vex::task::sleep(2000);
    // At the same time, run the shooter to shoot the ball
    runIntake(0);
    runFeeder(0);
    autoPowerDownShooter();
}

void pointTo(vex::vision::signature sig) {
    //camera image is 316 pixels wide, so the center is 316/2
    int screenMiddleX = 316 / 2;
    bool isLinedUp = false;
    while(!isLinedUp) {
        //snap a picture
        VisionSensor.takeSnapshot(sig);
        //did we see anything?
        if(VisionSensor.objectCount > 0) {
            //where was the largest thing?
            if(VisionSensor.largestObject.centerX < screenMiddleX - 5) {
                //on the left, turn left
                runDrive(0, -10);
            } else if (VisionSensor.largestObject.centerX > screenMiddleX + 5) {
                //on the right, turn right
                runDrive(0, 10);
            } else {
                //in the middle, we're done lining up
                isLinedUp = true;
                runDrive(0, 0);
            }
        } else {
            //saw nothing, rotate
            runDrive(0, -40);
        }
    }
}

void pointToDrive(vex::vision::signature sig){
    //camera image is 316 pixels wide, so the center is 316/2
    int screenMiddleX = 316 / 2;
    bool isLinedUp = false;
    while(!isLinedUp) {
        //snap a picture
        VisionSensor.takeSnapshot(sig);
        //did we see anything?
        if(VisionSensor.objectCount > 0) {
            //where was the largest thing?
            if(VisionSensor.largestObject.centerX < screenMiddleX - 5) {
                //on the left, turn left
                runDrive(30, -10);
            } else if (VisionSensor.largestObject.centerX > screenMiddleX + 5) {
                //on the right, turn right
                runDrive(30, 10);
            } else {
                //in the middle, we're done lining up
                isLinedUp = true;
                runDrive(30, 0);
            }
        } else {
            //saw nothing, rotate
            runDrive(0, 40);
        }
    }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control the robot during the autonomous phase of    */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/

void autonomous( void ) {
    if (!shouldRunAuton) {
        return;
    }

    if (justDriveStraight) {
        runIntake(100);
        autoDriveForward( 3.3 * 12, 50.0 ); // was 2.4
        IntakeMotor.rotateFor(0.5, vex::timeUnits::sec, 127.0, vex::velocityUnits::pct);
        runIntake(0);
        return;
    }// else {
    //    autoDriveForward( 1.2 * 12, 110.0 ); // 1.2 ft * 12 in/ft
    //}

    //if (isBlue) {
    //    autoTurn(90);
    //} else {
    //    autoTurn(-90);
    //}

    // Flagside: blue, right or red, left
    //if ((isBlue && isRight) || (!isBlue && !isRight)) {
    //    float power = 80;
    //    autoShoot(power);
    //    autoDriveForward( 4.0 * 12, 90.0 );
    //} else { // Not flagside, shoot far and don't drive
    //    float power = 100;
    //    autoShoot(power);
    //}
   // int currentVel = ShooterMotor.velocity(vex::velocityUnits::pct);
    //while(currentVel < 80){
    //    powerUpShooter(currentVel);
    //    currentVel = ShooterMotor.velocity(vex::velocityUnits::pct);
    //}
    ShooterMotor.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
    runDrive(60, 0);
    vex::task::sleep(700);
    runDrive(0, 0);
    vex::task::sleep(5000);
    
    IntakeMotor.startRotateFor(100, vex::rotationUnits::rev, 100, vex::velocityUnits::pct);
    Feeder.startRotateFor(10, vex::rotationUnits::rev, 60, vex::velocityUnits::pct);
    vex::task::sleep(4000);
    IntakeMotor.stop(vex::brakeType::coast);
    Feeder.stop(vex::brakeType::coast);
    ShooterMotor.stop(vex::brakeType::coast);
    
    pointTo(GREEN_FLAG);
    runDrive(40, 0);
    vex::task::sleep(3000);
    
    runDrive(-60, 0);
    vex::task::sleep(1500);
    
    pointTo(BLUE_OBJ);
    IntakeMotor.setReversed(true);
    IntakeMotor.startRotateFor(50, vex::rotationUnits::rev, 100, vex::velocityUnits::pct);
    pointToDrive(BLUE_OBJ);
    vex::task::sleep(2000);
    IntakeMotor.stop(vex::brakeType::coast);
}
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                           Programming Skills                              */
/*                                                                           */
/*  This task is used to control the robot during the autonomous phase of    */
/*  a VEX Competition. For use during programming skills event               */
/*---------------------------------------------------------------------------*/

void programmingSkills ( void ) {
    // Drive forward
    // Turn towards flag
    // Finish pointing
    // Shoot the ball
    // Drive forward
    // Back up
    // Turn right
    // Back Up to align and get a solid starting position
    // Drive forward and intake
    // Back up
    // Turn right
    // Drive forward to park
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
    //Use these variables to set the speed of the arm, intake, and shooter.
    int armSpeedPCT = 30;
    int intakeSpeedPCT = 100;
    int feederSpeedPCT = 60;
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
        int rotationPCT = Controller1.Axis1.value() * 0.3;

        if (isReversed) {
            powerPCT *= -1;
        }
        runDrive(powerPCT, rotationPCT);

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
            runArm(armSpeedPCT);
        } else if(Controller1.ButtonX.pressing()) { //If the down button is pressed...
            //...Spin the arm motor backward.
            runArm(-armSpeedPCT);
        } else { //If the the up or down button is not pressed...
            //...Brake the arm motor.
            runArm(0);
        }

        // Intake Control
        if(Controller1.ButtonL1.pressing()) { //If the upper left trigger is pressed...
            //...Spin the intake motor forward.
            runIntake(intakeSpeedPCT);
        } else if(Controller1.ButtonL2.pressing()) {
            //...Spin the intake motor backward.
            runIntake(-intakeSpeedPCT);
        } else {
            //...Stop spinning intake motor.
            runIntake(0);
        }
        wasIntakePressed = Controller1.ButtonL1.pressing();

        //Feeder Control
        if(Controller1.ButtonA.pressing()) {
            runFeeder(feederSpeedPCT);
        } else if(Controller1.ButtonB.pressing()) {
            runFeeder(-feederSpeedPCT);
        } else {
            runFeeder(0);
        }

        //Feeder Control
        if(Controller1.ButtonA.pressing()) {
            runFeeder(feederSpeedPCT);
        } else if(Controller1.ButtonB.pressing()) {
            runFeeder(-feederSpeedPCT);
        } else {
            runFeeder(0);
        }

        // Shooter Control
        currentShooterSpeedPCT = ShooterMotor.velocity(vex::velocityUnits::pct);
        if(Controller1.ButtonR1.pressing() || Controller1.ButtonR2.pressing()) {
            //...Spin the shooter motor forward.
            if(ShooterMotor.velocity(vex::velocityUnits::pct) < 90){
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
    if (isProgrammingSkillsChallenge) {
        Competition.autonomous( programmingSkills );
    } else {
        Competition.autonomous( autonomous );
    }

    Competition.drivercontrol( usercontrol );

    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }
}
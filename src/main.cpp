#include "robot-config.h"
/*---------------------------------------------------------------------------                                        
        Description: Team 2585's VEX Control Software for 2018-19          

Robot Configuration:
[Smart Port]    [Name]        [Type]           [Description]       [Reversed]
Motor Port 1    LeftMotor     V5 Smart Motor    Left side motor     false
Motor Port 10   RightMotor    V5 Smart Motor    Right side motor    true
Motor Port 3    ArmMotor      V5 Smart Motor    Arm motor          false
Motor Port 8    ClawMotor     V5 Smart Motor    Claw motor         false
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

void autonomous( void ) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................

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
    int clawSpeedPCT = 50;
    // User control code here, inside the loop
    while (1) {
        // This is the main execution loop for the user control program.
        // Each time through the loop your program should update motor + servo 
        // values based on feedback from the joysticks.

        //Drive Control
        //Set the left and right motor to spin forward using the controller Axis values as the velocity value.
        //Since we are using a single joystick, we will need to calculate the final volicity for each motor.
        LeftMotor.spin(vex::directionType::fwd, (Controller1.Axis3.value() + Controller1.Axis4.value())/2, vex::velocityUnits::pct); //(Axis3+Axis4)/2
        RightMotor.spin(vex::directionType::fwd, (Controller1.Axis3.value() - Controller1.Axis4.value())/2, vex::velocityUnits::pct);//(Axis3-Axis4)/2
        
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
        
        //Claw Control
        if(Controller1.ButtonA.pressing()) { //If the A button is pressed...
            //...Spin the claw motor forward.
            ClawMotor.spin(vex::directionType::fwd, clawSpeedPCT, vex::velocityUnits::pct);
        }
        else if(Controller1.ButtonY.pressing()) { //If the Y button is pressed...
            //...Spin the claw motor backward.
            ClawMotor.spin(vex::directionType::rev, clawSpeedPCT, vex::velocityUnits::pct);
        }
        else { //If the A or Y button are not pressed...        
            //...Stop the claw motor.
            ClawMotor.stop(vex::brakeType::brake);        
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
vex::brain Brain;
// Left side reversed. Usually right side is reversed,
// but our gearing reverses the rotation again, making
// the left side the one that needs to be reversed
vex::motor LeftBackMotor = vex::motor(vex::PORT1, false);
vex::motor LeftFrontMotor = vex::motor(vex::PORT2, false);
vex::motor RightBackMotor = vex::motor(vex::PORT10, true);
vex::motor RightFrontMotor = vex::motor(vex::PORT9, true);
vex::motor ArmMotor = vex::motor(vex::PORT11);
vex::motor Feeder = vex::motor(vex::PORT20);
vex::motor IntakeMotor = vex::motor(vex::PORT3);
vex::motor ShooterMotor = vex::motor(vex::PORT5, false);
vex::controller Controller1 = vex::controller();
vex::digital_in IntakeLimitSwitch = vex::digital_in(Brain.ThreeWirePort.B);

// Robot starting position
const bool shouldRunAuton = true;
const bool justDriveStraight = true;
const bool isBlue = false;
const bool isRight = false;

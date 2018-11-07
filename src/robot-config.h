vex::brain Brain;
// Left side reversed. Usually right side is reversed,
// but our gearing reverses the rotation again, making
// the left side the one that needs to be reversed
vex::motor LeftBackMotor = vex::motor(vex::PORT1, false);
vex::motor LeftFrontMotor = vex::motor(vex::PORT2, false);
vex::motor RightBackMotor = vex::motor(vex::PORT10, true);
vex::motor RightFrontMotor = vex::motor(vex::PORT9, true);
vex::motor ArmMotor = vex::motor(vex::PORT7, true);
vex::motor IntakeMotor = vex::motor(vex::PORT3);
vex::motor ShooterMotor = vex::motor(vex::PORT5);
vex::controller Controller1 = vex::controller();
vex::digital_in IntakeLimitSwitch = vex::digital_in(Brain.ThreeWirePort.B);
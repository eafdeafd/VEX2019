vex::brain Brain;
// Left side reversed. Usually right side is reversed,
// but our gearing reverses the rotation again, making
// the left side the one that needs to be reversed
vex::motor LeftMotor = vex::motor(vex::PORT10, true);
vex::motor RightMotor = vex::motor(vex::PORT1, false);
vex::motor ArmMotor = vex::motor(vex::PORT8);
vex::motor IntakeMotor = vex::motor(vex::PORT3);
vex::motor ShooterMotor = vex::motor(vex::PORT5);
vex::controller Controller1 = vex::controller();
vex::digital_in IntakeLimitSwitch = vex::digital_in(Brain.ThreeWirePort.H);
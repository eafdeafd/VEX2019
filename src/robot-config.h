vex::brain Brain;
vex::motor LeftMotor = vex::motor(vex::PORT1);
vex::motor RightMotor = vex::motor(vex::PORT10, true);
vex::motor ArmMotor = vex::motor(vex::PORT8);
vex::motor ClawMotor = vex::motor(vex::PORT3);
vex::controller Controller1 = vex::controller();
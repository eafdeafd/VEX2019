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
vex::motor ShooterMotor = vex::motor(vex::PORT7, false);
vex::controller Controller1 = vex::controller();
vex::vision::signature BLUE_OBJ (1, -3657, -2305, -2982, 6227, 12743, 9484, 2.1, 0);
vex::vision::signature RED_OBJ (2, 6769, 10637, 8703, -2445, -577, -1511, 2.200000047683716, 0);
vex::vision::signature YELLOW_BALL (3, -331, 1, -165, -4739, -1117, -2928, 1, 0);
vex::vision::signature GREEN_FLAG (4, -4471, -3747, -4109, -6521, -5421, -5971, 4, 0);
vex::vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision VisionSensor (vex::PORT5, 50, BLUE_OBJ, RED_OBJ, YELLOW_BALL, GREEN_FLAG, SIG_5, SIG_6, SIG_7);

const bool isProgrammingSkillsChallenge = false;

const bool shouldRunAuton = true;
const bool justDriveStraight = false;
// Robot starting position
const bool isBlue = false;
const bool isRight = false;

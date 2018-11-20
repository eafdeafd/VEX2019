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
vex::vision::signature BLUE_OBJ (1, -3657, -2305, -2982, 6227, 12743, 9484, 2.2, 0);
vex::vision::signature RED_OBJ (2, 8211, 11869, 10040, -1093, -303, -698, 3, 0);
vex::vision::signature YELLOW_BALL (3, 1383, 1805, 1594, -4531, -4109, -4320, 5.2, 0);
vex::vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 4, 0);
vex::vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision VisionSensor (vex::PORT5, 50, BLUE_OBJ, RED_OBJ, YELLOW_BALL, SIG_4, SIG_5, SIG_6, SIG_7);

const bool isProgrammingSkillsChallenge = true;

const bool shouldRunAuton = true;
const bool justDriveStraight = true;
// Robot starting position
const bool isBlue = false;
const bool isRight = false;
using namespace vex;
brain Brain;
// Left side reversed. Usually right side is reversed,
// but our gearing reverses the rotation again, making
// the left side the one that needs to be reversed
motor LeftBackMotor = motor(PORT1, false);
motor LeftFrontMotor = motor(PORT2, false);
motor RightBackMotor = motor(PORT10, true);
motor RightFrontMotor = motor(PORT9, true);
motor ArmMotor = motor(PORT11);
motor Feeder = motor(PORT20);
motor IntakeMotor = motor(PORT3);
motor ShooterMotor = motor(PORT7, false);
controller Controller1 = controller();
vision::signature BLUE_OBJ (1, -3657, -2305, -2982, 6227, 12743, 9484, 2.1, 0);
vision::signature RED_OBJ (2, 6769, 10637, 8703, -2445, -577, -1511, 2.200000047683716, 0);
vision::signature YELLOW_BALL (3, -331, 1, -165, -4739, -1117, -2928, 1, 0);
vision::signature GREEN_FLAG (4, -4471, -3747, -4109, -6521, -5421, -5971, 4, 0);
vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
vision VisionSensor (PORT5, 50, BLUE_OBJ, RED_OBJ, YELLOW_BALL, GREEN_FLAG, SIG_5, SIG_6, SIG_7);

const bool isProgrammingSkillsChallenge = false;

const bool shouldRunAuton = true;
const bool justDriveStraight = false;
// Robot starting position
const bool isBlue = false;
const bool isRight = false;


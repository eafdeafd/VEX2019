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
vex::vision::signature SIG_1 (1, -47, 45, -1, -3421, -2065, -2743, 2.0999999046325684, 0);
vex::vision::signature SIG_2 (2, 7011, 9817, 8414, -631, 659, 14, 1.7999999523162842, 0);
vex::vision::signature SIG_3 (3, -3641, -2941, -3291, 8355, 14489, 11422, 1.2000000476837158, 0);
vex::vision::signature GREEN_FLAG (4, -1961, -1123, -1542, -5291, -4201, -4746, 3, 0);
vex::vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3, 0);
vex::vision VisionSensor (PORT5, 50, SIG_1, SIG_2, SIG_3, GREEN_FLAG, SIG_5, SIG_6, SIG_7);

const bool isProgrammingSkillsChallenge = false;
const bool NO_AUTON = false;
// Robot starting position
const bool isBlue = false;
const bool isFront = false;

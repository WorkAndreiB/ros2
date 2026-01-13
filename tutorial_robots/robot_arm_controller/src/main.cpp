#include "robot_arm_controller/controller.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<RobotArmController>();
  node->init();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}

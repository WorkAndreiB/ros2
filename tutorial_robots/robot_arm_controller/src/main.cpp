#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "robot_arm_controller/robot_arm_controller.hpp"

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  auto arm_controller = std::make_shared<RobotArmController>();
  arm_controller->init();

  rclcpp::spin(arm_controller);
  rclcpp::shutdown();

  return 0;
}
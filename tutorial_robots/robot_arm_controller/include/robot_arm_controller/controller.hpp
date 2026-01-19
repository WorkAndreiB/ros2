#ifndef ROBOT_ARM_CONTROLLER_CONTROLLER_HPP
#define ROBOT_ARM_CONTROLLER_CONTROLLER_HPP

#include <rclcpp/rclcpp.hpp>

#include <robot_arm_commander_cpp/commander.hpp>

#include <robot_arm_interfaces/msg/joints_target.hpp>
#include <robot_arm_interfaces/msg/named_target.hpp>

class RobotArmController : public rclcpp::Node {
public:
  RobotArmController();
  void init();

private:
  void executeArmNamedStateCommand(
      const robot_arm_interfaces::msg::NamedTarget::SharedPtr msg);

  void executeArmJointsCommand(
      const robot_arm_interfaces::msg::JointsTarget::SharedPtr msg);

  void executeGripperCommand(
      const robot_arm_interfaces::msg::NamedTarget::SharedPtr msg);

  rclcpp::Subscription<robot_arm_interfaces::msg::NamedTarget>::SharedPtr
      arm_named_state_listener_;

  rclcpp::Subscription<robot_arm_interfaces::msg::JointsTarget>::SharedPtr
      arm_joints_listener_;

  rclcpp::Subscription<robot_arm_interfaces::msg::NamedTarget>::SharedPtr
      gripper_listener_;

  std::unique_ptr<Commander> arm_commander_;
};

#endif // ROBOT_ARM_CONTROLLER_CONTROLLER_HPP
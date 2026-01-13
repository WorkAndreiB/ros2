#include "robot_arm_controller/controller.hpp"

RobotArmController::RobotArmController() : Node("robot_arm_controller") {
  arm_named_state_listener_ =
      this->create_subscription<robot_arm_interfaces::msg::NamedTarget>(
          "robot_arm/arm_command/named_target", 10,
          std::bind(&RobotArmController::executeArmNamedStateCommand, this,
                    std::placeholders::_1));

  arm_joints_listener_ =
      this->create_subscription<robot_arm_interfaces::msg::JointsTarget>(
          "robot_arm/arm_command/joints_target", 10,
          std::bind(&RobotArmController::executeArmJointsCommand, this,
                    std::placeholders::_1));

  gripper_listener_ =
      this->create_subscription<example_interfaces::msg::String>(
          "robot_arm/gripper_command", 10,
          std::bind(&RobotArmController::executeGripperCommand, this,
                    std::placeholders::_1));
}

void RobotArmController::init() {
  arm_commander_ = std::make_unique<Commander>(this->shared_from_this());
}

void RobotArmController::executeArmNamedStateCommand(
    const robot_arm_interfaces::msg::NamedTarget::SharedPtr msg) {
  RCLCPP_INFO(this->get_logger(), msg->named_target.c_str());

  arm_commander_->moveArmToNamedTarget(msg->named_target);
}

void RobotArmController::executeArmJointsCommand(
    const robot_arm_interfaces::msg::JointsTarget::SharedPtr msg) {
  arm_commander_->moveArmToJointTarget(msg->joints);
}

void RobotArmController::executeGripperCommand(
    const example_interfaces::msg::String::SharedPtr msg) {
  RCLCPP_INFO(this->get_logger(), msg->data.c_str());

  if (msg->data == "close") {
    arm_commander_->closeGripper();
  } else if (msg->data == "open") {
    arm_commander_->openGripper();
  } else {
    RCLCPP_INFO(this->get_logger(), "unknown gripper command");
  }
}
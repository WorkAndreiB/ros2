#include "robot_arm_controller/robot_arm_controller.hpp"

RobotArmController::RobotArmController() : Node("robot_arm_controller") {
  move_robot_arm_server_ =
      rclcpp_action::create_server<robot_arm_interfaces::action::MoveRobotArm>(
          this, "MoveRobotArm",
          std::bind(&RobotArmController::handle_goal, this,
                    std::placeholders::_1, std::placeholders::_2),
          std::bind(&RobotArmController::handle_cancel, this,
                    std::placeholders::_1),
          std::bind(&RobotArmController::handle_accepted, this,
                    std::placeholders::_1));

  RCLCPP_INFO(this->get_logger(),
              "RobotArmController action server has been started");
}

rclcpp_action::GoalResponse RobotArmController::handle_goal(
    const rclcpp_action::GoalUUID &uuid,
    std::shared_ptr<const robot_arm_interfaces::action::MoveRobotArm::Goal>
        goal) {
  RCLCPP_INFO(this->get_logger(), "Received goal request");
  (void)uuid;
  (void)goal;
  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse RobotArmController::handle_cancel(
    const std::shared_ptr<rclcpp_action::ServerGoalHandle<
        robot_arm_interfaces::action::MoveRobotArm>>
        goal_handle) {
  RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
  (void)goal_handle;
  return rclcpp_action::CancelResponse::ACCEPT;
}

void RobotArmController::handle_accepted(
    const std::shared_ptr<rclcpp_action::ServerGoalHandle<
        robot_arm_interfaces::action::MoveRobotArm>>
        goal_handle) {
  std::thread{
      std::bind(&RobotArmController::execute, this, std::placeholders::_1),
      goal_handle}
      .detach();
}

void RobotArmController::execute(
    const std::shared_ptr<rclcpp_action::ServerGoalHandle<
        robot_arm_interfaces::action::MoveRobotArm>>
        goal_handle) {
  RCLCPP_INFO(this->get_logger(), "Executing goal");
}

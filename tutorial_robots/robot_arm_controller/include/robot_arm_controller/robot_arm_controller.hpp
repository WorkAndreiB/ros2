#ifndef ROBOT_ARM_CONTROLLER_HPP
#define ROBOT_ARM_CONTROLLER_HPP

#include <functional>
#include <robot_arm_commander_cpp/commander.hpp>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_arm_interfaces/action/move_robot_arm.hpp"

using MoveRobotArm = robot_arm_interfaces::action::MoveRobotArm;

class RobotArmController : public rclcpp::Node
{
 public:
  RobotArmController();
  void init();

 private:
  rclcpp_action::Server<MoveRobotArm>::SharedPtr move_robot_arm_server_;

  std::unique_ptr<Commander> robot_commander_;

  rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid,
                                          std::shared_ptr<const MoveRobotArm::Goal>);

  rclcpp_action::CancelResponse handle_cancel(
      const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle);

  void handle_accepted(const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle);

  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle);

  void move_arm(const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle,
                std::function<std::string(const std::string &)> callback);
};

#endif  // ROBOT_ARM_CONTROLLER_HPP
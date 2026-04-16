#ifndef ROBOT_ARM_CONTROLLER_HPP
#define ROBOT_ARM_CONTROLLER_HPP

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "robot_arm_interfaces/action/move_robot_arm.hpp"

class RobotArmController : public rclcpp::Node {
public:
  RobotArmController();

private:
  rclcpp_action::Server<robot_arm_interfaces::action::MoveRobotArm>::SharedPtr
      move_robot_arm_server_;

  rclcpp_action::GoalResponse handle_goal(
      const rclcpp_action::GoalUUID &uuid,
      std::shared_ptr<const robot_arm_interfaces::action::MoveRobotArm::Goal>);

  rclcpp_action::CancelResponse
  handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<
                    robot_arm_interfaces::action::MoveRobotArm>>
                    goal_handle);

  void handle_accepted(const std::shared_ptr<rclcpp_action::ServerGoalHandle<
                           robot_arm_interfaces::action::MoveRobotArm>>
                           goal_handle);

  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<
                   robot_arm_interfaces::action::MoveRobotArm>>
                   goal_handle);
};

#endif // ROBOT_ARM_CONTROLLER_HPP
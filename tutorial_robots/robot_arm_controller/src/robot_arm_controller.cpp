#include "robot_arm_controller/robot_arm_controller.hpp"

#include <chrono>
#include <future>
#include <thread>

RobotArmController::RobotArmController() : Node("robot_arm_controller")
{
  move_robot_arm_server_ = rclcpp_action::create_server<MoveRobotArm>(
      this, "MoveRobotArm",
      std::bind(&RobotArmController::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
      std::bind(&RobotArmController::handle_cancel, this, std::placeholders::_1),
      std::bind(&RobotArmController::handle_accepted, this, std::placeholders::_1));

  RCLCPP_INFO(this->get_logger(), "RobotArmController action server has been started");
}

void RobotArmController::init() { robot_commander_ = std::make_unique<Commander>(this->shared_from_this()); }

rclcpp_action::GoalResponse RobotArmController::handle_goal(const rclcpp_action::GoalUUID &uuid,
                                                            std::shared_ptr<const MoveRobotArm::Goal> goal)
{
  (void)uuid;
  (void)goal;

  RCLCPP_INFO(this->get_logger(), "Received goal request");

  return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse RobotArmController::handle_cancel(
    const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle)
{
  (void)goal_handle;

  RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
  robot_commander_->stopArm();
  robot_commander_->moveArmToNamedTarget("home");

  RCLCPP_INFO(this->get_logger(), "Goal canceled, moving arm to home");

  return rclcpp_action::CancelResponse::ACCEPT;
}

void RobotArmController::handle_accepted(
    const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle)
{
  std::thread{std::bind(&RobotArmController::execute, this, std::placeholders::_1), goal_handle}.detach();
}

void RobotArmController::execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle)
{
  RCLCPP_INFO(this->get_logger(), "Executing goal");

  const auto target = goal_handle->get_goal()->target;

  if (target == "arm")
  {
    move_arm(goal_handle,
             [this](const std::string &position) { return robot_commander_->moveArmToNamedTarget(position); });
  }
  else
  {
    move_arm(goal_handle,
             [this](const std::string &position) { return robot_commander_->moveGripperToNamedTarget(position); });
  }

  RCLCPP_INFO(this->get_logger(), "Executing goal finished");
}

void RobotArmController::move_arm(const std::shared_ptr<rclcpp_action::ServerGoalHandle<MoveRobotArm>> goal_handle,
                                  std::function<std::string(const std::string &)> callback)
{
  const auto target = goal_handle->get_goal()->target;
  const auto position = goal_handle->get_goal()->position;

  auto result = std::make_shared<MoveRobotArm::Result>();
  auto feedback = std::make_shared<MoveRobotArm::Feedback>();

  std::string move_result{};

  auto move_future = std::async(std::launch::async, callback, position);

  while (rclcpp::ok() && move_future.wait_for(std::chrono::seconds(1)) != std::future_status::ready)
  {
    if (goal_handle->is_canceling())
    {
      // to check more
      result->final_status = "Goal canceled";
      goal_handle->canceled(result);
      return;
    }

    feedback->feedback_status = "Robot" + target + " moving to position: " + position;
    goal_handle->publish_feedback(feedback);
  }

  try
  {
    move_result = move_future.get();
  }
  catch (const std::exception &e)
  {
    result->final_status = std::string("Goal execution failed: ") + e.what();
    goal_handle->abort(result);
    RCLCPP_ERROR(this->get_logger(), "%s", result->final_status.c_str());
    return;
  }

  result->final_status = move_result;
  goal_handle->succeed(result);

  return;
}

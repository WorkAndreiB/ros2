#include "robot_arm_commander_cpp/commander.hpp"

#include <iostream>

Commander::Commander(const rclcpp::Node::SharedPtr& node) : node_(node)
{
  arm_ = std::make_shared<MoveGroupInterface>(node_, "arm");
  gripper_ = std::make_shared<MoveGroupInterface>(node_, "gripper");

  setScalingFactor(arm_);
  setScalingFactor(gripper_);

  if (node_)
  {
    std::cout << "Commander initialized successfully!" << std::endl;
  }
}

void Commander::setScalingFactor(std::shared_ptr<MoveGroupInterface> interface)
{
  interface->setMaxAccelerationScalingFactor(0.5);
  interface->setMaxVelocityScalingFactor(0.5);
}

std::string Commander::planAndExecute(const std::shared_ptr<MoveGroupInterface>& interface)
{
  moveit::planning_interface::MoveGroupInterface::Plan plan;

  auto robot_status = interface->plan(plan);

  if (robot_status != moveit::core::MoveItErrorCode::SUCCESS)
  {
    RCLCPP_ERROR(node_->get_logger(), "Motion planning failed:");

    return moveit::core::errorCodeToString(robot_status);
  }
  RCLCPP_INFO(node_->get_logger(), "Motion planning completed with status: %s",
              moveit::core::errorCodeToString(robot_status).c_str());

  robot_status = interface->execute(plan);

  if (robot_status != moveit::core::MoveItErrorCode::SUCCESS)
  {
    RCLCPP_ERROR(node_->get_logger(), "Motion execution failed:");
  }

  RCLCPP_INFO(node_->get_logger(), "Motion execution completed with status: %s",
              moveit::core::errorCodeToString(robot_status).c_str());

  return moveit::core::errorCodeToString(robot_status);
}

std::string Commander::moveToNamedTarget(const std::string& target,
                                         const std::shared_ptr<MoveGroupInterface>& interface)
{
  interface->setStartStateToCurrentState();
  interface->setNamedTarget(target);
  return planAndExecute(interface);
}

std::string Commander::moveArmToNamedTarget(const std::string& target) { return moveToNamedTarget(target, arm_); }

std::string Commander::moveToJointTarget(const std::vector<double>& joints,
                                         const std::shared_ptr<MoveGroupInterface>& interface)
{
  interface->setStartStateToCurrentState();
  interface->setJointValueTarget(joints);
  return planAndExecute(interface);
}

std::string Commander::moveArmToJointTarget(const std::vector<double>& joints)
{
  return moveToJointTarget(joints, arm_);
}

std::string Commander::moveArmToPositionTarget(const PositionTarget target, bool cartesian_path)
{
  tf2::Quaternion q;
  q.setRPY(target.orientation.roll, target.orientation.pitch, target.orientation.yaw);
  q = q.normalize();

  geometry_msgs::msg::PoseStamped target_pose;
  target_pose.header.frame_id = "base_link";
  target_pose.pose.position.x = target.position.x;
  target_pose.pose.position.y = target.position.y;
  target_pose.pose.position.z = target.position.z;

  target_pose.pose.orientation.x = q.getX();
  target_pose.pose.orientation.y = q.getY();
  target_pose.pose.orientation.z = q.getZ();
  target_pose.pose.orientation.w = q.getW();

  if (!cartesian_path)
  {
    arm_->setStartStateToCurrentState();
    arm_->setPoseTarget(target_pose);
    return planAndExecute(arm_);
  }
  else
  {
    // cartesian path
    std::vector<geometry_msgs::msg::Pose> waypoints;
    waypoints.push_back(target_pose.pose);

    moveit_msgs::msg::RobotTrajectory trajectory;
    double fraction = arm_->computeCartesianPath(waypoints, 0.01, trajectory);
    if (fraction == 1)
    {
      arm_->execute(trajectory);
    }
    else
    {
      RCLCPP_ERROR(node_->get_logger(), "Cartesian path execution failed");
    }
  }

  return moveit::core::errorCodeToString(moveit::core::MoveItErrorCode::FAILURE);
}

std::string Commander::moveGripperToNamedTarget(const std::string& target)
{
  return moveToNamedTarget(target, gripper_);
}

void Commander::stopArm() { arm_->stop(); }

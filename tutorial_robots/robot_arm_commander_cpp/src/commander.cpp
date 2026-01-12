#include "commander.hpp"

Commander::Commander(std::shared_ptr<rclcpp::Node> node) : node_(node) {
  arm_ = std::make_shared<MoveGroupInterface>(node_, "arm");
  gripper_ = std::make_shared<MoveGroupInterface>(node_, "gripper");

  setScallingFactor(arm_);
  setScallingFactor(gripper_);
}

void Commander::setScallingFactor(
    std::shared_ptr<MoveGroupInterface> interface) {
  interface->setMaxAccelerationScalingFactor(0.5);
  interface->setMaxVelocityScalingFactor(0.5);
}

void Commander::planAndExecute(
    const std::shared_ptr<MoveGroupInterface> &interface) {

  moveit::planning_interface::MoveGroupInterface::Plan plan;
  bool success =
      (interface->plan(plan) == moveit::core::MoveItErrorCode::SUCCESS);

  if (success) {
    interface->execute(plan);
  }
}

void Commander::moveToNamedTarget(
    const std::string &target,
    const std::shared_ptr<MoveGroupInterface> &interface) {
  interface->setStartStateToCurrentState();
  interface->setNamedTarget(target);
  planAndExecute(interface);
}

void Commander::moveArmToNamedTarget(const std::string &target) {
  moveToNamedTarget(target, arm_);
}

void Commander::moveToJointTarget(
    const std::vector<double> &joints,
    const std::shared_ptr<MoveGroupInterface> &interface) {
  interface->setStartStateToCurrentState();
  interface->setJointValueTarget(joints);
  planAndExecute(interface);
}

void Commander::moveArmToJointTarget(const std::vector<double> &joints) {
  moveToJointTarget(joints, arm_);
}

void Commander::moveArmToPositionTarget(const PositionTarget target,
                                        bool cartesian_path) {
  tf2::Quaternion q;
  q.setRPY(target.orientation.roll, target.orientation.pitch,
           target.orientation.yaw);
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

  if (!cartesian_path) {
    arm_->setStartStateToCurrentState();
    arm_->setPoseTarget(target_pose);
    planAndExecute(arm_);
  } else {
    // cartesian path
    std::vector<geometry_msgs::msg::Pose> waypoints;
    waypoints.push_back(target_pose.pose);

    moveit_msgs::msg::RobotTrajectory trajectory;
    double fraction = arm_->computeCartesianPath(waypoints, 0.01, trajectory);
    if (fraction == 1) {
      arm_->execute(trajectory);
    }
  }
}

void Commander::openGripper() { moveToNamedTarget("open", gripper_); }

void Commander::openGripper() { moveToNamedTarget("closed", gripper_); }

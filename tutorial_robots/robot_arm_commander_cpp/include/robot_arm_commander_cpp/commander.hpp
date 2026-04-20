#ifndef ROBOT_ARM_COMMANDER_COMMANDER_HPP
#define ROBOT_ARM_COMMANDER_COMMANDER_HPP

#include <moveit/move_group_interface/move_group_interface.hpp>
#include <rclcpp/rclcpp.hpp>
#include <string>

using MoveGroupInterface = moveit::planning_interface::MoveGroupInterface;

struct Point3D
{
  double x{0.0};
  double y{0.0};
  double z{0.0};
};

/*
            +Z (up)
             |
             |
             o----> +X (forward)
            /
           /
      +Y (left)

  Positive rotations follow the right-hand rule:

  roll  (+) : rotation about +X  (tilt left/right)
  pitch (+) : rotation about +Y  (nose up/down)
  yaw   (+) : rotation about +Z  (turn left/right)

  Notes:
  - Units: radians
*/
struct Orientation
{
  double roll{0.0};
  double pitch{0.0};
  double yaw{0.0};
};

struct PositionTarget
{
  Point3D position{};
  Orientation orientation{};
};

class Commander
{
 public:
  Commander(const rclcpp::Node::SharedPtr &node);

  std::string moveGripperToNamedTarget(const std::string &target);
  std::string moveArmToNamedTarget(const std::string &target);
  std::string moveArmToJointTarget(const std::vector<double> &joints);
  std::string moveArmToPositionTarget(const PositionTarget position, bool cartesian_path = false);

  void stopArm();

 private:
  std::string moveToNamedTarget(const std::string &target, const std::shared_ptr<MoveGroupInterface> &interface);
  std::string moveToJointTarget(const std::vector<double> &joints,
                                const std::shared_ptr<MoveGroupInterface> &interface);

  void setScalingFactor(std::shared_ptr<MoveGroupInterface> interface);
  std::string planAndExecute(const std::shared_ptr<MoveGroupInterface> &interface);

  rclcpp::Node::SharedPtr node_;
  std::shared_ptr<MoveGroupInterface> arm_;
  std::shared_ptr<MoveGroupInterface> gripper_;
};

#endif  // ROBOT_ARM_COMMANDER_COMMANDER_HPP

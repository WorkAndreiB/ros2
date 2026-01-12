#include <moveit/move_group_interface/move_group_interface.hpp>
#include <rclcpp/rclcpp.hpp>

#include <vector>

#include "commander.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<rclcpp::Node>("test_moveit");

  Commander commander(node);

  rclcpp::executors::SingleThreadedExecutor executor;

  executor.add_node(node);

  auto spinner = std::thread([&executor]() { executor.spin(); });

  commander.moveArmToNamedTarget("pose_1");

  //*********************************************************** */
  // joints
  commander.moveArmToJointTarget(
      std::vector<double>{1.5, 0.5, 0.0, 1.5, 0.0, -0.7});

  //*****************/
  //
  //   commander.moveArmToNamedTarget("home");
  commander.moveArmToPositionTarget(
      PositionTarget{Point3D{0.7, 0.0, 0.4}, Orientation{3.14, 0.0, 0.0}},
      true);

  rclcpp::shutdown();
  spinner.join();
  return 0;
}

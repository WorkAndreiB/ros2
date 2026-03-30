#include "action_server_cpp/add_until_server.hpp"
#include <memory>
#include <rclcpp/rclcpp.hpp>

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<AddUntilServer>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
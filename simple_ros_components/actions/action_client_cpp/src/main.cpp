#include "action_client_cpp/add_until_client.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<AddUntilClient>();
  node->send_goal(10, 0.2);

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
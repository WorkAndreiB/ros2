#include "action_client_cpp/add_until_client.hpp"

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<AddUntilClient>();
  node->declare_parameter<int>("target_number", 10);
  node->declare_parameter<double>("period", 0.2);

  RCLCPP_INFO(node->get_logger(),
              "Starting AddUntilClient with target_number=%ld and period=%.2f",
              node->get_parameter("target_number").as_int(),
              node->get_parameter("period").as_double());

  const auto target_number = node->get_parameter("target_number").as_int();
  const auto period = node->get_parameter("period").as_double();

  node->send_goal(target_number, period);

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
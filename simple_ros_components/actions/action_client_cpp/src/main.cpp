#include "action_client_cpp/add_until_client.hpp"

#include <memory>
#include <rclcpp/rclcpp.hpp>

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);

  auto node = std::make_shared<AddUntilClient>();

  // use parameters to set the goal values, with defaults if the parameters are
  // not set use exact types for the parameters, which are defined in the action
  // definition, to avoid issues with type conversions.
  node->declare_parameter<AddUntil::Goal::_target_number_type>("target_number",
                                                               10);
  node->declare_parameter<AddUntil::Goal::_period_type>("period", 0.2);

  RCLCPP_INFO(node->get_logger(),
              "Starting AddUntilClient with target_number=%ld and period=%.2f",
              node->get_parameter("target_number").as_int(),
              node->get_parameter("period").as_double());

  const AddUntil::Goal::_target_number_type target_number =
      node->get_parameter("target_number").as_int();
  const AddUntil::Goal::_period_type period =
      node->get_parameter("period").as_double();

  node->send_goal(target_number, period);

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
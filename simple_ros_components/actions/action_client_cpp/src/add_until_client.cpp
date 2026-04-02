#include "action_client_cpp/add_until_client.hpp"

AddUntilClient::AddUntilClient() : Node("add_until_client") {
  add_until_client_ = rclcpp_action::create_client<AddUntil>(this, "AddUntil");
}

void AddUntilClient::send_goal(int target_number, double period) {
  // wait for server to be available
  add_until_client_->wait_for_action_server();

  // create goal
  auto goal_msg = AddUntil::Goal();
  goal_msg.target_number = target_number;
  goal_msg.period = period;

  auto send_goal_options = rclcpp_action::Client<AddUntil>::SendGoalOptions{};

  // define result callback
  // can also use std::bind instead of lambda
  send_goal_options.result_callback =
      [this](const rclcpp_action::ClientGoalHandle<AddUntil>::WrappedResult
                 &result) {
        RCLCPP_INFO(this->get_logger(), "Result received: %ld",
                    result.result->sum);
      };

  send_goal_options.goal_response_callback =
      [this](const rclcpp_action::ClientGoalHandle<AddUntil>::SharedPtr
                 &goal_handle) {
        if (!goal_handle) {
          RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
        } else {
          RCLCPP_INFO(this->get_logger(),
                      "Goal accepted by server, waiting for result");
        }
      };

  send_goal_options.feedback_callback =
      [this](const rclcpp_action::ClientGoalHandle<AddUntil>::SharedPtr
                 &goal_handle,
             const std::shared_ptr<const AddUntil::Feedback> feedback) {
        (void)goal_handle;
        RCLCPP_INFO(this->get_logger(), "Received feedback: %ld",
                    feedback->intermediate_sum);
      };

  // send goal
  RCLCPP_INFO(this->get_logger(), "Sending goal");
  add_until_client_->async_send_goal(goal_msg, send_goal_options);
}

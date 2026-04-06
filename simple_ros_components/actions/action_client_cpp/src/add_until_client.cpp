#include "action_client_cpp/add_until_client.hpp"

AddUntilClient::AddUntilClient() : Node("add_until_client") {
  add_until_client_ = rclcpp_action::create_client<AddUntil>(this, "AddUntil");
}

void AddUntilClient::send_goal(
    AddUntil::Goal::_target_number_type target_number,
    AddUntil::Goal::_period_type period) {
  // wait for server to be available. if the server is not available after 5
  // seconds, shutdown the client.
  if (!add_until_client_->wait_for_action_server(std::chrono::seconds(5))) {
    RCLCPP_ERROR(this->get_logger(),
                 "Action server not available after waiting for 5 seconds");
    rclcpp::shutdown();
    return;
  }

  // create goal
  auto goal_msg = AddUntil::Goal();
  goal_msg.target_number = target_number;
  goal_msg.period = period;

  auto send_goal_options = rclcpp_action::Client<AddUntil>::SendGoalOptions{};

  // define result callback
  // can also use std::bind() instead of lambda
  send_goal_options.result_callback =
      [this](const rclcpp_action::ClientGoalHandle<AddUntil>::WrappedResult
                 &result) {
        auto status = result.code;
        if (status == rclcpp_action::ResultCode::SUCCEEDED) {
          RCLCPP_INFO(this->get_logger(), "Goal succeeded");
        } else if (status == rclcpp_action::ResultCode::ABORTED) {
          RCLCPP_WARN(this->get_logger(), "Goal was aborted");
        } else if (status == rclcpp_action::ResultCode::CANCELED) {
          RCLCPP_WARN(this->get_logger(), "Goal was canceled");
        } else {
          RCLCPP_ERROR(this->get_logger(), "Unknown result code");
        }

        RCLCPP_INFO(this->get_logger(), "Result received: %ld",
                    result.result->sum);

        // shutdown client after receiving result
        rclcpp::shutdown();
      };

  send_goal_options.goal_response_callback =
      [this](const rclcpp_action::ClientGoalHandle<AddUntil>::SharedPtr
                 &goal_handle) {
        if (!goal_handle) {
          RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");

          // shutdown client since the goal was rejected.
          rclcpp::shutdown();
          return;
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

        // Simulate canceling the goal based on feedback
        // if (feedback->intermediate_sum > 20) {
        //   RCLCPP_INFO(this->get_logger(),
        //               "Canceling goal because intermediate sum is too high");
        //   add_until_client_->async_cancel_goal(goal_handle);
        // }
      };

  // send goal
  RCLCPP_INFO(this->get_logger(), "Sending goal");
  add_until_client_->async_send_goal(goal_msg, send_goal_options);
}

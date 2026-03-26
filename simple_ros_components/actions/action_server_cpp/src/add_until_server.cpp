#include "action_interfaces/action/add_until.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using AddUntil = action_interfaces::action::AddUntil;

class AddUntilServer : public rclcpp::Node {
public:
  AddUntilServer() : Node("add_until_server") {

    add_until_server_ = rclcpp_action::create_server<AddUntil>(
        this, "AddUntil",
        std::bind(&AddUntilServer::handle_goal, this, std::placeholders::_1,
                  std::placeholders::_2),
        std::bind(&AddUntilServer::handle_cancel, this, std::placeholders::_1),
        std::bind(&AddUntilServer::handle_accepted, this,
                  std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "AddUntilServer has been started.");
  }

private:
  rclcpp_action::Server<AddUntil>::SharedPtr add_until_server_;

  // Called when a new goal request is received.
  rclcpp_action::GoalResponse
  handle_goal(const rclcpp_action::GoalUUID &uuid,
              std::shared_ptr<const AddUntil::Goal>) {
    RCLCPP_INFO(this->get_logger(), "Received goal");
    (void)uuid;
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
  }

  // Called when a client requests cancellation of a running goal.
  rclcpp_action::CancelResponse
  handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
                    goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
    (void)goal_handle;
    return rclcpp_action::CancelResponse::ACCEPT;
  }

  // Called once a goal has been accepted.
  void handle_accepted(
      const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
          goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Accepted goal");
    std::thread{
        std::bind(&AddUntilServer::execute, this, std::placeholders::_1),
        goal_handle}
        .detach();
  }

  // Main goal execution loop.
  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
                   goal_handle) {
    RCLCPP_INFO(this->get_logger(), "Executing goal");

    const auto target_number = goal_handle->get_goal()->target_number;
    const auto period = goal_handle->get_goal()->period;

    rclcpp::Rate loop_rate(1.0 / period);

    auto feedback = std::make_shared<AddUntil::Feedback>();
    auto result = std::make_shared<AddUntil::Result>();

    int counter = 0;

    for (int i = 1; (i <= target_number) && rclcpp::ok(); ++i) {
      counter++;

      // If cancellation is requested, return the current progress and stop.
      if (goal_handle->is_canceling()) {
        result->sum = counter;
        goal_handle->canceled(result);
        RCLCPP_INFO(this->get_logger(), "Goal canceled");
        return;
      }

      // Publish progress after each increment.
      feedback->intermediate_sum = counter;
      goal_handle->publish_feedback(feedback);

      loop_rate.sleep();
    }

    // Report successful completion with final sum.
    if (rclcpp::ok()) {
      result->sum = counter;
      goal_handle->succeed(result);
      RCLCPP_INFO(this->get_logger(), "Goal succeeded");
    }
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<AddUntilServer>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
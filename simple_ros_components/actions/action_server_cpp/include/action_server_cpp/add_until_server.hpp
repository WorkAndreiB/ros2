#include "action_interfaces/action/add_until.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using AddUntil = action_interfaces::action::AddUntil;

class AddUntilServer : public rclcpp::Node {
public:
  AddUntilServer();

private:
  rclcpp_action::Server<AddUntil>::SharedPtr add_until_server_;

  // Called when a new goal request is received.
  rclcpp_action::GoalResponse
  handle_goal(const rclcpp_action::GoalUUID &uuid,
              std::shared_ptr<const AddUntil::Goal>);

  // Called when a client requests cancellation of a running goal.
  rclcpp_action::CancelResponse
  handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
                    goal_handle);

  // Called once a goal has been accepted.
  void handle_accepted(
      const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
          goal_handle);

  // Main goal execution loop.
  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
                   goal_handle);
};
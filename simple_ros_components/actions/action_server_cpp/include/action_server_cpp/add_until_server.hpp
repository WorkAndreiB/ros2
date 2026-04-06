#ifndef ADD_UNTIL_SERVER_HPP_
#define ADD_UNTIL_SERVER_HPP_

#include "action_interfaces/action/add_until.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using AddUntil = action_interfaces::action::AddUntil;

/**
 * @brief ROS 2 action server node for the AddUntil action.
 *
 * This node receives AddUntil goals, validates input constraints, executes the
 * summation loop, publishes intermediate feedback, and reports the final
 * result.
 */
class AddUntilServer : public rclcpp::Node {
public:
  AddUntilServer();

private:
  rclcpp_action::Server<AddUntil>::SharedPtr add_until_server_;

  /**
   * @brief Handle and validate a newly received goal request.
   *
   * Called when a new goal request is received.
   *
   * A goal is accepted only when:
   * - `target_number` is positive
   * - `target_number` is even
   * - `period` is greater than 0 and less than or equal to 10 seconds
   *
   * @param uuid Unique identifier for the incoming goal.
   * @param goal Goal payload received from the client.
   * @return `rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE` for valid goals,
   * otherwise `rclcpp_action::GoalResponse::REJECT`.
   */
  rclcpp_action::GoalResponse
  handle_goal(const rclcpp_action::GoalUUID &uuid,
              std::shared_ptr<const AddUntil::Goal>);

  /**
   * @brief Handle cancel request for a running goal.
   *
   * Called when a client requests cancellation of a running goal.
   *
   * @param goal_handle Handle to the goal requested for cancellation.
   * @return `rclcpp_action::CancelResponse::ACCEPT`.
   */
  rclcpp_action::CancelResponse
  handle_cancel(const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
                    goal_handle);

  /**
   * @brief Dispatch execution for an accepted goal.
   *
   * Called once a goal has been accepted.
   *
   * The goal is executed on a detached worker thread so the executor thread is
   * not blocked.
   *
   * @param goal_handle Handle to the accepted goal.
   */
  void handle_accepted(
      const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
          goal_handle);

  /**
   * @brief Execute the AddUntil summation loop for a goal.
   *
   * Publishes intermediate sums as feedback, supports goal cancellation, and
   * reports the final sum on success.
   *
   * @param goal_handle Handle to the goal being executed.
   */
  void execute(const std::shared_ptr<rclcpp_action::ServerGoalHandle<AddUntil>>
                   goal_handle);
};

#endif // ADD_UNTIL_SERVER_HPP_

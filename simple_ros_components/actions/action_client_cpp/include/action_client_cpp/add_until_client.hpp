#ifndef ADD_UNTIL_CLIENT_HPP_
#define ADD_UNTIL_CLIENT_HPP_

#include "action_interfaces/action/add_until.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using AddUntil = action_interfaces::action::AddUntil;

/**
 * @brief ROS 2 action client node for the AddUntil action.
 *
 * This node connects to the AddUntil action server, sends goal requests, and
 * handles goal response, feedback, and result callbacks.
 */
class AddUntilClient : public rclcpp::Node {
public:
  AddUntilClient();

  /**
   * @brief Send an AddUntil goal to the action server.
   *
   * Waits for the action server to be available, builds a goal message, and
   * dispatches it asynchronously with callbacks for goal acceptance, feedback,
   * and final result.
   *
   * @param target_number Upper bound for the summation sequence.
   * @param period Delay in seconds between server-side feedback updates.
   */
  void send_goal(int target_number, double period);

private:
  /// Action client instance used to communicate with the AddUntil server.
  rclcpp_action::Client<AddUntil>::SharedPtr add_until_client_;
};

#endif // ADD_UNTIL_CLIENT_HPP_
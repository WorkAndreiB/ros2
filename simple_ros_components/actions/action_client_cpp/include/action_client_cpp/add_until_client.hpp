#ifndef ADD_UNTIL_CLIENT_HPP_
#define ADD_UNTIL_CLIENT_HPP_

#include "action_interfaces/action/add_until.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

using AddUntil = action_interfaces::action::AddUntil;

class AddUntilClient : public rclcpp::Node {
public:
  AddUntilClient();
  void send_goal(int target_number, double period);

private:
  rclcpp_action::Client<AddUntil>::SharedPtr add_until_client_;
};

#endif // ADD_UNTIL_CLIENT_HPP_
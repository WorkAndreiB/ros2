#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

class RobotNewsStationListener : public rclcpp::Node
{
public:
    RobotNewsStationListener();

private:

    void ReadNews(const example_interfaces::msg::String::SharedPtr msg);

    rclcpp::Subscription<example_interfaces::msg::String>::SharedPtr listener_;
};


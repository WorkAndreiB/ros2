#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

class RobotNewsStation : public rclcpp::Node
{
public:
    RobotNewsStation();

private:

    void PublishNews();

    rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int counter_;
};


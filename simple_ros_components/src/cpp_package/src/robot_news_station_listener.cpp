#include "cpp_package/robot_news_station_listener.hpp"
#include <rclcpp/rclcpp.hpp>

RobotNewsStationListener::RobotNewsStationListener() : Node("robot_news_station_listener")
{
    listener_ = this->create_subscription<example_interfaces::msg::String>("robot_news", 10, std::bind(&RobotNewsStationListener::ReadNews, this, std::placeholders::_1));
    RCLCPP_INFO(this->get_logger(), "Robot news station listener");
}

void RobotNewsStationListener::ReadNews(const example_interfaces::msg::String::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), msg->data.c_str());
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<RobotNewsStationListener>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
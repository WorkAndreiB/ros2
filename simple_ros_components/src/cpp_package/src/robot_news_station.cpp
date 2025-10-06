#include "cpp_package/robot_news_station.hpp"

RobotNewsStation::RobotNewsStation() : Node("robot_news_station"), counter_(0)
{
    publisher_ = this->create_publisher<example_interfaces::msg::String>("robot_news", 10);
    timer_ = this->create_wall_timer(std::chrono::seconds(1), std::bind(&RobotNewsStation::PublishNews, this));
    RCLCPP_INFO(this->get_logger(), "Robot News station");
}

void RobotNewsStation::PublishNews()
{
   auto msg = example_interfaces::msg::String();
   msg.data = std::string("Hi, this is R2D2 from the robot news station with message: ") + std::to_string(counter_);
   publisher_->publish(msg);
   counter_++;
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<RobotNewsStation>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
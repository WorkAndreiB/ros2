#include "cpp_package/number_counter.hpp"

NumberCounter::NumberCounter() : Node("number_counter"), sum_(0)
{
    subscriber_ = this->create_subscription<example_interfaces::msg::Int64>("number", 1, std::bind(&NumberCounter::ReadNumber, this, std::placeholders::_1));
    publisher_ = this->create_publisher<example_interfaces::msg::Int64>("number_count", 1);
}

void NumberCounter::ReadNumber(const example_interfaces::msg::Int64::SharedPtr msg )
{
    RCLCPP_INFO(this->get_logger(), "Received number %ld: ", msg->data);
    sum_ += msg->data;
    auto sum_msg = example_interfaces::msg::Int64();
    sum_msg.data = sum_;
    publisher_->publish(sum_msg);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<NumberCounter>();

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}
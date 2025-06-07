#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from example_interfaces.msg import String


class RobotNewsStationListener(Node):
    def __init__(self):
        super().__init__("robot_news_station_listener")
        self.subscriber_ = self.create_subscription(String, "robot_news", self.callback_robot_news, 10)
    
    def callback_robot_news(self, msg: String):
        self.get_logger().info(f"Hi C3P0, I hear you: {msg.data}")

def main(args=None):
    rclpy.init(args=args)

    node = RobotNewsStationListener()

    rclpy.spin(node)

    rclpy.shutdown()


if __name__ == "__main__":
    main()       
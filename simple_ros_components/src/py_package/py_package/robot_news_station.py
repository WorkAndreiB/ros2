#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from example_interfaces.msg import String

class RobotNewsStation(Node):
    def __init__(self):
        super().__init__("robot_news_station")
        self.name_ = "C3PO"
        self.publishers_ = self.create_publisher(String, "robot_news", 10)
        self.timer_ = self.create_timer(1,self.publish_news)
        self.contor_ = 0

        self.get_logger().info("Hello. News station robot C3PO is ready! ")


    def publish_news(self):
        msg = String()
        msg.data = f"Hello this is {self.name_} from robot news station with message: {self.contor_}"
        self.publishers_.publish(msg=msg)
        self.contor_ += 1


def main(args=None):
    rclpy.init(args=args)

    node = RobotNewsStation()

    rclpy.spin(node)

    rclpy.shutdown()


if __name__ == "__main__":
    main()        
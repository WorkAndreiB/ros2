#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from example_interfaces.msg import Int64 
import random

class NumberPublisher(Node):
    """
    Custom Ros2 node that send a random number between 1 and 10, to a topic called 'number' every 2 sec
    """
      
    def __init__(self):
        super().__init__("number_publisher")

        self.publisher_ = self.create_publisher(Int64, "number", 1)
        self.timer_ = self.create_timer(2, self.publish_number)
        self.get_logger().info("Number publisher is ready")

    def publish_number(self):
        msg = Int64()
        msg.data = random.randint(1, 10)
        self.publisher_.publish(msg=msg)

    
def main(args=None):
    rclpy.init(args=args)

    node = NumberPublisher()

    rclpy.spin(node)

    rclpy.shutdown()


if __name__ == "__main__":
    main()        
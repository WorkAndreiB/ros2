#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from example_interfaces.msg import Int64 

class NumberCounter(Node):
    def __init__(self):
        super().__init__("number_counter")
        self.subscriber_ = self.create_subscription(Int64, "number", self.callback_number_publish, 1)
        self.publisher_ = self.create_publisher(Int64, "number_count", 1)
        self.sum_ = 0
        self.get_logger().info("Number counter is ready: ")


    def callback_number_publish(self, msg:Int64):
        self.get_logger().info(f"Received number:{msg.data}")
        self.sum_ += msg.data
        new_msg = Int64()
        new_msg.data = self.sum_
        self.publisher_.publish(new_msg)

def main(args=None):
    rclpy.init(args=args)

    node = NumberCounter()

    rclpy.spin(node)

    rclpy.shutdown()


if __name__ == "__main__":
    main()    
#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

class MyNode(Node):
    def __init__(self):
        super().__init__("py_test")
        self.contor = 0
        self.get_logger().info("Hello world! oop")
        self.create_timer(1.0, self.timer_callback)

    def timer_callback(self):
        self.get_logger().info(f"Hello from timer: {self.contor}")
        self.contor += 1
    

def main(args=None):
    #first thing is to init
    rclpy.init(args=args)

    node = MyNode()

    rclpy.spin(node)

    #shutdown at the end
    rclpy.shutdown()



if __name__ == "__main__":
    main()
#!/usr/bin/env python3

import rclpy
from rclpy.node import Node

def main(args=None):
    #first thing is to init
    rclpy.init(args=args)

    node = Node("py_test")
    node.get_logger().info("hello word!")

    #shutdown at the end
    rclpy.shutdown()



if __name__ == "__main__":
    main()
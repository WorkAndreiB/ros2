#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from action_interfaces.action import AddUntil
from rclpy.action.server import ServerGoalHandle
from rclpy.action import ActionServer

import time

class AddUntilServer(Node):
    def __init__(self):
        super().__init__("add_until_action_server")
        
        self.add_until_server_ = ActionServer(node=self, 
                                              action_type=AddUntil, 
                                              action_name="AddUntil", 
                                              execute_callback=self.add_until_callback)
        
        self.get_logger().info("AddUntil action server has been started")
    
    def add_until_callback(self, goal_handle: ServerGoalHandle):
        number = goal_handle.request.target_number
        period = goal_handle.request.period

        #execute action
        self.get_logger().info("Executing...")
        sum = 0
        for i in range(number):
            sum += i
            self.get_logger().info(f"Sum = {sum}")
            time.sleep(period)

        #set goal final state
        goal_handle.succeed()

        #send result
        result = AddUntil.Result()
        result.sum = sum

        return result
        

def main(args=None):
    #first thing is to init
    rclpy.init(args=args)

    node = AddUntilServer()

    rclpy.spin(node)

    #shutdown at the end
    rclpy.shutdown()


if __name__ == "__main__":
    main()
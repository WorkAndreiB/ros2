#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from action_interfaces.action import AddUntil
from rclpy.action.server import ServerGoalHandle
from rclpy.action import ActionServer, GoalResponse

import time


class AddUntilServer(Node):
    def __init__(self):
        super().__init__("add_until_action_server")

        self.add_until_server_ = ActionServer(
            node=self,
            action_type=AddUntil,
            action_name="AddUntil",
            goal_callback=self.goal_callback,
            execute_callback=self.add_until_callback,
        )

        self.get_logger().info("AddUntil action server has been started")

    def add_until_callback(self, goal_handle: ServerGoalHandle):
        number = goal_handle.request.target_number
        period = goal_handle.request.period

        # execute action
        self.get_logger().info("Executing...")
        sum = 0
        for i in range(number):
            sum += i
            self.get_logger().info(f"Sum = {sum}")
            time.sleep(period)

        # set goal final state
        goal_handle.succeed()

        # send result
        result = AddUntil.Result()
        result.sum = sum

        return result

    # first callback to be called when goal is received
    # if goal is accepted, then the add_until_callback is executed
    def goal_callback(self, goal_request: AddUntil.Goal):
        self.get_logger().info("Goal receievd")

        # Validate goal request
        if goal_request.target_number <= 0:
            self.get_logger().info("Reject target number <= 0")
            return GoalResponse.REJECT

        if goal_request.target_number % 2 == 1:
            self.get_logger().info(
                f"Reject odd target number {goal_request.target_number}"
            )
            return GoalResponse.REJECT

        self.get_logger().info("Accept tarrget number")
        return GoalResponse.ACCEPT


def main(args=None):
    # first thing is to init
    rclpy.init(args=args)

    node = AddUntilServer()

    rclpy.spin(node)

    # shutdown at the end
    rclpy.shutdown()


if __name__ == "__main__":
    main()

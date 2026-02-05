#!/usr/bin/env python3

"""
ROS 2 Action Server for the AddUntil action.

This node exposes an ActionServer named "AddUntil" that:
- Validates incoming goals (goal_callback)
- Executes accepted goals by incrementally computing a running sum (execute_callback)
- Publishes feedback (intermediate_sum) during execution
- Returns a final result (sum) when completed
- Randomly simulates failures by aborting the goal (for testing client behavior)

Action interface (conceptually):
- Goal:   target_number (int), period (float)
- Feedback: intermediate_sum (int)
- Result: sum (int)
"""

import rclpy
from rclpy.node import Node
from action_interfaces.action import AddUntil
from rclpy.action.server import ServerGoalHandle
from rclpy.action import ActionServer, GoalResponse

import time
import random


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
        """
        Execute callback for the action server.

        This runs after the goal has been accepted. It:
        - Reads the goal request (target_number, period)
        - Computes the running sum
        - Publishes feedback each iteration
        - Randomly aborts the goal (simulated failure)
        - Otherwise marks the goal as succeeded and returns the result

        Args:
            goal_handle (ServerGoalHandle): Handle containing the goal request and
            methods to publish feedback and set final state.

        Returns:
            AddUntil.Result: Final result object containing the computed sum.
        """
        number = goal_handle.request.target_number
        period = goal_handle.request.period

        # execute action
        self.get_logger().info("Executing...")
        feedback = AddUntil.Feedback()
        sum = 0
        for i in range(number):
            sum += i
            self.get_logger().info(f"Sum = {sum}")

            # Publish feedback
            feedback.intermediate_sum = sum
            goal_handle.publish_feedback(feedback)

            # Simulate execution
            time.sleep(period)

            # Simulate random fail events
            num = random.randrange(1, 100)
            if num <= 1:
                goal_handle.abort()
                result = AddUntil.Result()
                result.sum = sum
                return result

        # Mark goal as successfully completed
        goal_handle.succeed()

        # Return result
        result = AddUntil.Result()
        result.sum = sum

        return result

    def goal_callback(self, goal_request: AddUntil.Goal):
        """
        Goal validation callback.

        Called immediately when a goal is received (before execution starts).
        Decide whether to accept or reject the goal.

        Validation rules:
        - Reject if target_number <= 0
        - Reject if target_number is odd
        - Reject if period is not in [0, 10]
        - Otherwise accept

        Args:
            goal_request (AddUntil.Goal): Incoming goal request.

        Returns:
            GoalResponse: ACCEPT or REJECT depending on validation.
        """

        self.get_logger().info("Goal received")

        # Validate goal request
        if goal_request.target_number <= 0:
            self.get_logger().info("Reject target number <= 0")
            return GoalResponse.REJECT

        if goal_request.target_number % 2 == 1:
            self.get_logger().info(
                f"Reject odd target number {goal_request.target_number}"
            )
            return GoalResponse.REJECT

        if goal_request.period < 0 or goal_request.period > 10:
            self.get_logger().info("Reject period not in [0,10]")
            return GoalResponse.REJECT

        self.get_logger().info("Accept target number")
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

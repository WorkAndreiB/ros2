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
from rclpy.action import ActionServer, GoalResponse, CancelResponse
from rclpy.executors import MultiThreadedExecutor
from rclpy.callback_groups import ReentrantCallbackGroup
from rcl_interfaces.msg import ParameterDescriptor
from rcl_interfaces.msg import SetParametersResult

import time
import random
import threading


class AddUntilServer(Node):
    def __init__(self):
        super().__init__("add_until_action_server")
        self.goal_handle_: ServerGoalHandle = None
        self.goal_lock_ = threading.Lock()
        self.add_until_server_ = ActionServer(
            node=self,
            action_type=AddUntil,
            action_name="AddUntil",
            goal_callback=self.goal_callback,
            execute_callback=self.add_until_callback,
            cancel_callback=self.cancel_callback,
            callback_group=ReentrantCallbackGroup(),
        )

        self.valid_goal_policy_options_ = {"paralel", "reject", "preempt"}
        goal_policy_descriptor = ParameterDescriptor(
            description="Goal policy option",
            additional_constraints="Valid options: paralel, reject, preempt",
        )
        self.declare_parameter(
            name="goal_policy", value="paralel", descriptor=goal_policy_descriptor
        )
        self.goal_policy_ = self.get_parameter("goal_policy").value

        # Validate policy at startup
        if self.goal_policy_ not in self.valid_goal_policy_options_:
            raise ValueError(
                f"Invalid goal policy. Use only {self.valid_goal_policy_options_}"
            )

        # Register callback to validate policy at runtime changes
        self.add_on_set_parameters_callback(self.validate_goal_policy)

        self.get_logger().info("AddUntil action server has been started")
        self.get_logger().info(f"goal_policy: {self.goal_policy_}")

    def validate_goal_policy(self, params):
        """
         Validate and apply updates to the `goal_policy` ROS parameter.

        Args:
            params : list[rclpy.parameter.Parameter]
            The list of parameters requested to be changed in this update transaction.

        Returns:
            rcl_interfaces.msg.SetParametersResult
            Indicates whether the parameter update should be accepted or rejected.
        """
        for param in params:
            if param.name == "goal_policy":
                if param.value not in self.valid_goal_policy_options_:
                    return SetParametersResult(
                        successful=False,
                        reason=f"Invalid goal policy. Use only {self.valid_goal_policy_options_}",
                    )

        return SetParametersResult(successful=True)

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
        with self.goal_lock_:
            self.goal_handle_ = goal_handle

        number = goal_handle.request.target_number
        period = goal_handle.request.period

        # execute action
        self.get_logger().info("Executing...")

        result = AddUntil.Result()
        feedback = AddUntil.Feedback()

        print(f"About to set sum = 0 from thread:{threading.get_ident()}")
        sum = 0
        for i in range(number):

            # check if request is canceled
            if goal_handle.is_cancel_requested:
                self.get_logger().info("Canceling goal")
                goal_handle.canceled()
                result.sum = sum
                return result

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

            if not goal_handle.is_active:
                self.get_logger().info("Aborting current goal")
                print(f"Return {sum} from thread {threading.get_ident()}")
                result.sum = sum
                print(f"Return {result.sum} from thread {threading.get_ident()}")
                return result

        # Mark goal as successfully completed
        goal_handle.succeed()
        # Return result
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

        # Check goal policy. reject, run in paralel or preempt new goal
        with self.goal_lock_:
            # Check against the first received goal
            if self.goal_handle_ is not None:
                # Check goal policy and if there is an active goal request
                if self.goal_policy_ == "reject" and self.goal_handle_.is_active:
                    self.get_logger().info(
                        "A goal is already active; Rejecting new goal"
                    )
                    return GoalResponse.REJECT

                elif self.goal_policy_ == "preempt" and self.goal_handle_.is_active:
                    self.get_logger().info("Abort current goal and accept new goal")
                    self.goal_handle_.abort()

        self.get_logger().info("Accept goal")
        return GoalResponse.ACCEPT

    def cancel_callback(self, goal_handle: ServerGoalHandle):
        self.get_logger().info("Reveived cancel request")
        return CancelResponse.ACCEPT


def main(args=None):
    # first thing is to init
    rclpy.init(args=args)

    node = AddUntilServer()

    rclpy.spin(node, MultiThreadedExecutor())

    # shutdown at the end
    rclpy.shutdown()


if __name__ == "__main__":
    main()

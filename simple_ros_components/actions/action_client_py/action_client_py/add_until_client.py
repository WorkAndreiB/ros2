#!/usr/bin/env python3

import rclpy
from rclpy.node import Node
from rclpy.action import ActionClient
from action_interfaces.action import AddUntil
from rclpy.action.client import ClientGoalHandle, GoalStatus


class AddUntilClientNode(Node):
    def __init__(self):
        super().__init__("add_until_client_node")
        self.add_until_client_ = ActionClient(
            node=self, action_type=AddUntil, action_name="AddUntil"
        )

        # Declare ROS parameters with default values
        self.declare_parameter("target_number", 10)
        self.declare_parameter("period", 0.5)

    def send_goal(self, target_number, period):
        """
        Send a goal to the AddUntil action server.

        Args:
            target_number (int): The number up to which values will be added.
            period (float): Time delay (in seconds) between additions.
        """

        # wait for server to be available
        is_server_ready_ = self.add_until_client_.wait_for_server(5)

        if not is_server_ready_:
            self.get_logger().error("Server is not ready...")
            return

        self.get_logger().info("Server is ready...")

        # Populate the goal message
        goal = AddUntil.Goal()
        goal.target_number = target_number
        goal.period = period

        self.get_logger().info("Sending command to server...")

        # Send the goal asynchronously
        response_handle = self.add_until_client_.send_goal_async(
            goal=goal, feedback_callback=self.goal_feedback_callback
        )

        # Register callback for goal acceptance/rejection
        response_handle.add_done_callback(self.goal_response_callback)

    def goal_feedback_callback(self, feedback):
        """
        Callback function for receiving feedback from the action server.

        Args:
            feedback (AddUntil.FeedbackMessage): Feedback message containing
            the current intermediate sum.
        """
        self.get_logger().info(f"Feedback: {feedback.feedback.intermediate_sum}")

        # For testing purpose, triger cancel request
        # if feedback.feedback.intermediate_sum == 91:
        #     print("Canceling goal")
        #     self.goal_handle_.cancel_goal_async()

    def goal_response_callback(self, future):
        """
        Callback function for handling the goal response.

        This is called once the server accepts or rejects the goal.

        Args:
            future: Future object containing the ClientGoalHandle.
        """
        try:
            self.goal_handle_: ClientGoalHandle = future.result()
        except Exception as e:
            self.get_logger().error(f"Goal request exception: {e}")
            return

        if self.goal_handle_ is None:
            self.get_logger().error("Invalid goal handle")
            return

        if not self.goal_handle_.accepted:
            self.get_logger().warning("Goal got rejected")
            return

        self.get_logger().info("Goal accepted")
        # Request the final result asynchronously
        self.goal_handle_.get_result_async().add_done_callback(
            self.goal_result_callback
        )

    def goal_result_callback(self, future):
        """
        Callback function for handling the final result of the action.

        Args:
            future: Future object containing the result and status.
        """

        # check status of request
        status = future.result().status
        result = future.result().result

        if status == GoalStatus.STATUS_SUCCEEDED:
            self.get_logger().info("Succeeded goal")
        elif status == GoalStatus.STATUS_ABORTED:
            self.get_logger().error("Aborted goal")
        elif status == GoalStatus.STATUS_CANCELED:
            self.get_logger().info("Canceled goal")

        self.get_logger().info(f"Client result = {result.sum}")


def main(args=None):
    rclpy.init(args=args)

    node = AddUntilClientNode()

    target_number = node.get_parameter("target_number").value
    period = node.get_parameter("period").value

    node.send_goal(target_number, period)

    rclpy.spin(node)

    rclpy.shutdown()


if __name__ == "__main__":
    main()

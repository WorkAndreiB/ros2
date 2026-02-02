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

    def send_goal(self, target_number, period):
        # wait for server
        is_server_ready_ = self.add_until_client_.wait_for_server(5)

        if is_server_ready_:
            self.get_logger().info("Server is ready...")
            goal = AddUntil.Goal()
            goal.target_number = target_number
            goal.period = period

            self.get_logger().info("Sending command to server...")

            response_handle = self.add_until_client_.send_goal_async(
                goal=goal, feedback_callback=self.goal_feedback_callback
            )

            response_handle.add_done_callback(self.goal_response_callback)

        else:
            self.get_logger().error("Server is not ready...")

    # feedback callback
    def goal_feedback_callback(self, feedback):
        self.get_logger().info(f"Feedback: {feedback.feedback.intermediate_sum}")

    def goal_response_callback(self, future):
        goal_handle: ClientGoalHandle = future.result()

        if goal_handle.accepted:
            self.get_logger().info("Goal accepted")
            goal_handle.get_result_async().add_done_callback(self.goal_result_callback)
        else:
            self.get_logger().warn("Goal got rejected")

    def goal_result_callback(self, future):
        # check status of request
        status = future.result().status
        result = future.result().result

        if status == GoalStatus.STATUS_SUCCEEDED:
            self.get_logger().info("Success")
        elif status == GoalStatus.STATUS_ABORTED:
            self.get_logger().error("Aborted")

        self.get_logger().info(f"Client result = {result.sum}")


def main(args=None):
    rclpy.init(args=args)

    node = AddUntilClientNode()
    node.send_goal(11, 0.5)

    rclpy.spin(node)

    rclpy.shutdown()


if __name__ == "__main__":
    main()

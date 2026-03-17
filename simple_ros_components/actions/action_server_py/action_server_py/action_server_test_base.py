import rclpy
import unittest

from typing import Tuple

from rclpy.node import Node
from rclpy.action import ActionClient

from action_interfaces.action import AddUntil

from rclpy.action.client import ClientGoalHandle
from rclpy.task import Future


class AddUntilTestBase(unittest.TestCase):
    FUTURE_TIMEOUT_SEC = 5.0

    @classmethod
    def setUpClass(cls):
        rclpy.init()
        cls.node = Node("test_add_until_action_client")

    @classmethod
    def tearDownClass(cls):
        cls.node.destroy_node()
        rclpy.shutdown()

    def make_goal(self, target_number, period) -> AddUntil.Goal:
        """
        Create a new AddUntil goal.

        Args:
            target_number (int): The target number for the goal.
            period (float): The period for the goal.

        Returns:
            AddUntil.Goal: The created goal.
        """
        goal = AddUntil.Goal()
        goal.target_number = target_number
        goal.period = period
        return goal

    def make_client(self) -> ActionClient:
        """
        Create a new ActionClient for the AddUntil action.

        Returns:
            ActionClient: The created action client.
        """
        return ActionClient(
            node=self.node, action_type=AddUntil, action_name="AddUntil"
        )

    def send_goal_request(
        self, client: ActionClient, goal: AddUntil.Goal
    ) -> ClientGoalHandle:
        """
        Send a goal and wait for the goal response.

        Args:
            client
            goal

        Returns:
            ClientGoalHandle:
                Goal handle returned by the server.
        """
        send_goal_future = client.send_goal_async(goal)
        rclpy.spin_until_future_complete(
            self.node, send_goal_future, timeout_sec=self.FUTURE_TIMEOUT_SEC
        )

        return send_goal_future.result()

    def wait_for_result(self, goal_handle: ClientGoalHandle) -> Future:
        """
        Wait for the final result of a previously accepted goal.

        Args:
            goal_handle

        Returns:
            Future:
                Completed future containing the action result.
        """
        result_future = goal_handle.get_result_async()
        rclpy.spin_until_future_complete(
            self.node, result_future, timeout_sec=self.FUTURE_TIMEOUT_SEC
        )
        return result_future

    def send_goal_and_wait(
        self, client: ActionClient, goal: AddUntil.Goal
    ) -> Tuple[ClientGoalHandle, Future]:
        """
        Send a goal and wait until the final result

        Args:
            client
            goal

        Returns:
            Tuple[ClientGoalHandle, Future]:
        """
        goal_handle = self.send_goal_request(client, goal)
        result_future = self.wait_for_result(goal_handle)
        return goal_handle, result_future

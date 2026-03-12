import pytest
import rclpy
import unittest
import launch
import launch_ros
import launch_testing
import launch_testing.actions

from rclpy.node import Node
from rclpy.action import ActionClient

from action_interfaces.action import AddUntil


@pytest.mark.rostest
def generate_test_description():
    action_server = launch_ros.actions.Node(
        package="action_server_py",
        executable="add_until_server",
        name="add_until_server",
        output="screen",
        arguments=["--ros-args", "-p", "goal_policy:=parallel"],
    )

    return (
        launch.LaunchDescription(
            [
                action_server,
                launch_testing.actions.ReadyToTest(),
            ]
        ),
        {},
    )


class TestAddUntilActionServer(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        rclpy.init()
        cls.node = Node("test_add_until_action_client")

    @classmethod
    def tearDownClass(cls):
        cls.node.destroy_node()
        rclpy.shutdown()

    def test_send_goal(self):
        client = ActionClient(
            node=self.node,
            action_type=AddUntil,
            action_name="AddUntil",
        )

        self.assertTrue(client.wait_for_server(timeout_sec=5.0))

        goal = AddUntil.Goal()
        goal.target_number = 4
        goal.period = 0.01

        send_goal_future = client.send_goal_async(goal)
        rclpy.spin_until_future_complete(self.node, send_goal_future)

        goal_handle = send_goal_future.result()
        self.assertIsNotNone(goal_handle)
        self.assertTrue(goal_handle.accepted)

        result_future = goal_handle.get_result_async()
        rclpy.spin_until_future_complete(self.node, result_future)

        result = result_future.result().result

        # final assertion
        self.assertEqual(result.sum, 110)

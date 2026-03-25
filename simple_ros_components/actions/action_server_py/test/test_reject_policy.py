import pytest
import rclpy
import unittest
import launch
import launch_ros
import launch_testing
import launch_testing.actions

from typing import Tuple

from rclpy.node import Node
from rclpy.action import ActionClient

from action_interfaces.action import AddUntil

from rclpy.action.client import ClientGoalHandle
from rclpy.task import Future

from action_server_py.action_server_test_base import AddUntilTestBase


# method required by launch_testing
# tells ROS what nodes to be started before running tests
# equivalent with: ros2 run action_server_py add_until_server --ros-args -p goal_policy:=reject
@pytest.mark.rostest
def generate_test_description():
    action_server = launch_ros.actions.Node(
        package="action_server_py",
        executable="add_until_server",
        name="add_until_server",
        output="screen",
        arguments=["--ros-args", "-p", "goal_policy:=reject"],
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


class TestRejectPolicy(AddUntilTestBase):
    def test_valid_goal(self):
        client = self.make_client()

        self.assertTrue(client.wait_for_server(timeout_sec=5.0))

        goal = self.make_goal(target_number=4, period=0.01)
        goal_handle = self.send_goal_request(client, goal)
        self.assertIsNotNone(goal_handle)
        self.assertTrue(goal_handle.accepted)

        result_future = self.wait_for_result(goal_handle)

        result = result_future.result().result

        # final assertion
        self.assertEqual(result.sum, 10)

    def test_reject_goal(self):
        client = self.make_client()
        self.assertTrue(client.wait_for_server(timeout_sec=5.0))

        # send first goal
        goal1 = self.make_goal(target_number=4, period=0.01)
        goal_handle1 = self.send_goal_request(client, goal1)
        self.assertIsNotNone(goal_handle1)
        self.assertTrue(goal_handle1.accepted)

        # send second goal while the first one is still executing
        goal2 = self.make_goal(target_number=6, period=0.01)
        goal_handle2 = self.send_goal_request(client, goal2)
        self.assertIsNotNone(goal_handle2)
        self.assertTrue(goal_handle2.accepted)

        result_future = self.wait_for_result(goal_handle1)

        result = result_future.result().result

        # final assertion
        self.assertEqual(result.sum, 10)

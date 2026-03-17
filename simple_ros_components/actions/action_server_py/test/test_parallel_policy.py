import pytest
import rclpy
import unittest
import launch
import launch_ros
import launch_testing
import launch_testing.actions

from action_interfaces.action import AddUntil

from action_server_py.action_server_test_base import AddUntilTestBase


# method required by launch_testing
# tells ROS what nodes to be started before running tests
# equivalent with: ros2 run action_server_py add_until_server --ros-args -p goal_policy:=parallel
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


class TestParallelPolicy(AddUntilTestBase):
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

    def test_invalid_goal(self):
        client = self.make_client()

        self.assertTrue(client.wait_for_server(timeout_sec=5.0))

        # send odd target number
        goal = self.make_goal(target_number=5, period=0.01)
        goal_handle = self.send_goal_request(client, goal)

        self.assertIsNotNone(goal_handle)
        self.assertFalse(goal_handle.accepted)

        # send target number < 0
        goal = self.make_goal(target_number=-1, period=0.01)
        goal_handle = self.send_goal_request(client, goal)

        self.assertIsNotNone(goal_handle)
        self.assertFalse(goal_handle.accepted)

        # send invalid period
        goal = self.make_goal(target_number=6, period=-1.0)
        goal_handle = self.send_goal_request(client, goal)

        self.assertIsNotNone(goal_handle)
        self.assertFalse(goal_handle.accepted)

    def test_multiple_valid_goals(self):
        client = self.make_client()
        self.assertTrue(client.wait_for_server(timeout_sec=5.0))

        goals = [self.make_goal(target_number=i * 2, period=0.02) for i in range(1, 6)]

        send_goal_futures = []

        # send all goals async
        for goal in goals:
            future = client.send_goal_async(goal)
            send_goal_futures.append(future)

        # wait for all goals to be processed
        for future in send_goal_futures:
            rclpy.spin_until_future_complete(
                self.node, future, timeout_sec=self.FUTURE_TIMEOUT_SEC
            )

        # get all goal handles
        goal_handles = [future.result() for future in send_goal_futures]

        self.assertEqual(len(goal_handles), 5)
        self.assertTrue(all(goal_handle is not None for goal_handle in goal_handles))
        self.assertTrue(all(goal_handle.accepted for goal_handle in goal_handles))

        result_futures = [
            goal_handle.get_result_async() for goal_handle in goal_handles
        ]
        for future in result_futures:
            rclpy.spin_until_future_complete(
                self.node, future, timeout_sec=self.FUTURE_TIMEOUT_SEC
            )

        results = [future.result().result.sum for future in result_futures]
        expected_result = [3, 10, 21, 36, 55]

        self.assertEqual(len(results), 5)
        self.assertEqual(results, expected_result)

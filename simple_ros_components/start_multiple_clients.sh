#!/bin/bash

echo "Starting multiple action clients..."

ros2 action send_goal /AddUntil action_interfaces/action/AddUntil "{target_number: 10, period: 1.5}" --feedback &
ros2 action send_goal /AddUntil action_interfaces/action/AddUntil "{target_number: 16, period: 1.0}" --feedback &
ros2 action send_goal /AddUntil action_interfaces/action/AddUntil "{target_number: 20, period: 0.75}" --feedback &

wait

echo "All 3 clients completed."


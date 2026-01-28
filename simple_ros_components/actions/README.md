# Actions

## Commands

+ To send goal to action server:

`ros2 action send_goal /<action_name> <interface_pkg/action/interface_name> "{...}"`

`ros2 action send_goal /AddUntil action_interfaces/action/AddUntil "{target_number: 10, period: 1.5}"`
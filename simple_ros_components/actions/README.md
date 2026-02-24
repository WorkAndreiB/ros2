# Actions
Actions use a client-server model. An “action client” node sends a goal to an “action server” node that acknowledges the goal and returns a stream of feedback and a result.
## Commands

+ Start server:

`ros2 run action_server_py add_until_server --ros-args -p goal_policy:="<policy>"`

+ Start client with arguments:

`ros2 run action_client_py add_until_client --ros-args -p target_number:=20 -p period:=0.5`

+ To send goal to action server:

`ros2 action send_goal /<action_name> <interface_pkg/action/interface_name> "{...}" [--feedback]`

`ros2 action send_goal /AddUntil action_interfaces/action/AddUntil "{target_number: 10, period: 1.5}"`

+ See all actions available: `ros2 action list`
+ See action info: `ros2 action info /<action>`


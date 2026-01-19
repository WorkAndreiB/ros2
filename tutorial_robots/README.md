# Robotics
This component is used to create and interact with some somple robots

## Table of content
- first robot
- [Robot arm](#robot-arm)


### Robot arm

Use robot arm bringup package to start the robot arm visualization:
```sh
ros2 launch robot_arm_bringup robot_arm.launch.xml
```

Use robot arm controller package to enable communication with the arm
```sh
ros2 run robot_arm_controller ArmControllerExec
```

Use ros command line interface to send requests to robot arm
- Keep in mind that the arm and the gripper use different ros topics 

`
ros2 topic pub robot_arm/arm_command/named_target robot_arm_interfaces/msg/NamedTarget "{named_target: 'pose_1'}"
`

<video controls width="720">
  <source src="https://github.com/WorkAndreiB/ros2/blob/feature/implement_tutorial_robots/docs/demo.webm" type="video/webm">
</video>
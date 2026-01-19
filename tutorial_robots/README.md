# Robotics
This component is used to create and interact with some simple robots

## Table of contents
- [First robot](#first-robot)
- [Robot arm](#robot-arm)
- [Tools](#tools)

### First robot

Use first robot bringup package to launch the simulation of the robot in Gazebo and interact with it

The robot can move and interact with objects from Gazebo world. It can collide or push different objects based on their mass

```sh
ros2 launch first_robot_bringup first_robot_gazebo.launch.xml
```
See robot in gazebo and in rviz
![](/docs/robot_gazebo.png)
![](/docs/robot_rviz.png)

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

`ros2 topic pub robot_arm/arm_command/named_target robot_arm_interfaces/msg/NamedTarget "{named_target: 'pose_1'} -1"`

![Robot arm demo](/docs/robot_arm_home.png)

The robot arm moves to the predefined target `pose_1`.


![Move arm to pose_1](/docs/robot_arm_pose_1.png)


### Tools

- **RViz**: Visualization tool for robot state, TF frames, sensor data, and motion planning results.

- **Gazebo**: Physics-based simulation environment used to simulate the robot arm, test controllers, validate kinematics and dynamics, and verify behavior before deployment on real hardware.

- **MoveIt**:
  - Kinematic computations (forward and inverse kinematics)
  - Motion planning
  - Collision checking
  - Trajectory generation and execution

- **MoveIt Setup Assistant**:
  - Generates the MoveIt configuration package
  - Defines planning groups and end effectors
  - Configures robot kinematics, joint limits, and controllers
  - Sets up collision matrices and planning pipelines
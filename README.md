# Learn project

Ros2 learning project using C++ and Python 

Using Ros2 Jazzy distro

## Table of content
- [Ros jazy instalation steps](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)
- [Simple ros components](/simple_ros_components/README.md)
- [Robotics](#robotics)
    - [Tutorials](#urdf-tutorial)
    - [Documentation links](#documentation-links)


## Robotics components

### URDF Tutorial
URDF (Unified Robot Description Format)

#### Create tf tree

- need package: `ros-jazzy-tf2-tools`
- run 
```sh
ros2 run tf2_tools view_frames
```

#### Run robot state publisher:

```sh
ros2 run robot_state_publisher robot_state_publisher --ros-args -p robot_description:="$(xacro tutorial_robots/my_robot.urdf)"
```

```sh
ros2 run joint_state_publisher_gui joint_state_publisher_gui
```

```sh
ros2 run rviz2 rviz2
```


### Documentation links

+ URDF: https://wiki.ros.org/urdf/XML
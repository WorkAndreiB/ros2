# Learn projects

Ros2 learning project using mainly C++ and Python 

Env: Ros2 Jazzy distro

## Table of content
- [Ros jazy instalation steps](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)
- [Simple ros components](/simple_ros_components/README.md)
- [Robotics](/tutorial_robots/README.md)
- [Documentation links](#documentation-links)
- [Usefull commands](#commands)

### Documentation links

+ [URDF](https://wiki.ros.org/urdf/XML)
+ [Ros Interfaces](https://docs.ros.org/en/foxy/Concepts/About-ROS-Interfaces.html)

### Commands

## Build
- build entire workspace
```bash 
    colcon build
```

- build specified package
```bash 
    colcon build --packages-select <package_name>
```

## Source
Source workspace after any changes:

```bash
    source install/setup.bash
```

## Run
Runs a specific ROS 2 node from a package.

`ros2 run <package_name> <executable_name>`

## Launch 
Launches a set of ROS 2 nodes and configurations required to run the system.

`ros2 launch <package_name> <launch_file>.launch.py`

## Topics
- see topics
```bash 
    ros2 topic list
```

- see topics info: type, number of publisher, number of subscribers
```bash 
    ros2 topic info /<topic_name>
```

- listen on specific topic
```bash 
    ros2 topic echo /<topic>
```

- publish message on specific topic
```bash 
    ros2 topic pub -r <number_of_msg_per_second> <topic> <type> "{<data_field>: <data>}"
```
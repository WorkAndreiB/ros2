# Basic Ros instructions
This component is used to create simple ros commponents like ros2 nodes, or ros2 service client for learning purposes

# Build
- build entire workspace
```bash 
    colcon build
```

- build specified package
```bash 
    colcon build --packages-select <package_name>
```

# Nodes
- run node 
```bash 
    ros2 run <package_name> <node_name>
```

- rename node at runtime 
```bash  
    ros2 run <package_name> <node_name> --ros-args -r __node:=<new_node_name>
```

- show running nodes
```bash 
    ros2 node list
```

- get more info about specific node
```bash 
    ros2 node info /<node>
```

- see active nodes
```bash 
    ros2 node list
```

- see info about specific node
```bash 
    ros2 node info <node>
```

# Topics
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

- see the frequecy of topic messages
```bash 
    ros2 topic hz /<topic>
```

- publish message on specific topic
```bash 
    ros2 topic pub -r <number_of_msg_per_second> <topic> <type> "{<data_field>: <data>}"
```
Example: ros2 topic pub -r 5 robot_news example_interfaces/msg/String  "{data: 'Hello'}"

# Interface
- see interface implementation detail
```bash 
    ros2 interface show <interface>
```

# Remember:
- source workspace after any changes:
```bash
    source install/setup.bash
```
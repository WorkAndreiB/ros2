# Learn project

Ros2 learning project using C++ and Python 

# Instructions
- build entire workspace
```bash 
    colcon build
```

- build specified package
```bash 
    colcon build --packages-select <package_name>
```

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
    ros2 node info /node
```

- see topics
```bash 
    ros2 topic list
```

- listen on specific topic
```bash 
    ros2 topic echo /<topic>
```

- see active nodes
```bash 
    ros2 node list
```

- see info about specific node
```bash 
    ros2 node info <node>
```

# Remember:
- source workspace after any changes:
    ```bash
    source install/setup.bash
    ```
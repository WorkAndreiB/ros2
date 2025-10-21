from launch import LaunchDescription
from ament_index_python.packages import get_package_share_path
from launch_ros.parameter_descriptions import ParameterValue
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

import os

def generate_launch_description():

    # get path to robot description urdf file from share folder (where it was installed in CMake)
    urdf_path = os.path.join(get_package_share_path('first_robot_description'), 'urdf', 'my_robot.urdf.xacro')

    #get path to rviz configuration
    rviz_path = os.path.join(get_package_share_path('first_robot_description'), 'rviz', 'urdf_config.rviz')

    #create robot_description parameter for robot_state_publisher node
    robot_description = ParameterValue(Command(['xacro ', urdf_path]), value_type=str)

    # ros2 run robot_state_publisher robot_state_publisher 
    # --ros-args -p robot_description:="$(xacro first_robot.urdf.xacro)"
    robot_state_publisher_node= Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{'robot_description':robot_description}]
        #or can have robot_description value directly here
        # parameters=[{'robot_description': Command(['xacro ', urdf_path])
    )

    # Gazebo (Ignition Gazebo / ros_gz_sim)
    gz_sim_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            PathJoinSubstitution([
                os.path.join(get_package_share_path('ros_gz_sim'),
                'launch',
                'gz_sim.launch.py')
            ])
        ),
        launch_arguments={'gz_args': 'empty.sdf -r'}.items()
    )

    # Create entity from robot_description topic
    gz_create = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=['-topic', 'robot_description']
    )

    rviz_node = Node(
        package="rviz2",
        executable="rviz2",
        arguments=['-d', rviz_path]
    )

    return LaunchDescription([
        robot_state_publisher_node,
        gz_sim_launch,
        gz_create,
        rviz_node
    ])
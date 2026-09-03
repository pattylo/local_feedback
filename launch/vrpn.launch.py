#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():

    server_arg = DeclareLaunchArgument('server', default_value='192.168.8.218')
    port_arg = DeclareLaunchArgument('port', default_value='3883')
    vrpn_topic_arg = DeclareLaunchArgument(
        'vrpn_topic',
        default_value='/vrpn_mocap/rover0/pose'
    )

    out_topic_arg = DeclareLaunchArgument(
        'out_topic',
        default_value='/rover0/mavros/vision_pose/pose'
    )

    out_frame_arg = DeclareLaunchArgument(
        'out_frame',
        default_value='vision'
    )

    pkg_share = get_package_share_directory('vrpn_mocap')
    client_yaml = PathJoinSubstitution([pkg_share, 'config', 'client.yaml'])

    vrpn_node = Node(

        package='vrpn_mocap',
        executable='client_node',
        name='vrpn_mocap_client_node',
        namespace='vrpn_mocap',

        parameters=[

            client_yaml,
            {
                'server': LaunchConfiguration('server'),
                'port': LaunchConfiguration('port'),
            },
        ],
    )

    vrpn_relay = Node(
        package='local_feedback',
        executable='vrpn_relay',
        name='vrpn_relay',
        
        parameters=[
            {
                'vrpn_topic': LaunchConfiguration('vrpn_topic'),
                'out_topic': LaunchConfiguration('out_topic'),
                'out_frame': LaunchConfiguration('out_frame'),
            },
        ],
    )

    return LaunchDescription([
        server_arg,
        port_arg,
        vrpn_topic_arg,
        out_topic_arg,
        out_frame_arg,
        vrpn_node,
        vrpn_relay,
    ])
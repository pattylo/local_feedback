cmake_minimum_required(VERSION 3.8)
project(local_feedback)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3 REQUIRED)
# find_package(Sophus REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(mavros_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclpy REQUIRED)
find_package(visualization_msgs REQUIRED)


if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

include_directories(
    include
    # ${Sophus_INCLUDE_DIRS}
)

###################################################################

# vrpn_relay
add_executable(vrpn_relay
	src/vrpn_relay.cpp
)
ament_target_dependencies(vrpn_relay
	rclcpp 
	geometry_msgs
	sensor_msgs
	mavros_msgs
	# visualization_msgs
    # bezier_traj_msgs
)

install(TARGETS vrpn_relay DESTINATION lib/${PROJECT_NAME})

###################################################################
# Install Python scripts
install(PROGRAMS
#   script/servo_rc.py
#   script/servo_test.py
  script/offboard.py
  DESTINATION lib/${PROJECT_NAME}
)

###################################################################
install(
  DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
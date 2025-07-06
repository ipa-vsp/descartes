cmake_minimum_required(VERSION 3.10)
project(descartes_moveit)

add_compile_options(-Wall -Wextra)

find_package(ament_cmake REQUIRED)
find_package(descartes_core REQUIRED)
find_package(descartes_trajectory REQUIRED)
find_package(moveit_core REQUIRED)
find_package(moveit_ros_planning REQUIRED)
find_package(pluginlib REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_eigen REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

# Eigen 3.2 (Wily) only provides EIGEN3_INCLUDE_DIR, not EIGEN3_INCLUDE_DIRS
if(NOT EIGEN3_INCLUDE_DIRS)
  set(EIGEN3_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIR})
endif()



###########
## Build ##
###########
include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
)

## Descartes MoveIt lib
add_library(${PROJECT_NAME}
            src/ikfast_moveit_state_adapter.cpp
            src/moveit_state_adapter.cpp
            src/plugin_init.cpp
            src/seed_search.cpp
)
target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
)

ament_target_dependencies(
  ${PROJECT_NAME}
  descartes_core
  descartes_trajectory
  moveit_core
  moveit_ros_planning
  pluginlib
  tf2
  tf2_eigen
  Eigen3
)

#############
## Install ##
#############

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(FILES moveit_adapter_plugins.xml
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()

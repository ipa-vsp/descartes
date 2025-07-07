cmake_minimum_required(VERSION 3.10)
project(descartes_utilities)

add_compile_options(-Wall -Wextra)

find_package(ament_cmake REQUIRED)
find_package(descartes_core REQUIRED)
find_package(descartes_trajectory REQUIRED)
find_package(trajectory_msgs REQUIRED)

include_directories(
  include
)

add_library(${PROJECT_NAME}
  src/ros_conversions.cpp
)

target_link_libraries(${PROJECT_NAME})

ament_target_dependencies(
  ${PROJECT_NAME}
  descartes_core
  descartes_trajectory
  trajectory_msgs
  builtin_interfaces
)

install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()

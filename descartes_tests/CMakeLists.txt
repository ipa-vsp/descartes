cmake_minimum_required(VERSION 3.10)
project(descartes_tests)

add_compile_options(-Wall -Wextra)

find_package(ament_cmake REQUIRED)
find_package(descartes_moveit REQUIRED)
find_package(descartes_planner REQUIRED)
find_package(descartes_trajectory REQUIRED)
find_package(eigen_conversions REQUIRED)



include_directories(
  include
)

add_library(${PROJECT_NAME}
  src/cartesian_robot.cpp
)

ament_target_dependencies(
  ${PROJECT_NAME}
  descartes_moveit
  descartes_planner
  descartes_trajectory
  eigen_conversions
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

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  # Trajectory Unit Tests
  ament_add_gtest(${PROJECT_NAME}_trajectory_utest
    test/trajectory/utest.cpp
    test/trajectory/axial_symmetric_pt.cpp
    test/trajectory/cart_trajectory_pt.cpp
    test/trajectory/joint_trajectory_pt.cpp
    test/trajectory/cartesian_robot_test.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_trajectory_utest PUBLIC GTEST_USE_OWN_TR1_TUPLE=0)
  target_link_libraries(${PROJECT_NAME}_trajectory_utest ${PROJECT_NAME})

  # Planner unit tests
  ament_add_gtest(${PROJECT_NAME}_planner_utest
    test/planner/utest.cpp
    test/planner/dense_planner.cpp
    test/planner/sparse_planner.cpp
    test/planner/planning_graph_tests.cpp
    test/planner/utils/trajectory_maker.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_planner_utest PUBLIC GTEST_USE_OWN_TR1_TUPLE=0)
  target_link_libraries(${PROJECT_NAME}_planner_utest ${PROJECT_NAME})

  # Moveit adapter unit tests
  ament_add_gtest(${PROJECT_NAME}_moveit_utest
    test/moveit/launch/utest.launch
    test/moveit/utest.cpp
    test/moveit/moveit_state_adapter_test.cpp
  )
  target_compile_definitions(${PROJECT_NAME}_moveit_utest PUBLIC GTEST_USE_OWN_TR1_TUPLE=0)
  target_link_libraries(${PROJECT_NAME}_moveit_utest ${PROJECT_NAME})

endif()

ament_package()

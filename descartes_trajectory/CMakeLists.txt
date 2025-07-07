cmake_minimum_required(VERSION 3.10)
project(descartes_trajectory)

add_compile_options(-Wall -Wextra)

find_package(ament_cmake REQUIRED)
find_package(descartes_core REQUIRED)
find_package(moveit_core REQUIRED)
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

add_library(${PROJECT_NAME}
            src/axial_symmetric_pt.cpp
            src/cart_trajectory_pt.cpp
            src/joint_trajectory_pt.cpp
)

target_link_libraries(${PROJECT_NAME}
  ${Boost_LIBRARIES}
)

ament_target_dependencies(
  ${PROJECT_NAME}
  descartes_core
  moveit_core
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

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})


#############
## Testing ##
#############

if(BUILD_TESTING)
  find_package(ament_cmake_gtest REQUIRED)
  set(UTEST_SRC_FILES
    test/utest.cpp
    test/trajectory_pt.cpp
  )
  ament_add_gtest(${PROJECT_NAME}_utest ${UTEST_SRC_FILES})
  target_compile_definitions(${PROJECT_NAME}_utest PUBLIC GTEST_USE_OWN_TR1_TUPLE=0)
  target_link_libraries(${PROJECT_NAME}_utest ${PROJECT_NAME})
endif()

ament_package()

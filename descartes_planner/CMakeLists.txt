cmake_minimum_required(VERSION 3.10)
project(descartes_planner)

add_compile_options(-Wall -Wextra)

find_package(ament_cmake REQUIRED)
find_package(descartes_core REQUIRED)
find_package(descartes_trajectory REQUIRED)
find_package(pluginlib REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(Boost REQUIRED)

# Let's try to use open-mp parallization if we can
find_package(OpenMP)
if (OPENMP_FOUND)
  message(STATUS "Descartes will use OpenMP")
  set(OpenMP_FLAGS ${OpenMP_CXX_FLAGS})
endif()

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

## DescartesTrajectoryPt lib
add_library(${PROJECT_NAME}
            src/dense_planner.cpp
            src/ladder_graph_dag_search.cpp
            src/planning_graph.cpp
            src/plugin_init.cpp
            src/sparse_planner.cpp
)

target_link_libraries(${PROJECT_NAME}
                      ${OpenMP_FLAGS}
)

target_compile_options(${PROJECT_NAME} PRIVATE ${OpenMP_FLAGS})

ament_target_dependencies(
  ${PROJECT_NAME}
  descartes_core
  descartes_trajectory
  pluginlib
  Eigen3
  Boost
)

#############
## Install ##
#############
install(TARGETS ${PROJECT_NAME}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(FILES ${PROJECT_NAME}_plugins.xml
  DESTINATION share/${PROJECT_NAME}
)

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION include/${PROJECT_NAME}
)

ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

ament_package()

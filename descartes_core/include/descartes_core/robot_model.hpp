#ifndef ROBOT_KINEMATICS_HPP_
#define ROBOT_KINEMATICS_HPP_

#include <moveit/robot_model/robot_model.h>
#include <moveit/kinematic_constraints/kinematic_constraint.h>
#include <Eigen/Geometry>
#include "descartes_core/utils.hpp"
#include <memory>

namespace descartes_core
{
DESCARTES_CLASS_FORWARD(RobotModel);

/**
 * @brief Interface class defining robot kinematic and dynamic functions for ROS2.
 * Provides forward and inverse kinematics methods compatible with MoveIt2.
 */
class RobotModel
{
public:
  RobotModel() : check_collisions_(false) {}
  virtual ~RobotModel() = default;

  virtual bool getIK(const Eigen::Isometry3d& pose,
                     const std::vector<double>& seed_state,
                     std::vector<double>& joint_pose) const = 0;

  virtual bool getAllIK(const Eigen::Isometry3d& pose,
                        std::vector<std::vector<double>>& joint_poses) const = 0;

  virtual bool getFK(const std::vector<double>& joint_pose,
                     Eigen::Isometry3d& pose) const = 0;

  virtual int getDOF() const = 0;

  virtual bool isValid(const std::vector<double>& joint_pose) const = 0;

  virtual bool isValid(const Eigen::Isometry3d& pose) const = 0;

  virtual std::vector<double> getJointVelocityLimits() const = 0;

  virtual bool initialize(const std::string& robot_description,
                          const std::string& group_name,
                          const std::string& world_frame,
                          const std::string& tcp_frame) = 0;

  virtual void setCheckCollisions(bool check_collisions)
  {
    check_collisions_ = check_collisions;
  }

  virtual bool getCheckCollisions() const
  {
    return check_collisions_;
  }

  virtual bool isValidMove(const std::vector<double>& from_joint_pose,
                           const std::vector<double>& to_joint_pose,
                           double dt) const
  {
    return isValidMove(from_joint_pose.data(), to_joint_pose.data(), dt);
  }

  virtual bool isValidMove(const double* s, const double* f, double dt) const = 0;

protected:
  bool check_collisions_;
};

}  // namespace descartes_core

#endif /* ROBOT_KINEMATICS_H_ */

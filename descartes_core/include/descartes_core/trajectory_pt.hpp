#ifndef TRAJECTORY_PT_HPP_
#define TRAJECTORY_PT_HPP_

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <memory>
#include <rclcpp/rclcpp.hpp>

#include "descartes_core/robot_model.hpp"
#include "descartes_core/trajectory_id.hpp"
#include "descartes_core/trajectory_timing_constraint.hpp"

namespace descartes_core
{
/**
 * @brief Wrapper for an Eigen::Isometry3d affine transform with cached inverse.
 */
struct Frame
{
  Frame() = default;

  explicit Frame(const Eigen::Isometry3d &a) : frame(a), frame_inv(a.inverse()) {}

  Frame(const Eigen::Affine3d& a)
  {
    frame.translation() = a.translation();
    frame.linear() = a.rotation();
    frame_inv = frame.inverse();
  }

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  Eigen::Isometry3d frame{Eigen::Isometry3d::Identity()};
  Eigen::Isometry3d frame_inv{Eigen::Isometry3d::Identity()};

  static Frame Identity()
  {
    return Frame(Eigen::Isometry3d::Identity());
  }
};

/**
 * @brief Base class representing a point in a robot trajectory, supporting both Cartesian and Joint space queries.
 */
DESCARTES_CLASS_FORWARD(TrajectoryPt);
class TrajectoryPt
{
public:
  using ID = TrajectoryID64;

  explicit TrajectoryPt(const TimingConstraint &timing)
    : id_(TrajectoryID64::make_id()), timing_(timing)
  {}

  virtual ~TrajectoryPt() = default;

  virtual bool getClosestCartPose(const std::vector<double>& seed_state,
                                  const RobotModel& kinematics,
                                  Eigen::Isometry3d& pose) const = 0;

  virtual bool getNominalCartPose(const std::vector<double>& seed_state,
                                  const RobotModel& kinematics,
                                  Eigen::Isometry3d& pose) const = 0;

  virtual void getCartesianPoses(const RobotModel& kinematics,
                                 std::vector<Eigen::Isometry3d>& poses) const = 0;

  virtual bool getClosestJointPose(const std::vector<double>& seed_state,
                                   const RobotModel& model,
                                   std::vector<double>& joint_pose) const = 0;

  virtual bool getNominalJointPose(const std::vector<double>& seed_state,
                                   const RobotModel& model,
                                   std::vector<double>& joint_pose) const = 0;

  virtual void getJointPoses(const RobotModel& model,
                             std::vector<std::vector<double>>& joint_poses) const = 0;

  virtual bool isValid(const RobotModel& model) const = 0;

  virtual bool setDiscretization(const std::vector<double>& discretization) = 0;

  ID getID() const
  {
    return id_;
  }

  void setID(const ID& id)
  {
    id_ = id;
  }

  virtual TrajectoryPtPtr copy() const = 0;

  virtual TrajectoryPtPtr copyAndSetTiming(const TimingConstraint& tm) const
  {
    auto cp = copy();
    cp->setTiming(tm);
    return cp;
  }

  virtual TrajectoryPtPtr clone() const
  {
    auto cp = copy();
    cp->setID(TrajectoryID64::make_id());
    return cp;
  }

  virtual TrajectoryPtPtr cloneAndSetTiming(const TimingConstraint& tm) const
  {
    auto cp = clone();
    cp->setTiming(tm);
    return cp;
  }

  const TimingConstraint& getTiming() const
  {
    return timing_;
  }

  void setTiming(const TimingConstraint& timing)
  {
    timing_ = timing;
  }

protected:
  ID id_; ///< Unique identifier for the trajectory point.
  TimingConstraint timing_; ///< Timing constraints associated with this trajectory point.
};

} // namespace descartes_core

#endif /* TRAJECTORY_PT_HPP_ */
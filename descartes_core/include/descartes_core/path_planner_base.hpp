#ifndef DESCARTES_CORE_PATH_PLANNER_BASE_HPP_
#define DESCARTES_CORE_PATH_PLANNER_BASE_HPP_

#include "descartes_core/trajectory_pt.hpp"
#include "descartes_core/robot_model.hpp"
#include <vector>
#include <map>
#include <string>

namespace descartes_core
{
namespace PlannerErrors
{
enum class PlannerError
{
  OK = 1,
  IK_NOT_AVAILABLE = -1,
  FX_NOT_AVAILABLE = -2,
  SELF_COLLISION_FOUND = -3,
  ENVIRONMENT_COLLISION_FOUND = -4,
  PLANNING_TIMEOUT = -5,
  EMPTY_PATH = -6,
  SPEED_LIMIT_EXCEEDED = -7,
  ACCELERATION_LIMIT_EXCEEDED = -8,
  MAX_TRAJECTORY_SIZE_EXCEEDED = -9,
  UNINITIALIZED = -10,
  INVALID_ID = -11,
  INCOMPLETE_PATH = -12,
  INVALID_CONFIGURATION_PARAMETER = -13,
  UNKNOWN = -99
};

}  // namespace PlannerErrors

using PlannerError = PlannerErrors::PlannerError;
using PlannerConfig = std::map<std::string, std::string>;

DESCARTES_CLASS_FORWARD(PathPlannerBase);

class PathPlannerBase
{
public:
  PathPlannerBase() = default;
  virtual ~PathPlannerBase() = default;

  virtual bool initialize(RobotModelConstPtr model) = 0;

  virtual bool setConfig(const PlannerConfig& config) = 0;

  virtual void getConfig(PlannerConfig& config) const = 0;

  virtual bool planPath(const std::vector<TrajectoryPtPtr>& traj) = 0;

  virtual bool getPath(std::vector<TrajectoryPtPtr>& path) const = 0;

  virtual bool addAfter(const TrajectoryPt::ID& ref_id, const TrajectoryPtPtr& tp) = 0;

  virtual bool addBefore(const TrajectoryPt::ID& ref_id, const TrajectoryPtPtr& tp) = 0;

  virtual bool remove(const TrajectoryPt::ID& ref_id) = 0;

  virtual bool modify(const TrajectoryPt::ID& ref_id, const TrajectoryPtPtr& tp) = 0;

  virtual PlannerError getErrorCode() const = 0;

  virtual bool getErrorMessage(PlannerError error_code, std::string& msg) const = 0;
};

}  // namespace descartes_core

#endif /* DESCARTES_CORE_PATH_PLANNER_BASE_HPP_ */
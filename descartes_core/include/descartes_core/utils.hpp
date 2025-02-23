#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <memory>
#include <rclcpp/rclcpp.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <cmath>

/** \def DESCARTES_CLASS_FORWARD
    Macro that forward declares a class XXX, and defines two shared ptrs named XXXPtr and XXXConstPtr */

#define DESCARTES_CLASS_FORWARD(C) \
  class C; \
  using C##Ptr = std::shared_ptr<C>; \
  using C##ConstPtr = std::shared_ptr<const C>;

namespace descartes_core
{
namespace utils
{

/**
 * @brief Euler angle conventions for rotations
 */
enum class EulerConvention
{
  XYZ,
  ZYX,
  ZXZ
};

/**
 * @brief Converts translations and rotations into an Eigen Isometry3d frame.
 * Rotations follow specified Euler angle conventions.
 * 
 * @param tx, ty, tz Translations along x, y, z axes
 * @param rx, ry, rz Rotations around x, y, z axes
 * @param convention Euler angle convention (default ZYX)
 */
inline Eigen::Isometry3d toFrame(double tx, double ty, double tz, 
                                 double rx, double ry, double rz,
                                 EulerConvention convention = EulerConvention::ZYX)
{
  Eigen::Isometry3d rtn = Eigen::Isometry3d::Identity();

  switch (convention)
  {
    case EulerConvention::XYZ:
      rtn = Eigen::Translation3d(tx, ty, tz) *
            Eigen::AngleAxisd(rx, Eigen::Vector3d::UnitX()) *
            Eigen::AngleAxisd(ry, Eigen::Vector3d::UnitY()) *
            Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ());
      break;

    case EulerConvention::ZYX:
      rtn = Eigen::Translation3d(tx, ty, tz) *
            Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ()) *
            Eigen::AngleAxisd(ry, Eigen::Vector3d::UnitY()) *
            Eigen::AngleAxisd(rx, Eigen::Vector3d::UnitX());
      break;

    case EulerConvention::ZXZ:
      rtn = Eigen::Translation3d(tx, ty, tz) *
            Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ()) *
            Eigen::AngleAxisd(rx, Eigen::Vector3d::UnitX()) *
            Eigen::AngleAxisd(rz, Eigen::Vector3d::UnitZ());
      break;

    default:
      RCLCPP_ERROR(rclcpp::get_logger("descartes_core"), "Invalid Euler convention specified.");
      break;
  }

  return rtn;
}

/**
 * @brief Checks if two vectors are equal within a specified tolerance.
 *
 * @param lhs First vector
 * @param rhs Second vector
 * @param tol Tolerance for floating point comparison
 * @return True if vectors are equal within tolerance, otherwise false
 */
inline bool equal(const std::vector<double>& lhs, const std::vector<double>& rhs, double tol)
{
  if (lhs.size() != rhs.size())
    return false;

  for (size_t i = 0; i < lhs.size(); ++i)
  {
    if (std::fabs(lhs[i] - rhs[i]) > tol)
      return false;
  }

  return true;
}

}  // namespace utils
}  // namespace descartes_core

#endif /* UTILS_H_ */

#ifndef TRAJECTORY_TIMING_CONSTRAINT_HPP
#define TRAJECTORY_TIMING_CONSTRAINT_HPP

#include <rclcpp/rclcpp.hpp>

namespace descartes_core
{
/**
 * @brief Represents a window of time to achieve a trajectory point relative to a previous point or starting position.
 *
 * This struct defines 'lower' and 'upper' bounds for the desired timing window. If the upper bound
 * is zero or negative, it is considered unspecified.
 *
 * All time values are in seconds.
 */
struct TimingConstraint
{
  /**
   * @brief Default constructor creates an unspecified timing constraint.
   */
  TimingConstraint() : lower(0.0), upper(0.0) {}

  /**
   * @brief Constructs a timing constraint with a nominal time value (zero-width window).
   * @param nominal Desired time in seconds to achieve this point.
   */
  explicit TimingConstraint(double nominal) : lower(nominal), upper(nominal)
  {
    if (nominal < 0.0)
    {
      RCLCPP_WARN(rclcpp::get_logger("descartes_core"), "Nominal time must be >= 0.0, resetting to 0.0");
      lower = upper = 0.0;
    }
  }

  /**
   * @brief Constructs a timing constraint using a provided timing window.
   * @param lower Lower bound of the acceptable time window (seconds).
   * @param upper Upper bound of the acceptable time window (seconds).
   */
  TimingConstraint(double lower, double upper) : lower(lower), upper(upper)
  {
    if (lower < 0.0)
    {
      RCLCPP_WARN(rclcpp::get_logger("descartes_core"), "Lower time bound must be >= 0.0, resetting to 0.0");
      this->lower = 0.0;
    }

    if (upper < 0.0)
    {
      RCLCPP_WARN(rclcpp::get_logger("descartes_core"), "Upper time bound must be >= 0.0, resetting to 0.0");
      this->upper = 0.0;
    }
  }

  /**
   * @brief Checks if the timing constraint has been specified.
   * @return True if specified (upper bound > 0.0).
   */
  bool isSpecified() const
  {
    return upper > 0.0;
  }

  double lower;
  double upper;
};

}  // namespace descartes_core

#endif /* TRAJECTORY_TIMING_CONSTRAINT_H */

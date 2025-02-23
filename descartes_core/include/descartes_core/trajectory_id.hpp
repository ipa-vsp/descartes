#ifndef TRAJECTORY_ID_HPP_
#define TRAJECTORY_ID_HPP_

#include <iostream>
#include <mutex>
#include <atomic>

namespace descartes_core
{
namespace detail
{
/**
 * @brief Unimplemented base for IdGenerator. Users should specialize
 *        this struct for the base ID type. It represents a concept
 *        that defines the following types:
 *        1. value_type typedef representing the type of unique state object
 *        2. value_type make_nil() function that returns a nil sentinel state object
 *        3. value_type make_id() function that returns a unique state object
 *        4. bool is_nil(value_type) function that tests if an object is the sentinel
 */
template <typename T>
struct IdGenerator;

/**
 * @brief This specialization of the id generator uses a 64-bit unsigned integer
 *        for the unique 'state'. Zero is reserved as a special value.
 */
template <>
struct IdGenerator<uint64_t>
{
  using value_type = uint64_t;

  static value_type make_nil()
  {
    return 0;
  }

  static value_type make_id()
  {
    static std::atomic<value_type> counter{1};
    return counter.fetch_add(1, std::memory_order_relaxed);
  }

  static bool is_nil(value_type id)
  {
    return id == 0;
  }
};
}

/**
 * @brief TrajectoryID represents a unique ID associated with a trajectory point.
 */
template <typename T>
class TrajectoryID
{
public:
  using value_type = T;

  explicit TrajectoryID(value_type id) : id_(id) {}

  TrajectoryID() : id_(detail::IdGenerator<value_type>::make_nil()) {}

  bool is_nil() const
  {
    return detail::IdGenerator<value_type>::is_nil(id_);
  }

  value_type value() const
  {
    return id_;
  }

  static TrajectoryID make_id()
  {
    return TrajectoryID(detail::IdGenerator<value_type>::make_id());
  }

  static TrajectoryID make_nil()
  {
    return TrajectoryID(detail::IdGenerator<value_type>::make_nil());
  }

private:
  value_type id_;
};

//////////////////////
// Helper Functions //
//////////////////////

template <typename T>
inline bool operator==(const TrajectoryID<T>& lhs, const TrajectoryID<T>& rhs)
{
  return lhs.value() == rhs.value();
}

template <typename T>
inline bool operator!=(const TrajectoryID<T>& lhs, const TrajectoryID<T>& rhs)
{
  return !(lhs == rhs);
}

template <typename T>
inline bool operator<(const TrajectoryID<T>& lhs, const TrajectoryID<T>& rhs)
{
  return lhs.value() < rhs.value();
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const TrajectoryID<T>& id)
{
  os << "ID" << id.value();
  return os;
}

using TrajectoryID64 = TrajectoryID<uint64_t>;

}  // namespace descartes_core

#endif  // TRAJECTORY_ID_H
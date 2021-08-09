#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <memory>

namespace py = pybind11;

namespace CMatrixBinding {
/// Returns a numpy array that doesn't perform a deep copy of cv::Mat_<T>, but
/// doesn't own the data
template <typename T> py::array_t<T> ToNumpyArray(cv::Mat_<T> &&p_mat);

template <typename T> std::unique_ptr<cv::Mat_<T>> ToMat(py::array_t<T> p_arr);
}; // namespace CMatrixBinding

/**
 *  @brief Pybind11 custom type caster between numpy array and our custom matrix
 *
 *  Handles conversions between numpy arrays and cv::Mat_ without any user
 * interaction. Completely hides the binding from users on python and C++ side
 */
template <typename T>
struct py::detail::type_caster<cv::Mat_<T>> : public type_caster_base<cv::Mat_<T>> {
  using base = type_caster_base<cv::Mat_<T>>;

public:
  /// Handle ingoing cast: Python to C++
  /// We ignore boolean for convert because we always convert and never want to
  /// expose cv::Mat in C++ code
  bool load(py::handle src,
            bool) // We ignore boolean for convert because we always convert and
                  // never want to expose cv::Mat in C++ code
  {
    if (py::isinstance<py::array_t<T>>(src)) {
      this->value = CMatrixBinding::ToMat<T>(py::cast<array_t<T>>(src)).release();
      return true;
    } else
      return false;
  }

  /// Handle outgoing cast: C++ to Python
  /// Only accepting l-value references to automatically conform to a move
  /// return policy
  static py::handle
  cast(cv::Mat_<T> &&p_mat, py::return_value_policy,
       py::handle) { // Create the numpy array from custom matrix
    py::array_t<T> arr = CMatrixBinding::ToNumpyArray(std::move(p_mat));

    // Because we want to return ownership to python, we need to manually
    // increment it's reference count
    arr.inc_ref();

    return arr;
  }
};

// Include the .inl.hpp file here so class definition above serves as a forward
// declaration. Due to template class we can't work with a .cpp file so we need
// this inverted structure
#include "matrix-converter.inl.hh"

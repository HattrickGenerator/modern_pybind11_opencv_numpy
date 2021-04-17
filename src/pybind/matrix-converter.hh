#include <Python.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace CMatrixBinding
{
/// Returns a numpy array that doesn't perform a deep copy of CMat<T>, but doesn't own the data
template<typename T> py::array_t<T> ToNumpyArray(const CMat<T>& p_mat);

template<typename T> CMat<T>* ToMat(py::array_t<T> p_arr);
}; // namespace CMatrixBinding

/**
 *  @brief Pybind11 custom type caster between numpy array and our custom matrix
 *
 *  Handles conversions between numpy arrays and CMat without any user interaction. Completely hides the binding from users on python and C++ side
 */
template<> struct py::detail::type_caster<CMat<double>> : public type_caster_base<CMat<double>>
{
    using base = type_caster_base<CMat<double>>;

public:
    /// Handle ingoing cast: Python to C++
    /// We ignore boolean for convert because we always convert and never want to expose cv::Mat in C++ code
    bool load(py::handle src, bool);

    /// Handle outgoing cast: C++ to Python
    /// Only accepting l-value references to automatically conform to a move return policy
    static py::handle cast(CMat<double>&& p_mat, py::return_value_policy, py::handle);
};

//Include the .inl.hpp file here so class definition above serves as a forward declaration. Due to template class we can't work with a .cpp file so we need this inverted structure
#include "matrix-converter.inl.hh"

#include "matrix-binding.hh"
#include <pybind11/pybind11.h>
#include "matrix-converter.hh"

namespace py = pybind11;

/**  @brief
*
*  This file serves as a central unit for all the python bindings to be done.
*  Each definition "package" can be forward declared here and added to the PYBIND11_MODULE macro.
*  This reduces compile time and modularizes the code.
*
*  Usage: Forward declare init function. Add it in macro
*  https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-reduce-the-build-time
*
*/


//Example function of a transform
CMat<double> transform(CMat<double> p_mat, double p_factor, double p_offset)
{
    return p_mat * p_factor + p_offset;
}


PYBIND11_MODULE(cvmatbindpy, m)
{    //py::array::forcecast is the default argument and ensures that non conforming arguments are converted into an array

    m.def("transform", &transform);
}

#include "matrix-binding.hh"
#include <pybind11/pybind11.h>

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
py::array_t<double> transform(py::array_t<double> p_mat, double p_factor, double p_offset)
{
    py::array_t<double> workingCopy{p_mat.request(true)}; //Perform a deep copy
    CMat<double> mat(workingCopy);
    mat = mat*p_factor+p_offset;
    return workingCopy;
}


PYBIND11_MODULE(cvmatbindpy, m)
{
    py::class_<CMat<double>>(m, "CMat").def(py::init(&CMat<double>::wrapArray));

    //py::array::forcecast is the default argument and ensures that non conforming arguments are converted into an array
    py::implicitly_convertible<py::array_t<double>, CMat<double>>();

    m.def("transform", &transform);
}

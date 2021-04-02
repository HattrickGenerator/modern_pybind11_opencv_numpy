#pragma once
#include <opencv2/core/core.hpp>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

template<typename T> class CMat : public cv::Mat_<T>
{
public:
    static CMat wrapArray(py::array_t<T> xs)
    {
        return CMat<T>(xs);
    }

    static CMat copyArray(py::array_t<T> xs)
    {
        return CMat<T>(py::array_t<T>(xs.request(true)));
    }

    //This implementation will use the constructor of cv::Mat_(int, int*, T*) for general Nd-arrays
    CMat(py::array_t<T> xs)
        : cv::Mat_<T>(int(xs.request(true).shape.size()),
                      [](const auto & shape){return std::vector<int>(shape.begin(), shape.end());}(xs.request(true).shape).data(),
                      static_cast<T*>(xs.request(true).ptr))
    {
    }

    CMat() : cv::Mat_<T>() { }

    CMat(const CMat& p_other) : cv::Mat_<T>(p_other) { }
    CMat(CMat&& p_other) : cv::Mat_<T>(std::forward<CMat&&>(p_other)) { }

    CMat& operator=(const cv::MatExpr& expr)
    {
        cv::Mat::operator=(std::forward<const cv::MatExpr&>(expr));
        return *this;
    }

    CMat(const cv::MatExpr& e) : cv::Mat(std::forward<const cv::MatExpr&>(e)) { }

    //customMat(const cv::MatExpr& mat) : cv::Mat_<T>(mat) { }
    CMat(const cv::Mat_<T>& mat) : cv::Mat_<T>(mat) { }

    CMat(int p_row, int p_column) : cv::Mat_<T>(p_row, p_column, 1) { }

    T* data()
    {
        return static_cast<T*>(static_cast<void*>(this->ptr()));
    }

public:

    py::array_t<T> toArray()
    {
        return py::array_t<T>(py::buffer_info(this->data(),
                                              sizeof(T),
                                              py::format_descriptor<T>::format(),
                                              2,
                                              {this->rows, this->cols},
                                              {sizeof(T) * this->cols, sizeof(T)}));
    }
    operator py::array_t<T>()
    {
        return toArray();
    }

};

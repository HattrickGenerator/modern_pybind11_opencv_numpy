#pragma once
#include <opencv2/core/core.hpp>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <iostream>


namespace py = pybind11;

template<typename T> class CMat : public cv::Mat_<T>
{

public:
    /// Default constructable for possible in use in vectors and arrays
    CMat() = default;

    ////////////////////////////////////////
    ///Use perfect forwarding for compatiblity with cv::Mat_<T> as well as MatExpr functions and constructors.
    /// As base class accept r-values and l-value references we want to use perfect forwarding to efficiently solve this
    template<typename U> CMat(U&& expr) : cv::Mat_<T>(std::forward<U>(expr)) { }

    /// Constructs n-dim matrix on top of user-allocated data. steps are in bytes(!!!), regardless of the type
    /// Each entry in p_dimensions array is a dimension and its value is its length.
    /// e.g. a vector of std::vector<int>vec{3,1,3} equals a 3-d array with dimensions 3, 1 and 3. We use integer here to follow the specification of cv::Mat_
    CMat(const std::vector<int>& p_dimensions, T* p_data, const size_t* _steps = 0) : cv::Mat_<T>(p_dimensions.size(), p_dimensions.data(), p_data, _steps)
    {
    }

    /// Print function using the "making new friends" idiom, that will instantiate a new function for each template instantiation of the class (as a non template function, providing deeper search for implicit conversion)
    friend std::ostream& operator<<(std::ostream& os, const CMat<T>& b)
    {
        return os << static_cast<cv::Mat_<T>>(b);
    }

};

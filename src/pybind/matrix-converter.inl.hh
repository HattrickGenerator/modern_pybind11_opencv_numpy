py::handle py::detail::type_caster<CMat<double>>::cast(CMat<double>&& p_mat, py::return_value_policy, py::handle)
{
    //Create the numpy array from custom matrix
    py::array_t<double> arr = CMatrixBinding::ToNumpyArray(p_mat);

    //Because we want to return ownership to python, we need to manually increment it's reference count
    arr.inc_ref();

    return arr;
}

bool py::detail::type_caster<CMat<double>>::load(
    py::handle src,
    bool) //We ignore boolean for convert because we always convert and never want to expose cv::Mat in C++ code
{
    if (py::isinstance<py::array_t<double>>(src))
    {
        value = CMatrixBinding::ToMat(py::cast<array_t<double>>(src));
        return true;
    }

    return false;
}

template<typename T> py::array_t<T> CMatrixBinding::ToNumpyArray(const CMat<T>& p_mat)
{
    // give memory cleanup responsibility to the Numpy array

    //Get vector of dimensions
    std::vector<ssize_t> dims;

    //This behavior rests a design decision. Ultimately we can't mimic the behavior of of numpy, as the dimensions member is always >= 2, and can't be one.
    for (int i = 0; i < p_mat.dims; ++i)
    {
        dims.push_back(p_mat.size[i]);
    }

    // Due to reference semantics of cv::Mat we want to allocate a new array, that has data implicitely shared.
    // This will increase the ref count by one. We can't do this with the this pointer, as the current matrix might go out of scope, invalidating the this pointer
    // and our possibility to call release on it.
   CMat<T>* refCountDummy = new CMat<T>(p_mat);

    // Hand over our ref counting dummy for deletion in a capsule from the python side
    py::capsule free_when_done(refCountDummy, [](void* f) { delete reinterpret_cast<CMat<T>*>(f); });

    //Not dealing with strides yet
    if (p_mat.isContinuous())
    {
        //Using a constructor only with dimensions here. "If strides are not provided, they are deduced" if strides are notprovided, they are deduced assuming C-contiguity.  Also added simplified constructors for 1-dimensionalcase."
        //From the official documentation (1.18) https://pybind11.readthedocs.io/_/downloads/en/latest/pdf/
        return py::array_t<double>(dims,                                  // shape
                                   reinterpret_cast<double*>(p_mat.data), // the data pointer
                                   free_when_done);                       // numpy array references this parent
    }
    else
    {
        throw std::logic_error(
            "Non continuous array. Not implemented yet, need strides!"); //TODOMG: implement with strides
        return py::array_t<T>();
    }
}

template<typename T> CMat<T>* CMatrixBinding::ToMat(py::array_t<T> p_arr)
{
    py::buffer_info info = p_arr.request(true);

    std::vector<int> shapeVector {info.shape.begin(), info.shape.end()};

    return new CMat<T>(shapeVector, static_cast<T*>(info.ptr));
}

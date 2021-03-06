
template <typename T>
py::array_t<T> CMatrixBinding::ToNumpyArray(cv::Mat_<T> &&p_mat) {
  // give memory cleanup responsibility to the Numpy array

  // Get vector of dimensions
  std::vector<ssize_t> dims;

  // This behavior rests a design decision. Ultimately we can't mimic the
  // behavior of of numpy, as the dimensions member is always >= 2, and can't be
  // one.
  for (int i = 0; i < p_mat.dims; ++i) {
    dims.push_back(p_mat.size[i]);
  }

  // Due to reference semantics of cv::Mat we want to allocate a new array, that
  // has data implicitely shared. This will increase the ref count by one. We
  // can't do this with the this pointer, as the current matrix might go out of
  // scope, invalidating the this pointer and our possibility to call release on
  // it.
  cv::Mat_<T> *refCountDummy;

  // This is a hack: In order to avoid segmentation faults to return data that
  // isn't owned by this matrix We check if the u-pointer in the matrix isn't
  // null. If this is the case we have an owning view! -> We return the complete
  // array. Otherwise we deep copy.
  if (p_mat.u && p_mat.isContinuous()) {
    refCountDummy = new cv::Mat_<T>(p_mat);
  } else {
    refCountDummy = new cv::Mat_<T>(p_mat.clone());
  }

  // Hand over our ref counting dummy for deletion in a capsule from the python
  // side
  py::capsule free_when_done(refCountDummy, [](void *f) {
    delete reinterpret_cast<cv::Mat_<T> *>(f);
  });

  // Using a constructor only with dimensions here. "If strides are not
  // provided, they are deduced"
  // assuming C-contiguity.  Also added simplified constructors for
  // 1-dimensionalcase." From the official documentation (1.18)
  // https://pybind11.readthedocs.io/_/downloads/en/latest/pdf/
  return py::array_t<T>(
      dims,                                       // shape
      reinterpret_cast<T *>(refCountDummy->data), // the data pointer
      free_when_done); // numpy array references this parent
}

template <typename T>
std::unique_ptr<cv::Mat_<T>> ToMat_CStyle(py::array_t<T> p_arr) {
  py::buffer_info info = p_arr.request(true);
  std::vector<py::ssize_t> shapes = info.shape;
  std::vector<int> shapeVector(info.shape.begin(), info.shape.end());

  return std::make_unique<cv::Mat_<T>>(shapeVector.size(), shapeVector.data(),
                                       static_cast<T *>(info.ptr));
}

template <typename T>
std::unique_ptr<cv::Mat_<T>> CMatrixBinding::ToMat(py::array_t<T> p_arr) {
  //////////////////////////////////////////////////////////////////////////////////////
  // C-style array is naturally supported by opencv. We only need to use
  // shapes and strides
  if (py::isinstance<py::array_t<T, py::array::c_style>>(p_arr)) {
    return ToMat_CStyle(p_arr);
  }

  //////////////////////////////////////////////////////////////////////////////////////
  // F-style array isn't naturally supported by opencv (I think?). So we
  // transpose, create the array and transpose again
  else if (py::isinstance<py::array_t<T, py::array::f_style>>(p_arr)) {
    py::array_t<T> array_T =
        py::cast<py::array_t<T>>(p_arr.attr("transpose")());

    std::unique_ptr<cv::Mat_<T>> ptr = ToMat_CStyle(array_T);

    cv::transpose(*ptr, *ptr);
    return ptr;
  }
  //////////////////////////////////////////////////////////////////////////////////////
  // Numpy arrays with any shapes and strides get copied for simplicity. A
  // copied matrix will follow c-style arrays
  else {
    py::array_t<T> array_copy = py::cast<py::array_t<T>>(p_arr.attr("copy")());

    std::unique_ptr<cv::Mat_<T>> ptr = ToMat_CStyle(array_copy);

    // We need to perform another deep copy because we can't take ownership of
    // the python array, as it only lives in our current scope
    *ptr = ptr->clone();
    return ptr;
  }
}

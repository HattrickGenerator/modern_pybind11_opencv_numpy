#include "python-import-test.hh"

TEST_F(PythonTests, importModule)
{
    m_worker->setArguments(QStringList() << "-c"
                                         << "import sys; sys.path.insert(0, './src/pybind/'); import cvmatbindpy");
}


/// Test if vector after transform stays the same
TEST_F(PythonTests, DISABLED_numpy_ndarray_shape_test)
{
    m_worker->setArguments(QStringList() << "pytestfiles/numpy_ndarray_shape_test.py");
}

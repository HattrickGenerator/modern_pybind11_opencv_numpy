# -*- coding: utf-8 -*-

import numpy as np
import ./src/pybind/cvmatbindpy as cvmatbind

array = np.arange(0,19440,1,dtype=float)
array = array.reshape((5,2,4,6,3,1,9,3))

arrayTransform = cvmatbind.transform(array, 1,0)
assert(array.shape == arrayTransform.shape)
assert( (array==arrayTransform).all() )

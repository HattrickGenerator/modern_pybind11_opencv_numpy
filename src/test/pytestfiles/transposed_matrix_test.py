# -*- coding: utf-8 -*-

import numpy as np

##Add the current directory to the search path so the shared library is found
import sys
sys.path.append('.')
import cvmatbindpy as cvmatbind

array = np.arange(0,10,1,dtype=float)
array = array.reshape((5,2))
array = array.transpose()

arrayTransform = cvmatbind.transform(array, 1,0)
assert(array.shape == arrayTransform.shape)
assert( (array==arrayTransform).all() )

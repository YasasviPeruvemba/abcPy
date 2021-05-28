# abcPy
A Python module for the [Berkeley-ABC](https://github.com/berkeley-abc/abc) logic synthesis framework.

--------
# Install

# Berkeley-ABC
The logic synthesis tool can be installed from [here](https://github.com/berkeley-abc/abc).

# pybind11
[pybind11](https://github.com/pybind/pybind11) has also been added as a submodule, hence following the instruction above, it will also be installed along with `mockturtle`

To install the system version, please go to the official [pybind11 document](http://pybind11.readthedocs.org/en/master) for further instructions.

The following command will install pybind11 using pip.
```
pip install pybind11
```

# Makefile Configure

The project is tested with `cmake 3.13.2`. [cmake](https://cmake.org/)
Export `CXX` to the compiler you like. The tested is made on `g++ 9.3.0`.

Correctly set the path to the Berkeley-ABC directory. Either export to system variable `ABC_DIR` or configure the flag within `setup.py` `-DABC_DIR=<path>`. The compilation process is dependent on this environment variable so make sure to set it correctly.

The cmake will automatically find the system Python.
To use the other Python, add cmake flags `cmake -DPYTHON_INCLUDE_DIR=<path> -DPYTHON_LIBRARIES=<static libarary>`.
For example, `-DPYTHON_INCLUDE_DIR=<path>/include/python3.7m -DPYTHON_LIBRARIES=<path>/lib/libpython3.7m.a`

# Build
```
python<version> setup.py build
python<version> setup.py install
```

The install command may require sudo permissions, instead passing the `--user` flag will install it only for the current user.

--------
# Usage

`import abcPy` like the standard Python library. For more information on its functionalities, use `help(abcPy)` after importing it.
In case the import statement indocates the absence of the module, make sure to include the .egg file generated install process into the system path. This can be done within python -
```
import sys
sys.path.append(<path_to_egg>)
import abcPy
```

--------
# Contact
Yasasvi V Peruvemba, Indian Institute of Technology Indore  \[[mail](ee170002061@iiti.ac.in)\]

Please contact me over any issue or further suggestions.


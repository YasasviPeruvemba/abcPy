#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include "global/global.h"

namespace py = pybind11;

void initAbcInterfaceAPI(py::module &);

PYBIND11_MAKE_OPAQUE(std::vector<PROJECT_NAMESPACE::IndexType>);

PYBIND11_MODULE(abcPy, m)
{
    initAbcInterfaceAPI(m);
}

/*
Copyright (c) 2015, Michael Droettboom
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/

#include "fcpy.h"
#include "doc/fcpy.h"

#include "config.h"
#include "pattern.h"
#include "version.h"


static PyObject *
Py_Config_default_config(Py_Config *self, PyObject *args, PyObject *kwds)
{
    FcConfig *config;

    config = FcInitLoadConfig();

    if (config == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "Could not load default config");
        return NULL;
    }

    return Py_Config_cnew(config);
}


static PyMethodDef module_methods[] = {
    {"default_config",
     (PyCFunction)Py_Config_default_config,
     METH_NOARGS,
     doc_fcpy_default_config},
    {NULL}  /* Sentinel */
};


PyObject *fcpy_module;


/* TODO: Hide all exported symbols in the shared library except this one */

#if PY3K
    static void
    fcpy_module_dealloc(PyObject *m)
    {

    }

    static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "_fcpy",
        NULL,
        0,
        module_methods,
        NULL,
        NULL,
        NULL,
        (freefunc)fcpy_module_dealloc
    };

    #define INITERROR return NULL

    PyMODINIT_FUNC
    PyInit__fcpy(void)
#else
    #define INITERROR return

    PyMODINIT_FUNC
    init_fcpy(void)
#endif
{
    if (!FcInit()) {
        PyErr_SetString(PyExc_RuntimeError, "Could not initialize fontconfig");
        INITERROR;
    }

#if PY3K
    fcpy_module = PyModule_Create(&moduledef);
#else
    fcpy_module = Py_InitModule3(
        "_fcpy", module_methods,
        "fontconfig bindings");
#endif

    if (fcpy_module == NULL) {
        INITERROR;
    }

    if (setup_Config(fcpy_module) ||
        setup_Pattern(fcpy_module) ||
        setup_pyutil(fcpy_module) ||
        setup_version(fcpy_module)) {
        INITERROR;
    }

    #if PY3K
    return fcpy_module;
    #endif
}

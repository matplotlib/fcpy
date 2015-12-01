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

#include "pyutil.h"


static const char *qualified_name_to_name(const char *qualified_name)
{
    const char *name = qualified_name;
    for (; *name != 0 && *name != '.'; ++name) {
        ;
    }
    if (*name == '.') {
        ++name;
    }

    return name;
}


static int
generic_traverse(fcpy_Object *self, visitproc visit, void *arg)
{
    Py_VISIT(self->owner);

    return 0;
}


static int
generic_clear(fcpy_Object *self)
{
    PyObject *tmp;

    tmp = self->owner;
    self->owner = NULL;
    Py_XDECREF(tmp);

    return 0;
}


void fcpy_Object_dealloc(PyObject* self)
{
    Py_TYPE(self)->tp_clear((PyObject*)self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


PyObject *fcpy_Object_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    fcpy_Object *self;

    self = (fcpy_Object *)type->tp_alloc(type, 0);
    if (self == NULL) {
        return NULL;
    }
    self->owner = NULL;
    return (PyObject *)self;
}


int fcpy_setup_type(PyObject *module, PyTypeObject *type)
{
    const char *name;

    if (type->tp_dealloc == NULL) {
        type->tp_dealloc = (destructor)fcpy_Object_dealloc;
    }

    if (type->tp_flags == 0) {
        type->tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC;
    }

    if (type->tp_traverse == NULL) {
        type->tp_traverse = (traverseproc)generic_traverse;
    }

    if (type->tp_clear == NULL) {
        type->tp_clear = (inquiry)generic_clear;
    }

    if (type->tp_new == NULL) {
        type->tp_new = fcpy_Object_new;
    }

    if (PyType_Ready(type) < 0) {
        return -1;
    }

    Py_INCREF(type);

    if (module == NULL) {
        return 0;
    } else {
        name = qualified_name_to_name(type->tp_name);
        if (PyModule_AddObject(module, name, (PyObject *)type)) {
            Py_DECREF(type);
            return -1;
        }
        return 0;
    }
}


int setup_pyutil(PyObject *m)
{
    return 0;
}


PyObject *
fcpy_strlist_to_python(FcStrList *list)
{
    FcChar8 *item;
    PyObject *result = NULL;
    PyObject *pyitem = NULL;

    result = PyList_New(0);
    if (result == NULL) {
        return NULL;
    }

    FcStrListFirst(list);

    while (item = FcStrListNext(list)) {
        pyitem = PyBytes_FromString(item);
        if (pyitem == NULL) {
            Py_DECREF(result);
            return NULL;
        }

        if (PyList_Append(result, pyitem)) {
            Py_DECREF(result);
            Py_DECREF(pyitem);
            return NULL;
        }

        Py_DECREF(pyitem);
    }

    FcStrListDone(list);

    return result;
}

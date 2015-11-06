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

#ifndef __PYUTIL_H__
#define __PYUTIL_H__

#include "fcpy.h"


#define STRINGIFY(s) XSTRINGIFY(s)
#define XSTRINGIFY(s) #s


typedef struct {
    PyObject_HEAD
    PyObject *owner;
} fcpy_Object;


int fcpy_setup_type(PyObject *m, PyTypeObject *type);


int setup_pyutil(PyObject *m);


void fcpy_Object_dealloc(PyObject* self);


PyObject *fcpy_Object_new(PyTypeObject *type, PyObject *args, PyObject *kwds);


#define MAKE_GETTER(pytype, name, convert_func, member) \
    static PyObject * name ## _get(pytype *self, PyObject *closure) \
    {                                           \
        return convert_func(member);      \
    }                                           \


#define DEF_GETTER(name, doc)                \
  { #name, (getter)name ## _get, NULL, (doc) }


#define DEF_METHOD(name, type) \
    { #name, (PyCFunction)Py_ ## type ## _ ## name, METH_VARARGS|METH_KEYWORDS, doc_ ## type ## _ ## name }


#define DEF_METHOD_NOARGS(name, type) \
    { #name, (PyCFunction)Py_ ## type ## _ ## name, METH_NOARGS, doc_ ## type ## _ ## name }


#endif

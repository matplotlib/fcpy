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

#include "pattern.h"
#include "doc/pattern.h"
#include "result.h"


#define PATTERN_METHOD(name) DEF_METHOD(name, Pattern)
#define PATTERN_METHOD_NOARGS(name) DEF_METHOD_NOARGS(name, Pattern)


PyTypeObject Py_Pattern_Type;


static FcBool
_Py_Pattern_add(FcPattern *pattern, char *object, PyObject *value);


/****************************************************************************
 Object basics
*/


static void
Py_Pattern_dealloc(Py_Pattern* self)
{
    FcPatternDestroy(self->x);
    Py_TYPE(self)->tp_clear((PyObject*)self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyObject *
Py_Pattern_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Py_Pattern *self;

    self = (Py_Pattern *)fcpy_Object_new(type, args, kwds);
    if (self == NULL) {
        return NULL;
    }
    self->x = NULL;
    return (PyObject *)self;
}


PyObject *
Py_Pattern_cnew(FcPattern *pattern)
{
    Py_Pattern *self;

    self = (Py_Pattern *)(&Py_Pattern_Type)->tp_alloc(&Py_Pattern_Type, 0);
    if (self == NULL) {
        return NULL;
    }
    self->x = pattern;
    return (PyObject *)self;
}


static int
Py_Pattern_init(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    PyObject *py_pattern;
    PyObject *key;
    PyObject *value;
    char *object;
    Py_ssize_t pos;

    /* TODO: Handle kwarg init */

    if (PyTuple_Size(args) == 1 &&
        PyDict_Size(kwds) == 0) {

        py_pattern = PyTuple_GetItem(args, 0);
        if (PyBytes_Check(py_pattern)) {
            self->x = FcNameParse(
                (unsigned char *)PyBytes_AsString(py_pattern));
            goto exit;
        } else if (PyUnicode_Check(py_pattern)) {
            self->x = FcNameParse(
                (unsigned char *)PyUnicode_AsUTF8(py_pattern));
            goto exit;
        }

    } else if (PyTuple_Size(args) == 0 &&
               PyDict_Size(kwds) == 0) {

        self->x = FcPatternCreate();
        goto exit;

    } else if (PyTuple_Size(args) == 0 &&
               PyDict_Size(kwds) > 0) {

        self->x = FcPatternCreate();
        if (self->x == NULL) {
            goto exit;
        }

        pos = 0;
        while (PyDict_Next(kwds, &pos, &key, &value)) {
            if (PyBytes_Check(key)) {
                object = PyBytes_AsString(key);
            } else if (PyUnicode_Check(key)) {
                object = PyUnicode_AsUTF8(key);
            } else {
                PyErr_SetString(PyExc_ValueError, "Invalid key");
                return -1;
            }

            if (_Py_Pattern_add(self->x, object, value)) {
                if (!PyErr_Occurred()) {
                    PyErr_Format(
                        PyExc_ValueError, "Error setting '%s'", object);
                }
                return -1;
            }
        }
        goto exit;

    }

 exit:

    if (self->x == NULL) {
        PyErr_SetString(
            PyExc_MemoryError, "Could not create pattern");
        return -1;
    }

    return 0;
}


static long
Py_Pattern_hash(Py_Pattern *self)
{
    return (long)FcPatternHash(self->x);
}


static PyObject *
Py_Pattern_richcompare(PyObject *o1, PyObject *o2, int op)
{
    Py_Pattern *p1;
    Py_Pattern *p2;

    if (op == Py_EQ &&
        Py_TYPE(o1) == &Py_Pattern_Type &&
        Py_TYPE(o2) == &Py_Pattern_Type) {
        p1 = (Py_Pattern *)o1;
        p2 = (Py_Pattern *)o2;
        if (FcPatternEqual(p1->x, p2->x)) {
            Py_RETURN_TRUE;
        } else {
            Py_RETURN_FALSE;
        }
    }

    Py_INCREF(Py_NotImplemented);
    return Py_NotImplemented;
}


/* TODO: FcPatternEqual */
/* TODO: FcPatternEqualSubset */

/* TODO: FcPatternAdd -- use dict-like interface */
/* TODO: FcPatternGet -- use dict-like interface */
/* TODO: FcPatternDel -- use dict-like interface */

/* TODO: FcDefaultSubstitute */
/* TODO: FcPatternFormat */


/****************************************************************************
 repr
*/


static PyObject *
Py_Pattern_repr(Py_Pattern *self)
{
    return PyUnicode_FromString((char *)FcNameUnparse(self->x));
}


/****************************************************************************
 Methods
*/


static FcBool
_Py_Pattern_add(FcPattern *pattern, char *object, PyObject *value)
{
    int i;
    double d;
    char *s;

    if (PyLong_Check(value)) {
        i = PyLong_AsLong(value);
        return !FcPatternAddInteger(pattern, object, i);
    }
    #if !PY3K
    else if (PyInt_Check(value)) {
        i = PyInt_AsLong(value);
        return !FcPatternAddInteger(pattern, object, i);
    }
    #endif
    else if (PyFloat_Check(value)) {
        d = PyFloat_AsDouble(value);
        return !FcPatternAddDouble(pattern, object, d);
    } else if (PyBytes_Check(value)) {
        s = PyBytes_AsString(value);
        return !FcPatternAddString(pattern, object, (unsigned char *)s);
    } else if (PyUnicode_Check(value)) {
        s = PyUnicode_AsUTF8(value);
        return !FcPatternAddString(pattern, object, (unsigned char *)s);
    }

    PyErr_SetString(PyExc_TypeError, "Unknown type");
    return 1;
}


static PyObject *
Py_Pattern_add(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    char *object;
    PyObject *value;

    FcBool result;

    static char *kwlist[] = {"object", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "sO:add", kwlist,
            &object, &value)) {
        return NULL;
    }

    result = _Py_Pattern_add(self->x, object, value);

    if (result) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Could not add value");
        }
        return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject *
Py_Pattern_delete(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    char *object;

    static char *kwlist[] = {"object", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "s:delete", kwlist,
            &object)) {
        return NULL;
    }

    FcPatternDel(self->x, object);

    Py_RETURN_NONE;
}


static PyObject *
Py_Pattern_format(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    char *format;
    char *output;
    PyObject *result;

    static char *kwlist[] = {"format", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "sO:format", kwlist,
            &format)) {
        return NULL;
    }

    output = (char *)FcPatternFormat(self->x, (unsigned char *)format);

    if (output == NULL) {
        PyErr_SetString(PyExc_ValueError, "Invalid format");
        return NULL;
    }

    result = PyUnicode_FromString(output);
    free(output);
    return result;
}


static PyObject *
Py_Pattern_get(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    char *object;
    int n;
    FcChar8 *value;

    FcBool result;

    static char *kwlist[] = {"object", "index", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "si:get", kwlist,
            &object, &n)) {
        return NULL;
    }

    result = FcPatternGetString(self->x, object, n, &value);

    if (result == FcResultMatch) {
        return PyUnicode_FromString((char *)value);
    } else {
        fcpy_result_to_exception(result);
        return NULL;
    }
}


static PyObject *
Py_Pattern_set(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    char *object;
    PyObject *value;

    FcBool result;

    static char *kwlist[] = {"object", "value", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "sO:set", kwlist,
            &object, &value)) {
        return NULL;
    }

    FcPatternDel(self->x, object);

    result = _Py_Pattern_add(self->x, object, value);

    if (result) {
        if (!PyErr_Occurred()) {
            PyErr_SetString(PyExc_ValueError, "Could not set value");
        }
        return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject *
Py_Pattern_substitute(Py_Pattern *self, PyObject *args, PyObject *kwds)
{
    FcDefaultSubstitute(self->x);

    Py_RETURN_NONE;
}


static PyMethodDef Py_Pattern_methods[] = {
    PATTERN_METHOD(add),
    PATTERN_METHOD(delete),
    PATTERN_METHOD(format),
    PATTERN_METHOD(get),
    PATTERN_METHOD(set),
    PATTERN_METHOD_NOARGS(substitute),
    {NULL}  /* Sentinel */
};


/****************************************************************************
 Setup
*/


int setup_Pattern(PyObject *m)
{
    memset(&Py_Pattern_Type, 0, sizeof(PyTypeObject));
    Py_Pattern_Type = (PyTypeObject) {
        .tp_name = "fcpy.Pattern",
        .tp_basicsize = sizeof(Py_Pattern),
        .tp_dealloc = (destructor)Py_Pattern_dealloc,
        .tp_doc = doc_Pattern__init__,
        .tp_methods = Py_Pattern_methods,
        .tp_repr = (unaryfunc)Py_Pattern_repr,
        .tp_init = (initproc)Py_Pattern_init,
        .tp_new = Py_Pattern_new,
        .tp_hash = (hashfunc)Py_Pattern_hash,
        .tp_richcompare = (richcmpfunc)Py_Pattern_richcompare
    };

    fcpy_setup_type(m, &Py_Pattern_Type);

    return 0;
}

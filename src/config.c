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

#include "config.h"
#include "doc/config.h"
#include "pattern.h"
#include "result.h"


#define CONFIG_METHOD(name) DEF_METHOD(name, Config)
#define CONFIG_METHOD_NOARGS(name) DEF_METHOD_NOARGS(name, Config)


PyTypeObject Py_Config_Type;


/****************************************************************************
 Object basics
*/


static void
Py_Config_dealloc(Py_Config* self)
{
    FcConfigDestroy(self->x);
    Py_TYPE(self)->tp_clear((PyObject*)self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}


static PyObject *
Py_Config_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    Py_Config *self;

    self = (Py_Config *)fcpy_Object_new(type, args, kwds);
    if (self == NULL) {
        return NULL;
    }
    self->x = NULL;
    return (PyObject *)self;
}


PyObject *
Py_Config_cnew(FcConfig *config)
{
    Py_Config *self;
    self = (Py_Config *)(&Py_Config_Type)->tp_alloc(&Py_Config_Type, 0);

    if (self == NULL) {
        return NULL;
    }
    self->x = config;
    return (PyObject *)self;
}


static int
Py_Config_init(Py_Config *self, PyObject *args, PyObject *kwds)
{
    if (!PyArg_ParseTuple(args, ":Config")) {
        return -1;
    }

    self->x = FcConfigCreate();

    if (self->x == NULL) {
        PyErr_SetString(
            PyExc_RuntimeError, "Could not create config");
        return -1;
    }

    return 0;
}


/****************************************************************************
 Methods
*/


static PyObject*
Py_Config_add_dir(Py_Config *self, PyObject *args, PyObject *kwds)
{
    /* TODO: Handle Unicode file encoding madness */

    unsigned char *dir;

    static char *kwlist[] = {"dir", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "s:add_dir", kwlist,
            &dir)) {
        return NULL;
    }

    if (!FcConfigAppFontAddDir(self->x, dir)) {
        PyErr_SetString(PyExc_ValueError, "Could not add directory");
        return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject*
Py_Config_add_file(Py_Config *self, PyObject *args, PyObject *kwds)
{
    /* TODO: Handle Unicode file encoding madness */

    unsigned char *file;

    static char *kwlist[] = {"file", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "s:add_font", kwlist,
            &file)) {
        return NULL;
    }

    if (!FcConfigAppFontAddFile(self->x, file)) {
        PyErr_SetString(PyExc_ValueError, "Could not add font");
        return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject*
Py_Config_build_fonts(Py_Config *self, PyObject *args, PyObject *kwds)
{
    FcBool result;

    result = FcConfigBuildFonts(self->x);

    if (!result) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject*
Py_Config_clear(Py_Config *self, PyObject *args, PyObject *kwds)
{
    FcConfigAppFontClear(self->x);

    Py_RETURN_NONE;
}


static PyObject*
Py_Config_get_config_files(Py_Config *self, PyObject *args, PyObject *kwds)
{
    FcStrList *dirs;

    dirs = FcConfigGetConfigFiles(self->x);

    if (dirs == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Could not get config files");
        return NULL;
    }

    return fcpy_strlist_to_python(dirs);
}


static PyObject*
Py_Config_get_font_dirs(Py_Config *self, PyObject *args, PyObject *kwds)
{
    FcStrList *dirs;

    dirs = FcConfigGetConfigDirs(self->x);

    if (dirs == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "Could not get config dirs");
        return NULL;
    }

    return fcpy_strlist_to_python(dirs);
}


static PyObject*
Py_Config_substitute(Py_Config *self, PyObject *args, PyObject *kwds)
{
    PyObject *py_pattern;

    static char *kwlist[] = {"pattern", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "O:substitute", kwlist,
            &py_pattern)) {
        return NULL;
    }

    if (Py_TYPE(py_pattern) != &Py_Pattern_Type) {
        PyErr_SetString(PyExc_TypeError, "pattern must be Pattern object");
        return NULL;
    }

    if (!FcConfigSubstitute(self->x, ((Py_Pattern *)py_pattern)->x, FcMatchFont)) {
        PyErr_SetString(PyExc_MemoryError, "Out of memory");
        return NULL;
    }

    Py_RETURN_NONE;
}


static PyObject*
Py_Config_match(Py_Config *self, PyObject *args, PyObject *kwds)
{
    PyObject *py_pattern;
    FcPattern *match = NULL;
    FcResult result;

    static char *kwlist[] = {"pattern", NULL};

    if (!PyArg_ParseTupleAndKeywords(
            args, kwds, "O:match", kwlist,
            &py_pattern)) {
        return NULL;
    }

    if (Py_TYPE(py_pattern) != &Py_Pattern_Type) {
        PyErr_SetString(PyExc_TypeError, "pattern must be Pattern object");
        return NULL;
    }

    match = FcFontMatch(self->x, ((Py_Pattern *)py_pattern)->x, &result);

    if (result == FcResultMatch) {
        return Py_Pattern_cnew(match);
    } else {
        FcPatternDestroy(match);
        fcpy_result_to_exception(result);
        return NULL;
    }
}


static PyMethodDef Py_Config_methods[] = {
    CONFIG_METHOD(add_dir),
    CONFIG_METHOD(add_file),
    CONFIG_METHOD_NOARGS(build_fonts),
    CONFIG_METHOD_NOARGS(clear),
    CONFIG_METHOD_NOARGS(get_config_files),
    CONFIG_METHOD_NOARGS(get_font_dirs),
    CONFIG_METHOD(substitute),
    CONFIG_METHOD(match),
    {NULL}  /* Sentinel */
};


/****************************************************************************
 Setup
*/


int setup_Config(PyObject *m)
{
    memset(&Py_Config_Type, 0, sizeof(PyTypeObject));
    Py_Config_Type = (PyTypeObject) {
        .tp_name = "fcpy.Config",
        .tp_basicsize = sizeof(Py_Config),
        .tp_dealloc = (destructor)Py_Config_dealloc,
        .tp_doc = doc_Config__init__,
        .tp_methods = Py_Config_methods,
        .tp_init = (initproc)Py_Config_init,
        .tp_new = Py_Config_new
    };

    fcpy_setup_type(m, &Py_Config_Type);

    return 0;
}

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

#include "fc_constants.h"
#include "doc/fc_constants.h"
#include "constants.h"


ftpy_ConstantType Py_WEIGHT_ConstantType;

static PyTypeObject Py_WEIGHT_Type;
#define WEIGHT_CONST(name) DEF_CONST(name, FC_WEIGHT)
static constant_def WEIGHT_constants[] = {
    WEIGHT_CONST(LIGHT),
    WEIGHT_CONST(MEDIUM),
    WEIGHT_CONST(DEMIBOLD),
    WEIGHT_CONST(BOLD),
    WEIGHT_CONST(BLACK),
    {NULL}
};


ftpy_ConstantType Py_SLANT_ConstantType;

static PyTypeObject Py_SLANT_Type;
#define SLANT_CONST(name) DEF_CONST(name, FC_SLANT)
static constant_def SLANT_constants[] = {
    SLANT_CONST(ROMAN),
    SLANT_CONST(ITALIC),
    SLANT_CONST(OBLIQUE),
    {NULL}
};


ftpy_ConstantType Py_SPACING_ConstantType;

static PyTypeObject Py_SPACING_Type;
#define SPACING_CONST(name) DEF_CONST(name, FC)
static constant_def SPACING_constants[] = {
    SPACING_CONST(PROPORTIONAL),
    SPACING_CONST(MONO),
    SPACING_CONST(CHARCELL),
    {NULL}
};


int
setup_constants(PyObject *m)
{
  return (
      define_constant_namespace(
          m, &Py_WEIGHT_Type, &Py_WEIGHT_ConstantType,
          "fcpy.WEIGHT",
          doc_WEIGHT, WEIGHT_constants) ||
      define_constant_namespace(
          m, &Py_SLANT_Type, &Py_SLANT_ConstantType,
          "fcpy.SLANT",
          doc_SLANT, SLANT_constants) ||
      define_constant_namespace(
          m, &Py_SPACING_Type, &Py_SPACING_ConstantType,
          "fcpy.SPACING",
          doc_SPACING, SPACING_constants)
      );
}

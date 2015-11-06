# -*- coding: utf-8 -*-

# Copyright (c) 2015, Michael Droettboom All rights reserved.

# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:

# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.

# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# The views and conclusions contained in the software and
# documentation are those of the authors and should not be interpreted
# as representing official policies, either expressed or implied, of
# the FreeBSD Project.

from __future__ import print_function, unicode_literals, absolute_import

Pattern__init__ = """
A pattern to match against available fonts, or information about a
font.

May be initialized in three ways:

- No arguments: Create an empty pattern.

- One string argument: The argument is a fontconfig string in the
  format passed to ``fc-match``.  It will be parsed, and its
  attributes set on the new pattern.

- Keyword arguments: Set attributes on the new pattern.
"""

Pattern_add = """
Add a value to a pattern.

Adds a single value to end of the list of values associated with the
property named object.

Parameters
----------
object : str
    TODO: Define

value : any
    TODO: Define types
"""

Pattern_delete = """
Delete all values of a given type from a pattern.

Parameters
----------
object : str
    TODO: Define
"""

Pattern_format = """
Format a pattern into a string according to a format specifier.

Converts given pattern ``pat`` into text described by the format
specifier ``format``.

The format is loosely modeled after ``printf``-style format string.
The format string is composed of zero or more directives: ordinary
characters (not "``%``"), which are copied unchanged to the output
stream; and tags which are interpreted to construct text from the
pattern in a variety of ways (explained below).  Special characters
can be escaped using backslash. C-string style special characters like
``\n`` and ``\r`` are also supported. It is advisable to always escape
curly braces that are meant to be copied to the output as ordinary
characters.

Each tag is introduced by the character "``%``", followed by an
optional minimum field width, followed by tag contents in curly braces
(``{}``). If the minimum field width value is provided, the tag will
be expanded and the result padded to achieve the minimum width. If the
minimum field width is positive, the padding will right-align the
text. Negative field width will left-align. The rest of this section
describes various supported tag contents and their expansion.

A simple tag is one where the content is an identifier. When simple
tags are expanded, the named identifier will be looked up in pattern
and the resulting list of values returned, joined together using
comma. For example, to print the family name and style of the pattern,
use the format "``%{family} %{style}\n``". To extend the family column
to forty characters use "``%-40{family}%{style}\n``".

Simple tags expand to list of all values for an element. To only
choose one of the values, one can index using the syntax
"``%{elt[idx]}``". For example, to get the first family name only, use
"``%{family[0]}``".

If a simple tag ends with "``=``" and the element is found in the
pattern, the name of the element followed by "``=``" will be output
before the list of values. For example, "``%{weight=}``" may expand to
the string "``weight=80``". Or to the empty string if pattern does not
have weight set.

If a simple tag starts with "``:``" and the element is found in the
pattern, "``:``" will be printed first. For example, combining this
with the ``=``, the format "``%{:weight=}``" may expand to
"``:weight=80``" or to the empty string if pattern does not have
weight set.

If a simple tag contains the string "``:-``", the rest of the the tag
contents will be used as a default string. The default string is
output if the element is not found in the pattern.  For example, the
format "``%{:weight=:-123}``" may expand to "``:weight=80``" or to the
string "``:weight=123``" if pattern does not have weight set.

A count tag is one that starts with the character "``#``" followed by
an element name, and expands to the number of values for the element
in the pattern. For example, "``%{#family}``" expands to the number of
family names pattern has set, which may be zero.

A sub-expression tag is one that expands a sub-expression. The tag
contents are the sub-expression to expand placed inside another set of
curly braces. Sub-expression tags are useful for aligning an entire
sub-expression, or to apply converters (explained later) to the entire
sub-expression output. For example, the format "``%40{{%{family}
%{style}}}``" expands the sub-expression to construct the family name
followed by the style, then takes the entire string and pads it on the
left to be at least forty characters.

A filter-out tag is one starting with the character "``-``" followed
by a comma-separated list of element names, followed by a
sub-expression enclosed in curly braces. The sub-expression will be
expanded but with a pattern that has the listed elements removed from
it. For example, the format "``%{-size,pixelsize{sub-expr}}``" will
expand "sub-expr" with pattern sans the size and pixelsize elements.

A filter-in tag is one starting with the character "``+``" followed by
a comma-separated list of element names, followed by a sub-expression
enclosed in curly braces. The sub-expression will be expanded but with
a pattern that only has the listed elements from the surrounding
pattern. For example, the format "``%{+family,familylang{sub-expr}}``"
will expand "sub-expr" with a sub-pattern consisting only the family
and family lang elements of pattern.

A conditional tag is one starting with the character "``?``" followed
by a comma-separated list of element conditions, followed by two
sub-expression enclosed in curly braces. An element condition can be
an element name, in which case it tests whether the element is defined
in pattern, or the character "``!``" followed by an element name, in
which case the test is negated. The conditional passes if all the
element conditions pass. The tag expands the first sub-expression if
the conditional passes, and expands the second sub-expression
otherwise. For example, the format
"``%{?size,dpi,!pixelsize{pass}{fail}}``" will expand to "``pass``" if
pattern has ``size`` and ``dpi`` elements but no ``pixelsize``
element, and to "``fail``" otherwise.

An enumerate tag is one starting with the string "``[]``" followed by
a comma-separated list of element names, followed by a sub-expression
enclosed in curly braces. The list of values for the named elements
are walked in parallel and the sub-expression expanded each time with
a pattern just having a single value for those elements, starting from
the first value and continuing as long as any of those elements has a
value.  For example, the format "%``{[]family,familylang{%{family}
(%{familylang})\n}}``" will expand the pattern "``%{family}
(%{familylang})\n``" with a pattern having only the first value of the
``family`` and ``familylang`` elements, then expands it with the
second values, then the third, etc.

As a special case, if an enumerate tag has only one element, and that
element has only one value in the pattern, and that value is of type
``FcLangSet``, the individual languages in the language set are
enumerated.

A builtin tag is one starting with the character "``=``" followed
by a builtin name. The following builtins are defined:

- ``unparse``: Expands to the result of calling ``FcNameUnparse()`` on
  the pattern.

- ``fcmatch``: Expands to the output of the default output format of
  the ``fc-match`` command on the pattern, without the final newline.

- ``fclist``: Expands to the output of the default output format of
  the ``fc-list`` command on the pattern, without the final
  newline.

- ``fccat``: Expands to the output of the default output format of
  the ``fc-cat`` command on the pattern, without the final
  newline.

- ``pkgkit``: Expands to the list of PackageKit ``font()`` tags for
  the pattern. Currently this includes tags for each family name, and
  each language from the pattern, enumerated and sanitized into a set
  of tags terminated by newline.  Package management systems can use
  these tags to tag their packages accordingly.

For example, the format "``%{+family,style{%{=unparse}}}\n``" will
expand to an unparsed name containing only the ``family`` and
``style`` element values from pattern.

The contents of any tag can be followed by a set of zero or more
converters. A converter is specified by the character "``|``" followed
by the converter name and arguments. The following converters are
defined:

- ``basename``: Replaces text with the results of calling
  ``FcStrBasename()`` on it.

- ``dirname``: Replaces text with the results of calling
  ``FcStrDirname()`` on it.

- ``downcase``: Replaces text with the results of calling
  ``FcStrDowncase()`` on it.

- ``shescape``: Escapes text for one level of shell
  expansion. (Escapes single-quotes, also encloses text in
  single-quotes.)

- ``cescape``: Escapes text such that it can be used as part of a C
  string literal. (Escapes backslash and double-quotes.)

- ``xmlescape``: Escapes text such that it can be used in XML and
  HTML.  (Escapes less-than, greater-than, and ampersand.)

- ``delete(chars)``: Deletes all occurrences of each of the characters
  in chars from the text. This converter is not UTF-8 aware yet.

- ``escape(chars)``: Escapes all occurrences of each of the characters
  in chars by prepending it by the first character in chars.  This
  converter is not UTF-8 aware yet.

- ``translate(from,to)``: Translates all occurrences of each of the
  characters in from by replacing them with their corresponding
  character in to. If to has fewer characters than from, it will be
  extended by repeating its last character.  This converter is not
  UTF-8 aware yet.

For example, the format "``%{family|downcase|delete( )}\n``" will
expand to the values of the family element in pattern, lower-cased and
with spaces removed.
"""

Pattern_get = """
Gets a value.

Parameters
----------
object : str

index : int

Result
------
value : str
    Always returns a string.  It is up to the caller to convert to the
    appropriate data type.
"""


Pattern_set = """
Sets a given object to a given value.

Equivalent to `delete` followed by `add`.

Parameters
----------
object : str
    TODO: Define

value : any
    TODO: Define types
"""

Pattern_substitute = """
Perform default substitutions in-place.

Supplies default values for underspecified font patterns:

- Patterns without a specified style or weight are set to Medium

- Patterns without a specified style or slant are set to Roman

- Patterns without a specified pixel size are given one computed from
  any specified point size (default 12), dpi (default 75) and scale
  (default 1).
"""

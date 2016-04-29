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

Config__init__ = """
A font configuration.

A set of settings that create a set of fonts to find matches in.
"""

Config_add_dir = """
Add fonts from a directory to the font database.

Scans the specified directory for fonts, adding each one found
to the application-specific set of fonts.

Parameters
----------
dir : str
    Path to a directory of fonts

Raises
------
ValueError :
    If the fonts cannot be added (due to allocation failure or no
    fonts found).
"""

Config_add_file = """
Add font file to the font database.

Parameters
----------
file : str
    Path to a font file

Raises
------
ValueError :
    If the font cannot be added (due to allocation failure or no
    font found).
"""

Config_add_pattern = """
Add a pattern to the application-specific font database.  Useful for adding
fonts that fontconfig doesn't index on its own.

Parameters
----------
pattern : Pattern
    The font pattern to add.
"""

Config_build_fonts = """
Build font database.

Builds the set of available fonts for the given configuration.
Note that any changes to the configuration after this call have
indeterminate effects.

Raises
------
MemoryError :
    If this operation runs out of memory.
"""

Config_clear = """
Clears the set of application-specific fonts.
"""

Config_get_config_files = """
Get the list of config files.
"""

Config_get_font_dirs = """
Get the list of font directories.

This includes the configured font directories along with any
directories below those in the filesystem.
"""

Config_match = """
Return the best font.

Finds the font in the active sets most closely matching the given
pattern.

This function should only be called after `Config.substitute` and
`Pattern.substitute` have been called for *pattern*, otherwise the
results will not be correct.

Parameters
----------
pattern : Pattern

Returns
-------
match : Pattern
"""

Config_substitute = """
Execute substitutions.

Parameters
----------
pattern : Pattern

Raises
------
ValueError :
    Substitution can not be performed.
"""

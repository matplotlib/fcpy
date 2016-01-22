Introduction to fcpy
====================

Usage consists of two distinct phases: (1) setting up a `Config`
object and adding directories and files to it, and (2) querying that
object for font matches.

For example, setting up the `Config` object::

  import fcpy

  fc = fcpy.default_config()
  fc.add_dir('/my/custom/dir/of/fonts')
  fc.build_fonts()

And then querying for a font::

  pattern = fc.Pattern()
  pattern.set('family', ['Arial'])
  pattern.set('style', 'italic')

  # Apply substitutions to fill in defaults
  fc.substitute(pattern)
  pattern.substitute()

  match = fc.match(pattern)

  filename = match.get('file')

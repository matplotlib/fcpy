.. _properties:

Properties
==========

The following is the list of font properties understood by fontconfig:

=============== ======= =========================================================
Property        Type    Description
=============== ======= =========================================================
family          String  Font family names
familylang      String  Languages corresponding to each family
style           String  Font style. Overrides weight and slant
stylelang       String  Languages corresponding to each style
fullname        String  Font full names (often includes style)
fullnamelang    String  Languages corresponding to each fullname
slant           Int     Italic, oblique or roman
weight          Int     Light, medium, demibold, bold or black
size            Double  Point size
width           Int     Condensed, normal or expanded
aspect          Double  Stretches glyphs horizontally before hinting
pixelsize       Double  Pixel size
spacing         Int     Proportional, dual-width, monospace or charcell
foundry         String  Font foundry name
antialias       Bool    Whether glyphs can be antialiased
hinting         Bool    Whether the rasterizer should use hinting
hintstyle       Int     Automatic hinting style
verticallayout  Bool    Use vertical layout
autohint        Bool    Use autohinter instead of normal hinter
globaladvance   Bool    Use font global advance data (deprecated)
file            String  The filename holding the font
index           Int     The index of the font within the file
ftface          FT_Face Use the specified FreeType face object
rasterizer      String  Which rasterizer is in use (deprecated)
outline         Bool    Whether the glyphs are outlines
scalable        Bool    Whether glyphs can be scaled
color           Bool    Whether any glyphs have color
scale           Double  Scale factor for point->pixel conversions (deprecated)
dpi             Double  Target dots per inch
rgba            Int     unknown, rgb, bgr, vrgb, vbgr,
                        none - subpixel geometry
lcdfilter       Int     Type of LCD filter
minspace        Bool    Eliminate leading from line spacing
charset         CharSet Unicode chars encoded by the font
lang            String  List of RFC-3066-style languages this
                        font supports
fontversion     Int     Version number of the font
capability      String  List of layout capabilities in the font
fontformat      String  String name of the font format
embolden        Bool    Rasterizer should synthetically embolden the font
embeddedbitmap  Bool    Use the embedded bitmap instead of the outline
decorative      Bool    Whether the style is a decorative variant
fontfeatures    String  List of the feature tags in OpenType to be enabled
namelang        String  Language name to be used for the default value of
                        familylang, stylelang, and fullnamelang
prgname         String  String  Name of the running program
postscriptname  String  Font family name in PostScript
=============== ======= =========================================================

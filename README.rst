Image match palette
===================

Convert image to match the given color palette.

How it works
------------

Swaps every pixel with the closest one from the palette, or blends it by
the blend factor.

Usage
-----

.. code-block:: bash

    image_match_palette <input_path.png> <output_path.png> <palette_path> [blend 0.0-1.0, default 1.0]

You can append a blend factor at the end (from 0.0 to 1.0), but I
personally prefer to keep it solid.

Example:

.. code-block:: bash

    image_match_palette palatte-gruvbox.txt in.png out.png

Build instructions
==================

Prerequisites
-------------
 - c++17 compiler
 - Boost GIL library

Build with Make
---------------

.. code-block:: bash

  make

Build with CMake
----------------

.. code-block:: bash

  cmake -S. -Bbuild
  cmake --build build

Notes
=====

- Currently 8-bit RGB PNG file type is hard coded

Splat-Generator
===============

This repo. contains 4 files namely:
  1. main.cpp
  2. plyloader.h
  3. plyLoader_C_Binary.cpp
  4. male_binary.ply
  5. SplatGenerator.exe 

plyloader_C_Binary.cpp is the fle having all neccessary function needed to read a .PLY point cloud file and generate a splatted .PLY file, so that it can be rendered using BLENDER.
male_binary.ply : point cloud file.

.exe usage from windows command line
SplatGenerator.exe inputBinaryPlyFile.ply outputfile.ply
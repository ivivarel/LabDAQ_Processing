# LabDAQ_Processing
A simple package containing examples and code snippets for the "Laboratory of Data Acquisition and Processing" of the Degree in Physics - University of Bologna

- The file `CodeFitting.cxx` contains a simple example of a three-layer tracking measuring a linear track with fixed intercept and slope. The measurement introduced a gaussian smearing of the coordinates, and it can also introduce further distortions depending on the "mode" chosen for the detector. To use it, open root, load it with .L CodeFitting.cxx+ (note the + at the end which forces compilation), then run with CodeFitting(i) (where i specifies the mode). 

Available modes are: 
  - 0 - only the nominal gaussian smearing is applied.
  - 1 - an additional gaussian smearing is applied on top of the nominal 1.
  - 2 - some bias is applied to the hit position.

- The file `DrawCodeFitting` simply plots the histograms produced by the above snippet of code. 

- The file `hough_example.cxx` builds a simple example of a 1D hough transformation with a straight line.


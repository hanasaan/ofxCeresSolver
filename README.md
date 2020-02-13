# ofxCeresSolver

## Description

A thin and easy-to-build [openFrameworks](http://openframeworks.cc) wrapper for [ceres-solver](http://ceres-solver.org/).

## Reference

- `VectorMath` and `RigidBodyTransformError` from [ofxCeres](https://github.com/elliotwoods/ofxCeres) by Elliot Woods 
- `addon_config.mk` style from [ofxEigen](https://github.com/bakercp/ofxEigen) by Christopher Baker.

## Tested On

- OF 0.11.0 and macos 10.14

## Getting Started

- Install ceres-solver via home brew by the command below.

`brew install ceres-solver`

- Optionally you can manually copy *dylibs from `libs/ceres/lib/osx` to each hard coded directory, which you can check via `otool -L *.dylib`


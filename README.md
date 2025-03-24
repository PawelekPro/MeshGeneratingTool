# MeshGeneratingTool

MeshGeneratingTool is an open source 3D finite element mesh generator with a
built-in simple CAD engine. Its design goal is to provide a user-friendly
meshing tool with parametric input and flexible visualization capabilities.

## Table of Contents

1. [About the Project](#about-the-project)
1. [Project Status](#project-status)
1. [Getting Started](#getting-started)
    1. [Dependencies](#dependencies)
    1. [Build Instructions](#build-instructions)
    1. [Installation](#installation)
    1. [Usage](#usage)
1. [How to Get Help](#how-to-get-help)
1. [License](#license)
1. [Authors](#authors)
1. [Acknowledgements](#acknowledgements)

# About the Project

![Workflow](docs/video.gif)

# Project Status

In progress ...

# Getting Started

## Dependencies

To compile and run this project, the following libraries are required:

1. **VTK (Visualization Toolkit)**
    - Version: 9.3.0
    - [VTK Website](https://vtk.org/)

2. **OpenCASCADE**
    - Version: 7.7.0
    - [OpenCASCADE Website](https://www.opencascade.com/)

3. **Qt**
    - Version: 6.6.1
    - [Qt Website](https://www.qt.io/)

4. **Gmsh**
    - Version > 4.12
    - [Gmsh Website](https://gmsh.info/)

4. **CMake**
    - Version > 3.20
    - [CMake Website](https://cmake.org/)

## Build Instructions

1. Clone the project source code from the GitHub repository.

   ```bash
   git clone https://github.com/PawelekPro/MeshGeneratingTool.git
   ```

   ```bash
   mkdir build && cd build
   ```

    ```bash
    cmake ..
    ```

    ```bash
    make
    ```

# How to Get Help

# License

Copyright (c) 2024 Paweł Gilewicz, Krystian Fudali

This project is licensed under the GPLv3 License - see [LICENSE.md](LICENSE.md)
file for details.

# Authors

* **[Paweł Gilewicz](https://github.com/PawelekPro)**
* **[Krystian Fudali](https://github.com/KFudali)**

See also the list
of [contributors](https://github.com/PawelekPro/MeshGeneratingTool/graphs/contributors)
who participated in this project.

# Acknowledgements

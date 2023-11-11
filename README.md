# Modeling and visualization of dynamic systems

![Hedley Attractor](https://github.com/chaoticsyst/dynsys/blob/master/media/Hadley.gif)

## Introduction

There are first-order differential equation systems on coordinates, which describe the position of points in space over time. Our application simulates them, thus allowing a convenient exploration of this area.

In particular, the application already contains 35 systems that simulate classic strange attractors.

Examples of equation systems: [Lorenz Attractor](https://en.wikipedia.org/wiki/Lorenz_system) and [Rössler Attractor](https://en.wikipedia.org/wiki/R%C3%B6ssler_attractor)

## UI

The interface is implemented using Qt5. It includes:
* Model selection
* Selection of constants characterizing the model
* Detailed settings of modeling and visualization
* Video recording capability
* Possibility to pause and fast-forward time

![UI screenshot](https://github.com/chaoticsyst/dynsys/blob/master/media/Interface.png)

## Simulation

Simulation is carried out using the fourth-order Runge-Kutta method, with a constant step. Written in the style of metaprogramming to achieve maximum performance.

To support models that are not integrated into the application, a mathematical equation parser has been implemented, which supports standard operations (+ - * /), brackets, and basic mathematical functions (sin, cos, exp, log, etc.).

No additional libraries are used for calculations.

## Visualization

Visualization is performed using OpenGL and the Qt wrapper over it.

The application takes several points around the initial position, calculates their trajectories, and then displays the points with some tail of their trajectories. Points calculated by the model are interpolated by the Catmull-Rom curve.

Free movement is implemented.

For convenience, it is possible to record video in avi format. This uses the libav library. You can also take screenshots.

## OS Support

The application is supported by the following operating systems:
* Linux
* Windows
* MacOS

## Installation Instructions
Required library versions:
* Qt5 — 5.10 and higher.
* libav (libavcodec, libavformat, libavutil, libswscale) — 57 and higher (but it is better to have at least 58).
* OpenGL — 3.3 and higher.

Next are instructions for specific OSs.

<details>
<summary>
Linux (U)buntu
</summary>

* Installing Qt:
```
sudo apt-get install qt5-default
```

* Installing libav:
```
sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev
```

* Updating OpenGL:
    * To check the version:
    ```
    glxinfo | grep "OpenGL version"
    ```
    * If the version is below 3.3, you need to update:
    ```
    sudo add-apt-repository ppa:oibaf/graphics-drivers
    sudo apt-get update
    sudo apt-get upgrade
    ```

</details>

<details>
<summary>
Windows
</summary>

* To get the necessary version of C++ (g++7 and above), download and install [MinGW](https://sourceforge.net/projects/mingw-w64/files/Multilib%20Toolchains%28Targetting%20Win32%20and%20Win64%29/ray_linn/gcc-9.x-with-ada/), and also add `/path/to/MinGW/bin/` to `PATH`.

* Install [Cmake](https://cmake.org/download/). It should be added to `PATH` automatically.

* Install Qt5, following the instructions [from here](https://doc.qt.io/archives/qt-4.8/install-win.html).

* Download [libav](http://builds.libav.org/windows/nightly-gpl/libav-x86_64-w64-mingw32-20180108.7z) and unpack it in a convenient location for you. Then add `/path/to/libav/bin` and `/path/to/libav/include` to `PATH`.

</details>

## How to use?

To visualize a model, one needs to:

* Select the model to be simulated:
    * You can choose one of the 35 presented models
    * You can enter your own differential equations
* Select the constants that characterize the model:
    * You can choose classical values
    * You can enter your own values
* Set the necessary settings:
    * Camera settings:
    * Modeling settings:
    * Visualization settings:

Controls:
* `WASDQE` — move forward / left / backward / right / up / down

* `F` — return to the original position
* `R` — take a screenshot

## Examples

![Dequan Li Attractor](https://github.com/chaoticsyst/dynsys/blob/master/media/Dequan%20Li.gif)

![Example 1](https://github.com/chaoticsyst/dynsys/blob/master/media/Example1.png)

![Example 2](https://github.com/chaoticsyst/dynsys/blob/master/media/Example2.png)

## Links

Channels:
* [YouTube](https://www.youtube.com/channel/UCL6pzFtbNd7fZKhABJCvV8g?view_as=subscriber)

* [Telegram](https://t.me/strangeattractors)

## About us

The application is being developed as part of a project work at the HSE University in St. Petersburg.

Authors: [Vladislav Nosivskoy](https://github.com/vladnosiv), [Roman Venediktov](https://github.com/e2e4b6b7), [Kirill Karnaukhov](https://github.com/kkarnauk)

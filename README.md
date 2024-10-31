SWE1D
===========

## General Information

This code has been tested under Ubuntu 22.04 or higher. Other Linux distributions will probably work.
However, we do not recommend using Windows or MacOS.
If you do not have Linux installed on your computer, please use WSL, Virtual Machine, Docker or similar.

## Docker

A prebuilt Docker image is available in [Dockerhub](https://hub.docker.com/r/tumi5/swe).

To use the prebuilt image, we do recommended that you first clone SWE1D, then navigate into the SWE1D checkout
and run the Docker container interactively by mapping the SWE1D directory into the container (here, we use `work` as our mapping point):

```shell
docker run -it -v ${PWD}:/work --rm --privileged tumi5/swe /bin/bash
```

Navigate into the `work` directory and continue with the steps below.

## Build and Test

As build system configurator we use CMake. To compile the code execute the following commands in this directory:

* Create a build directory: `mkdir build`. You can also choose any other name for your build directory.
* Switch to this directory: `cd build`
* Run CMake: `cmake ..` (for an overview of all available options, use `ccmake ..`)
* For a `Debug` build, run `cmake .. -DCMAKE_BUILD_TYPE=Debug`
* Run Make: `make` (or `make -j` to compile with multiple cores).
* Run Tests: Some basic unit tests have been implemented (`make test`). Feel free to add your own test cases inside the `Tests` folder.

## Running a Simulation

* Run the code: `./SWE1D-Runner`
* With `./SWE1D-Runner --help`, you can see additional command-line arguments you can pass.

## Visualize the Results

We use Paraview to visualize the results of the simulation. Make sure to update to a recent Paraview version (to avoid compatibility issues).
In the build directory where you have executed the simulation you will find a few output files that have the extension .vtr and one output
file that has the extension .vtp. The latter is the index file that combines all other files with an additional description (in our case the timestep).

We are now interested in that index file. Open Paraview and read the .vtp file. If Paraview cannot automatically deduce the correct reader, choose
the `PVD Reader` from the list.

Now start with a `Plot Over Line` filter and let the animation run.

## Adding New Source Files

You can add new source files by just creating them somewhere within the `Source` folder. CMake automatically detects these files and adds them to the build.

## Creating Doxygen Documentation

* Navigate into the SWE1D directory
* Run the following Doxygen command: `doxygen Documentation/Doxyfile`
* You should now see a directory called `html`
* Open `index.html` to see the generated documentation.

## Development Hints & FAQ

### It does not compile and everything seems fine?

Make sure to use `make clean` before you use `make`. Sometimes there are build artifacts from previous build processes that spoil your current compilation process. `make clean` takes care of deleting everything that should not be there and allows the compiler to start from scratch.

Sometimes it is also helpful to delete the `build` folder and create a new one, following the steps from the compilation section above.

### How can I see all the compiler flags the generated Makefile is using?

Instead of using `make`, run `VERBOSE=1 make`. You can also run `make -n` to invoke a dry run where you see what the Makefile would do in case of compilation.

### How can I see the test output?

Instead of using `make test`, run `ctest --verbose`.

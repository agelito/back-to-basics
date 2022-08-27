# Configuring Build
To avoid having to manually invoke the `clang` compiler manually each time we want to build the project, as well as allowing dependencies to be specified regardless of which computer or operating system is used we'll set up `cmake` to generate a project and then use `ninja` to build.

`cmake` use a file called `CMakeLists.txt` to determine which dependencies, input files, and outputs should be generated. We'll create a minimal `CMakeLists.txt` file to build our `hello_world` program using that instead.

```sh
touch CMakeLists.txt
```

Open `CMakeLists.txt` and type the following:
```cmake
cmake_minimum_required(VERSION 3.24)

project(back_to_basics)

# The path to clang might vary depending on system
set(CMAKE_C_COMPILER /usr/bin/clang)

# Use the c11 standard
set(CMAKE_C_STANDARD 11)

# Define the hello_world executable target
add_executable(hello_world src/hello_world.c)
```

Now use `cmake` to generate the `Ninja` project configuration and then build the project:
```sh
mkdir build && cd build
cmake -GNinja ../.
ninja
```

After runnig the previous commands a new `hello_world` executable has been generated try running it to ensure we still see the `Hello, World!` output being printed:
```sh
./hello_world
```

Since we don't want to commit the `build` directory into git we'll add it to the ignore list. Open `.gitignore` file and replace all contents with the following:
```gitignore
build/
```

To clean up from the manual build made earlier we can remove the old `bin/` directory as well.
```sh
cd .. && rm -rf bin
```


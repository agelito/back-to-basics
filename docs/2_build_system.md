# Build System
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

Commit the changes using git:
```sh
git add . && git commit -m "feat: minimum cmake configuration to build hello_world"
```

## Visual Studio Code
>It's recommended to install the C/C++ Extension Pack for Visual Studio Code before proceeding with setting up the building and debugging tasks. https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack.


We can configure tasks to build and debug the application in Visual Studio Code. This will allow us to press F7 to build.

### Building

Start by creating the `.vscode` directory and a `tasks.json` file.

```sh
mkdir .vscode && touch .vscode/tasks.json
```

We'll populate the `.vscode/tasks.json` file with two tasks, one task for generating the `ninja` build configuration using `cmake` and one task to invoke `ninja` to build the project.

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "CMake Generate Debug",
            "type": "shell",
            "command": "cmake",
            "args": [
                "-GNinja",
                "-DCMAKE_BUILD_TYPE=Debug",
                "../."
            ],
            "group": "build",
            "presentation": {
                "reveal": "always",
                "panel": "shared",
                "showReuseMessage": false
            },
            "options": {
                "cwd": "${workspaceFolder}/build"
            }
        },
        {
            "label": "Ninja Build",
            "type": "shell",
            "command": "ninja",
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "presentation": {
                "reveal": "always",
                "panel": "shared",
                "showReuseMessage": false
            },
            "options": {
                "cwd": "${workspaceFolder}/build",
            },
            "dependsOn": [
                "CMake Generate Debug"
            ],
        }
    ]
}
```

Pressing `ctrl+shift+b` will trigger the build. Since the `Ninja Build` task is depending on `CMake Generate Debug` it will run automatically before `Ninja Build` runs.

### Debugging
To debug or launch the application we'll have to create the `.vscode/launch.json` configuration file. Here we can specify how VSCode can run the application when user press `F5` or `Ctrl+F5`.

Create the `.vscode/launch.json` file:
```sh
touch .vscode/launch.json
```

Define the launch configuration within `vscode/launch.json` file:
```json
{
    "version": "0.2.0",
    "configurations": [

        {
            "type": "cppdbg",
            "request": "launch",
            "name": "Launch Application",
            "program": "${workspaceRoot}/build/hello_world",
            "cwd": "${workspaceRoot}",
            "preLaunchTask": "${defaultBuildTask}",
        }
    ]
}
```

Now when pressing `F5` the application can be debugged, and will stop on any breakpoints in the code.

Don't forget to test this and if all works well commit the changes using git.

```sh
git add . && git commit -m "chore: add tasks for building and launching the application"
```

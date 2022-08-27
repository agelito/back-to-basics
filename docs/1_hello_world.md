# Hello World

This part of the guide describes setting up the development environment and making sure we can build C code. None of the tools or structure
used in this part if enforced, but using a similar setup will make it easier to follow the rest of the guide.

## Installing required tools
This is a list of tools we'll need initially. More tools might be added later on as we're implementing more features.

### Text Editor
For editing the code I'll be using `Visual Studio Code`. This tool is available in most linux distrubution's package repositories. In Arch Linux (pacman) it's called `code` and can be installed using the following command:

```sh
# Arch Linux (pacman)
pacman -S code
```

### Compiler
To compile the code we'll be using `clang`.

> clang  is  a C, C++, and Objective-C compiler which encompasses preprocessing, parsing, optimization, code generation, assembly, and linking.


`clang` can be installed from most distributions package repositories:
```sh
# Arch Linux (pacman)
pacman -S clang
```

### Build System
`cmake` and `ninja` will be used to build the code. We'll just install them for now and cover configuration at a later point.

```sh
# Arch Linux (pacman)
pacman -S cmake ninja
```

## Creating basic project directories
Aiming to keep the directory structure as simple as possible for now, we'll just create a root directory for the project and one directory for source code. Later on there's also going to be one directory for the `cmake` build.

```sh
# Create directories and hello_world source file.
mkdir back-to-basics && cd back-to-basics
mkdir src && touch src/hello_world.c
mkdir bin

# Initialize git repository so the source code changes can be tracked.
git init && touch .gitignore

# Open the project with VSCode
code .
```

## Creating hello_world
Open the `src/hello_world.c` file and input the following content. All this file is responsible for is printing `Hello, World!` to the terminal and then exit. It's purpose is just to ensure we can compile and run C code.

```C
#include <stdio.h>

int main(int argc, char* argv[]) 
{
    printf("Hello, World!\n");

    return 0;
}
```

## Building hello_world
For now `src/hello_world.c` will just be built with default options. Later on we'll be using `cmake` to configure various build options and settings and not invoke the `clang` compiler manually. 
```sh
# Compile and link the hello_world program.
clang -o bin/hello_world --std=c11 src/main.c

# Run hello_world
./bin/hello_world
```

After running these commands `Hello, World!` should be printed to the terminal. Congratulations we've just built our first C program.

## Commit changes
To track our code changes over time we'll commit the hello_world program using `git`. We don't want to include files built from the source code so we'll also put some folders to be ignored inside the `.gitignore` file.

Open the `.gitignore` file and insert the following:
```.gitignore
bin/
```

Now commit the changes the following files will be added to our commit:

> * .gitignore 
> * src/hello_world.c

To commit use the following commands:
```sh
git add .
git commit -m "feat: create hello_world sample program"
```
# Welcome to Back to Basics

## Introduction
This is an example project and tutorial for making and rendering a game without using a graphics card**. We'll be calculating each individual pixel that appears on the screen using the CPU only.

Some knowledge of C is expected and the operating system used by the author is Arch Linux. The project should run on most systems though and if it doesn't, only minor modifications should be necessary to make it run.

The end goal of this isn't the game itself, so the tutorial will not focus much on creating a great game, but rather the fundamental building blocks. Also it should be considered mostly academic since there's better ways of rendering games using modern graphic card API's (DirectX/Vulkan/OpenGL/etc).

The tutorial is split into multiple chapters. Each chapter is of arbitrary size and I'm writing them as I go along implementing each future myself.

>** SDL2 will be used for creating a window which in turn might use the graphics card to display the pixels we've defined to screen. This choice was made because the focus of this tutorial shouldn't be interacting with the OS API's for opening and managing windows.

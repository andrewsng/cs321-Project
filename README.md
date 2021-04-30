# VGA Graphics Programming in DOS


## Description

This project was meant to be an exploration of the way computer graphics were done through the VGA display device in classic DOS video games. My toolchain ended up being the
DOSBox emulator, as well as the fairly modern DJGPP development tools, which is a DOS port of GCC. I first started playing around with the VGA BIOS Mode 13h, which allows for easy 
access to a 320x200 256-Color framebuffer at memory segment 0xA000. Here, I implemented an old-school scanline algorithm for filling in 2D polygons.

![palette](https://user-images.githubusercontent.com/54720199/116652637-13c3f380-a932-11eb-9605-29b981e3cb76.png)
![polyfill](https://user-images.githubusercontent.com/54720199/116652631-11619980-a932-11eb-8f09-410011ce40bc.png)


After that, I added capabilities for rendering 3D objects, as well as animating their transformations. However, the rendering speeds of the emulator resulted in horrendous 
flickering artifacts. To fix this, I had to use the undocumented VGA Mode X. Entering Mode X involved directly setting many of the VGA's internal registers, which was done through 
read/write calls to hardware I/O ports. The benefits were a higher resolution, and full access to the VGA's 256KB video memory, allowing for page flipping with vsync using 2 
framebuffers.

The great disadvantage this brought was that it requires the VGA memory layout to work as 4 64KB bitplanes, instead of the linear layout that Mode 13h offers. However, if your 
program takes advantage of it, this can result in great speedups by using the VGA hardware to write pixels to all 4 bitplanes at once in parallel, which is definitely useful for 
filling polygons on the screen with solid colors.

![triangle](https://user-images.githubusercontent.com/54720199/116652652-19213e00-a932-11eb-89a8-3163bb42053a.gif)
![cube](https://user-images.githubusercontent.com/54720199/116652679-25a59680-a932-11eb-9610-d6a8fd472ea9.gif)

## References

My main reference for the graphics algorithms and the VGA hardware in general was the Graphics Programming Black Book by Michael Abrash, which is freely available online:
https://www.jagregory.com/abrash-black-book/

# SDL_3D
A simple 3D graphics library to run on top of SDL 2.x primarily without 3D acceleration hardware

Many years ago (around 1994) I wrote a simple filled-polygon 3D engine for the Commodore Amiga, this was for a simple flight sim and was a fun project to learn about 3D graphics.

Recently I wanted to add a graphics output to a microcontroller project I was working on, and I wrote a very simple 2D library, modelled upon SDL2, no hardware acceleration, just a simple framebuffer. This worked well, but I noticed that the M3 microcontroller I had been using also had an onboard FPU! So I wondered if I could do 3D graphics on it!

I pulled my Amiga 1200 out and hunted around for my 20 year old 3D source code. The Amiga 1200 didn't have an FPU so all the calculations were done using 16bit Fixed point maths, and was written in SAS/C. Fun stuff. Anyway, I rewrote the 3D library to use C++ and use the SDL2 style graphics functions for output. Currently only outputting wireframe, but it is a good tech demo.

So I have upladed here, a cleaned up version of the 3D library. I have gathered all the classes into a single .hpp/.cpp pair. so you can just drop the files into your project and go, no messing around with linkers and system includes.

At this stage, we are basically at v0.01a... the API is subject to change dramatically until we reach v1.0. 

Until v1.0, the licence is basically free to use in non commercial projects, and you need to credit the use of this code.

My hope is that others will want to help improve the project, as it might make quite a nice little 3D Scene framework for other hobbists.

The main.cpp, give an example of how to set up a 3D object, manipulate it and then draw it on the screen.



The important parts of the API are listed below.

Key issues:
No hardware acceleration... Perhaps I'll add an openGL mode in the future.
Fixed camera... The Camera is fixed at 0,0,0 and looks back down the Z axis.
Wireframe only, with back culling based on clockwise triangles.

TDMesh = The verticies and triangles, this has a method to load in both binary and ASCII Stanford PLY models as long as the vertex only has x,y,z and the faces are triangles (quads comming soon ;) ).

TDObject = This is an instance of a mesh, which can be manipulated in the 3D space, a mesh must be supplied.

  locate(float,float,float); positions the object in space.
  orient(float,float,float); sets the object's orientation is space (angles in radians).

  move(float,float,float); move the object relative to its current position.
  roatate(float,float,float); roatate the object relative to its current orientation.
  
Rasterizer = This class draws objects to the screen.
  clearDisplay(); clears the screen ready to draw the objects
  drawObject(TDObject*); draws an object to the screen
  updateDisplay(); show the screen, with all your freshly drawn objects.

TDScene = This is currently undeveloped, but will eventually be the primary way to build and maipulate your 3D objects. 

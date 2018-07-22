//
//  main.cpp
//  SDL_3D
//
//  Created by Matt Parsons on 22/07/2018.
//  Copyright © 2018 Matt Parsons. All rights reserved.
//

#include <stdio.h>

// SDL2 Headers
#include <SDL2/SDL.h>

// SDL_3D Header
#include "SDL_3D.hpp"


int main(int argc, const char * argv[]) {
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        printf("Failed to init SDL\n");
        return -1;
    }
    
    // Create our window
    SDL_Window* window = SDL_CreateWindow("SDL 3D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          320, 200, SDL_WINDOW_RESIZABLE);
    
    Rasterizer raster;
    
    int width=0;
    int height=0;
    SDL_GetWindowSize(window, &width, &height);
    raster.setDisplay(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC),width, height);
    raster.focalLength = 0.2;
    raster.backfaceCull= 0;
    

    raster.camera.locate(0, 0, 0);
    
    
    //Object test
    TDObject craft;
    craft.loadPly("/Users/matt/Library/Mobile\ Documents/com\~apple\~CloudDocs/vulc1.ply");
    craft.vz=-0.1;
    craft.locate(-1, 0, -4);  //The object can't be rasterized until a locate command is issued.
    
    TDObject car;
    car.vz=-0.1;
    car.locate(1, 0, -1);     //The object can't be rasterized until a locate command is issued.
    
    //craft.loadPly("/Users/matt/Library/Mobile\ Documents/com\~apple\~CloudDocs/NormalCubeASCII.ply");
    //craft.loadPly("/Users/matt/Library/Mobile\ Documents/com\~apple\~CloudDocs/sphere1.ply");
    car.loadPly("/Users/matt/Library/Mobile\ Documents/com\~apple\~CloudDocs/delor1.ply");
    
    
    
    float x=-1;
    float y=-0.5;
    float z=0;
    
    TDQuad plane;
    //Quads test
    for(int i=0;i<100*100;++i){
        
        // plane[i].a.x=
        
        
        
        
    }
    
    
    int loop=true;
    
    while (loop){
        
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){loop = false;}
            
            if (event.type == SDL_WINDOWEVENT) {
                switch (event.window.event) {
                        
                    case SDL_WINDOWEVENT_RESIZED:
                        SDL_GetWindowSize(window, &width, &height);
                        raster.adjustDisplay(width, height);
                        break;
                        
                    default:
                        break;
                }
            }
            
            
            if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        loop = false;
                        break;
                    case SDLK_r:
                        // Cover with red
                        raster.clearCol.r=1;
                        raster.clearCol.g=0;
                        raster.clearCol.b=0;
                        break;
                        
                    case SDLK_g:
                        // Cover with green
                        raster.clearCol.r=0;
                        raster.clearCol.g=1;
                        raster.clearCol.b=0;
                        break;
                        
                    case SDLK_b:
                        // Cover with blue
                        raster.clearCol.r=0;
                        raster.clearCol.g=0;
                        raster.clearCol.b=1;
                        break;
                        
                    case SDLK_SPACE:
                        // Cover with black
                        raster.clearCol.r=0;
                        raster.clearCol.g=0;
                        raster.clearCol.b=0;
                        break;
                        
                    default:
                        raster.clearCol.r=(float)rand()/RAND_MAX;
                        raster.clearCol.g=(float)rand()/RAND_MAX;
                        raster.clearCol.b=(float)rand()/RAND_MAX;
                        break;
                }
            }
        }
        
        
        
        raster.clearDisplay();
        
        //Draw gfx

        
        if(craft.z()<-64 || craft.z()>0){
            craft.vz=-craft.vz;
        }
        craft.move(0, 0, craft.vz);
        craft.rotate(0.01,0.01,0.01);
        raster.drawObject(&craft);
        
        
        if(car.z()<-32 || car.z()>0){
            car.vz=-car.vz;
        }
        car.move(0, 0, car.vz);
        car.rotate(0.01,0.01,0.01);
        raster.drawObject(&car);
        
        // Swap our back buffer to the front
        raster.updateDisplay();
        
    }
    
    return 0;
}


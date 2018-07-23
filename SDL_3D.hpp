//
//  SDL_3D.hpp
//  SDL_3D
//
//  Created by Matt Parsons on 22/07/2018.
//  Copyright © 2018 Matt Parsons. All rights reserved.
//

#ifndef SDL_3D_hpp
#define SDL_3D_hpp

#include <stdio.h>

// SDL2 Headers
#include <SDL2/SDL.h>


struct Col{
    float r;
    float g;
    float b;
    float a;
};

struct TDPoint{
    float x;
    float y;
    float z;
};

struct TDTriangle{
    int a;
    int b;
    int c;
    Col colour;
};

struct TDQuad{
    int a;
    int b;
    int c;
    int d;
    Col colour;
};


class TDMesh{
public:
    
    ~TDMesh();
    
    TDPoint* localVertex; //Vertex array, vertices are in local coordinates
    int vertices;         //Vertex count
    
    TDTriangle* triangle; //Triangle array
    int triangles;        //Triangle count
    
    //modelling
    void init(int,int);   //initilise object with memory for vertices and triangles
    int addVertex(float,float,float);   // takes x,y,z and returns the vertex index
    int addTriangle(int,int,int);       // takes vertex indices and retruns the triangle index
    void setTriangleColour(int,float,float,float,float);    //sets the r,g,b,a value for triangle at index
    
    void loadPly(char*);    //file name of a stanford Ply model (little endian binary and ASCII only).
    
};


class TDCamera{
public:
    TDPoint position;     //Camera position in world coordinates
    TDPoint angle;        //Camera rotion in world coordinates
    

    void locate(float,float,float); //Absolute
    void   move(float,float,float); //Relative
    
    void orient(float,float,float); //Absolute
    void rotate(float,float,float); //Relative
};



class TDObject{
public:
    
    TDObject(TDMesh*);
    ~TDObject();
    
    float x();
    float y();
    float z();
    
    //User variables
    float vx;
    float vy;
    float vz;
    
    TDMesh* mesh;
    TDPoint* vertex;      //Vertex array, vertices in world coordinates
    
    //These operate on the object vertices
    void locate(float,float,float); //Absolute
    void   move(float,float,float); //Relative
    
    void orient(float,float,float); //Absolute
    void rotate(float,float,float); //Relative
    
    void  size(float,float,float);  //Absolute *
    void scale(float,float,float);  //Relative *
    
    
private:
    TDPoint position;     //object position in world coordinates
    TDPoint angle;        //object rotion in world coordinates
    TDPoint proportion;   //object scale factor
};




class Rasterizer{
public:

    float aspectRatio;
    float focalLength;
    TDCamera camera;
    
    int backfaceCull=0;
    Col clearCol;
    void drawTQuad(TDPoint*,TDQuad*);
    void drawTriangle(TDPoint*, TDTriangle*);
    void drawPoint(TDPoint*);
    void drawLine(TDPoint*,TDPoint*);
    
    void drawObject(TDObject*);
    
    void setDrawColour(int,int,int,int);
    
    void setDisplay(SDL_Renderer*,int,int);
    void adjustDisplay(int,int);
    void clearDisplay();
    void updateDisplay();
    
    SDL_Point projection(TDPoint*);
    
private:
    SDL_Renderer* renderer;
    float hWidth;
    float hHeight;
    
};


//Scene

class TDScene{
    
};

#endif /* SDL_3D_hpp */

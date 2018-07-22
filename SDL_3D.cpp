//
//  SDL_3D.cpp
//  SDL_3D
//
//  Created by Matt Parsons on 22/07/2018.
//  Copyright © 2018 Matt Parsons. All rights reserved.
//

#include "SDL_3D.hpp"

#include <string.h>

#define TYPE_UNKNOWN  0
#define TYPE_ASCII    1
#define TYPE_BINARYLE 2
#define TYPE_BINARYBE 3

void TDCamera::locate(float x,float y,float z){
    
    position.x=x;
    position.y=y;
    position.z=z;
    
}

void TDCamera::move(float x,float y,float z){
    
    position.x+=x;
    position.y+=y;
    position.z+=z;
    
}

void TDCamera::rotate(float xA, float yA, float zA){
    
    angle.x +=xA;
    angle.y +=yA;
    angle.z +=zA;
    
}

void TDCamera::orient(float xA,float yA,float zA){
    
    angle.x = xA;
    angle.y = yA;
    angle.z = zA;
    
}

//Object************************************


void TDObject::init(int verts, int tri){
    
    localVertex =(TDPoint*)calloc(verts, sizeof(TDPoint));
    vertex =(TDPoint*)calloc(verts, sizeof(TDPoint));
    vertices = verts;
    
    triangles = tri;
    triangle =(TDTriangle*)calloc(tri, sizeof(TDTriangle));
    
}

float TDObject::x(){return position.x;}
float TDObject::y(){return position.y;}
float TDObject::z(){return position.z;};

void TDObject::loadPly(char* filename){
    
    
    SDL_RWops *file = SDL_RWFromFile(filename, "rb");
    
    Sint64 res_size = SDL_RWsize(file);
    printf("Size: %d\n",res_size);
    
    Uint8 buf[res_size];
    
    if (file != NULL) {
        SDL_RWread(file, buf, res_size, 1);
        SDL_RWclose(file);
    }else{
        printf("Load failed.\n");
        return;
    }
    
    //Check for the PLY magic number
    uint32_t* magic =(uint32_t*) buf;
    if( *magic != 175729776){
        printf("Not a PLY File.\n");
        return;
    }
    
    //Determine Data type
    int type=TYPE_UNKNOWN;
    char* test = NULL;
    
    test =strstr((const char*)buf, "format ascii");
    if(test !=NULL){
        type = TYPE_ASCII;
    }
    
    test =strstr((const char*)buf, "format binary_little_endian");
    if(test !=NULL){
        type = TYPE_BINARYLE;
    }
    
    test =strstr((const char*)buf, "format binary_big_endian");
    if(test !=NULL){
        type = TYPE_BINARYBE;
    }
    
    //Determine data start in the buffer
    char* data =strstr((const char*)buf, "end_header")+11;
    
    
    //Determine the vertex structure (only the number at the moment, vertex must be x y z)
    test =strstr((const char*)buf, "element vertex ");
    if(test !=NULL){
        test +=15;
    }
    vertices = atoi(test);
    
    //Determine the face structure (only triangles supported at the moment)
    test =strstr((const char*)buf, "element face ");
    if(test !=NULL){
        test +=13;
    }
    triangles = atoi(test);
    
    
    
    
    //Only supports Binary LE, no big endian support yet.
    if(type==TYPE_BINARYLE){
        
        //allocate memory for data
        init(vertices, triangles);
        
        
        //Load in vertices
        int pointer=0;
        for(int i=0;i<vertices;++i){
            
            float x = (*(float*)&data[pointer]);
            float y = (*(float*)&data[(pointer+4)]);
            float z = (*(float*)&data[(pointer+8)]);
            
            localVertex[i].x = x;
            localVertex[i].y = y;
            localVertex[i].z = z;
            
            pointer +=4*3;  //size of a float * 3
            //printf("Vertex %d:, X:%f, Y:%f, Z:%f\n",i,x,y,z);
        }
        
        //Load in triangles
        data +=pointer;
        pointer=0;
        for(int i=0;i<triangles;++i){
            
            if(data[pointer]==3){
                
                int* a=(int*)&data[pointer+1];
                int* b=(int*)&data[pointer+5];
                int* c=(int*)&data[pointer+9];
                
                triangle[i].a = *a;
                triangle[i].b = *b;
                triangle[i].c = *c;
                
                triangle[i].colour.r = 1;
                triangle[i].colour.g = 1;
                triangle[i].colour.b = 1;
                triangle[i].colour.a = 1;
                
                pointer += 1 + 4*3;//1 byte for the list type + the size of an int * 3
                //printf("Triangle %d:, A:%d, B:%d, C:%d\n",i,*a,*b,*c);
            }
            
            
            if(data[pointer]==4){
                
                //need to support quads
                /*
                 int a=(int)data[pointer+1];
                 int b=(int)data[pointer+5];
                 int c=(int)data[pointer+9];
                 
                 triangle[i].a = &vertex[a];
                 triangle[i].b = &vertex[b];
                 triangle[i].c = &vertex[c];
                 
                 triangle[i].colour.r = 1;
                 triangle[i].colour.g = 1;
                 triangle[i].colour.b = 1;
                 triangle[i].colour.a = 1;
                 */
                pointer += 1 + 4*4;//1 byte for the list type + the size of an int * 4
            }
            
        }
        
        
    }
    
    //ASCII file type.
    if(type==TYPE_ASCII){
        
        //allocate memory for data
        init(vertices, triangles);
        
        
        //Load in vertices
        for(int i=0;i<vertices;++i){
            
            
            
            float x = atof(data);
            data =(char*)memchr(data,0x20,20)+1;
            
            float y = atof(data);
            data =(char*)memchr(data,0x20,20)+1;
            
            float z = atof(data);
            data =(char*)memchr(data,0x0A,20)+1;
            
            localVertex[i].x = x;
            localVertex[i].y = y;
            localVertex[i].z = z;
            
            
        }
        
        for(int i=0;i<triangles;++i){
            
            if(atoi(data)==3){
                data =(char*)memchr(data,0x20,20)+1;
                
                
                int a=atoi(data);
                data =(char*)memchr(data,0x20,20)+1;
                
                int b=atoi(data);
                data =(char*)memchr(data,0x20,20)+1;
                
                int c=atoi(data);
                data =(char*)memchr(data,0x0A,20)+1;
                
                triangle[i].a = a;
                triangle[i].b = b;
                triangle[i].c = c;
                
                triangle[i].colour.r = 1;
                triangle[i].colour.g = 1;
                triangle[i].colour.b = 1;
                triangle[i].colour.a = 1;
                
            }
            
            
            if(atoi(data)==4){
                
                //need to support quads
                /*
                 int a=(int)data[pointer+1];
                 int b=(int)data[pointer+5];
                 int c=(int)data[pointer+9];
                 
                 triangle[i].a = &vertex[a];
                 triangle[i].b = &vertex[b];
                 triangle[i].c = &vertex[c];
                 
                 triangle[i].colour.r = 1;
                 triangle[i].colour.g = 1;
                 triangle[i].colour.b = 1;
                 triangle[i].colour.a = 1;
                 */
                
            }
            
        }
        
        
        
        
        
        printf("ASCII\n");
        
    }
    
    
    
    
    
    printf("Object Loaded\n");
}

int TDObject::addVertex(float x, float y, float z){
    
    ++vertices;
    
    localVertex = (TDPoint*) realloc(localVertex, sizeof(TDPoint)*vertices);
    vertex      = (TDPoint*) realloc(vertex, sizeof(TDPoint)*vertices);
    
    int thisVertex = vertices-1;
    
    localVertex[thisVertex].x = x;
    localVertex[thisVertex].y = y;
    localVertex[thisVertex].z = z;
    
    return thisVertex;
}

int TDObject::addTriangle(int a, int b, int c){
    
    ++triangles;
    
    triangle = (TDTriangle*) realloc(triangle, sizeof(TDTriangle)*triangles);
    
    int thisTriangle = triangles - 1;
    
    triangle[thisTriangle].a = a;
    triangle[thisTriangle].b = b;
    triangle[thisTriangle].c = c;
    
    return thisTriangle;
}


void TDObject::setTriangleColour(int tri, float r, float g, float b, float a){
    
    triangle[tri].colour.r = r;
    triangle[tri].colour.g = g;
    triangle[tri].colour.b = b;
    triangle[tri].colour.a = a;
}

TDObject::~TDObject(){
    
    //Free up the allocated memory.
    if(localVertex !=nullptr){
        free(localVertex);
    }
    
    if(vertex !=nullptr){
        free(vertex);
    }
    
    if(triangle !=nullptr){
        free(triangle);
    }
    
}

void TDObject::locate(float x,float y,float z){
    
    position.x=x;
    position.y=y;
    position.z=z;
    
    //translate all local vertices to world coordinates
    for(int i=0;i<vertices;++i){
        vertex[i].x = localVertex[i].x + x;
        vertex[i].y = localVertex[i].y + y;
        vertex[i].z = localVertex[i].z + z;
    }
    
}

void TDObject::move(float x,float y,float z){
    
    position.x+=x;
    position.y+=y;
    position.z+=z;
    
    //translate all vertices relative world coordinates
    for(int i=0;i<vertices;++i){
        vertex[i].x = vertex[i].x + x;
        vertex[i].y = vertex[i].y + y;
        vertex[i].z = vertex[i].z + z;
    }
    
}

void TDObject::rotate(float xA, float yA, float zA){
    
    angle.x +=xA;
    angle.y +=yA;
    angle.z +=zA;
    
    float cosa = cos(angle.z);
    float sina = sin(angle.z);
    
    float cosb = cos(angle.x);
    float sinb = sin(angle.x);
    
    float cosc = cos(angle.y);
    float sinc = sin(angle.y);
    
    float Axx = cosa*cosb;
    float Axy = cosa*sinb*sinc - sina*cosc;
    float Axz = cosa*sinb*cosc + sina*sinc;
    
    float Ayx = sina*cosb;
    float Ayy = sina*sinb*sinc + cosa*cosc;
    float Ayz = sina*sinb*cosc - cosa*sinc;
    
    float Azx = -sinb;
    float Azy = cosb*sinc;
    float Azz = cosb*cosc;
    
    for (int  i = 0; i < vertices; ++i) {
        float px = localVertex[i].x;
        float py = localVertex[i].y;
        float pz = localVertex[i].z;
        
        vertex[i].x = (Axx*px + Axy*py + Axz*pz)+position.x;
        vertex[i].y = (Ayx*px + Ayy*py + Ayz*pz)+position.y;
        vertex[i].z = (Azx*px + Azy*py + Azz*pz)+position.z;
    }
    
    return;
    
}

void TDObject::orient(float xA,float yA,float zA){
    
    angle.x = xA;
    angle.y = yA;
    angle.z = zA;
    
    rotate(0,0,0);
    
}

//Rasterizer************************************

SDL_Point Rasterizer::projection(TDPoint* point){
    
    SDL_Point retval;
    
    float z = 1 - (point->z*focalLength);
    
    
    retval.x = (int)((point->x/z)*hWidth);
    retval.y = -(int)((point->y/z)*hHeight);
    
    retval.x = (retval.x + (hWidth*aspectRatio) )/aspectRatio;
    retval.y+=hHeight;
    
    return retval;
}

void Rasterizer::drawTQuad(TDPoint* vertex, TDQuad* quad){
    SDL_SetRenderDrawColor(renderer, quad->colour.r*255, quad->colour.g*255, quad->colour.b*255, quad->colour.a*255);
    
    SDL_Point tri[5];
    
    tri[0]=projection(&vertex[quad->a]);
    tri[1]=projection(&vertex[quad->b]);
    tri[2]=projection(&vertex[quad->c]);
    tri[3]=projection(&vertex[quad->d]);
    
    //Close the loop
    tri[4].x = tri[0].x;
    tri[4].y = tri[0].y;
    
    SDL_RenderDrawLines(renderer, tri, 4);
}

void Rasterizer::drawTriangle(TDPoint* vertex, TDTriangle* triangle){
    
    SDL_SetRenderDrawColor(renderer, triangle->colour.r*255, triangle->colour.g*255, triangle->colour.b*255, triangle->colour.a*255);
    
    SDL_Point tri[4];
    
    tri[0]=projection(&vertex[triangle->a]);
    tri[1]=projection(&vertex[triangle->b]);
    tri[2]=projection(&vertex[triangle->c]);
    tri[3].x = tri[0].x;
    tri[3].y = tri[0].y;
    
    if(backfaceCull==1){
        int test = (tri[0].x*tri[1].y) - (tri[1].x*tri[0].y) + (tri[1].x*tri[2].y) - (tri[2].x*tri[1].y) + (tri[2].x*tri[3].y) - (tri[3].x*tri[2].y) + (tri[3].x*tri[0].y) -(tri[0].x*tri[3].y);
        if(-test<0){
            return;
        }
    }
    
    SDL_RenderDrawLines(renderer, tri, 4);
    //SDL_RenderPresent(renderer);
    //printf("DrawLine\n");
    
    /*
     SDL_Point tri[4];
     
     tri[0].x = (int)((camera.z * (triangle->a.x-camera.x)) / (camera.z + triangle->a.z) + camera.x);
     tri[0].y = (int)((camera.z * (triangle->a.y-camera.y)) / (camera.z + triangle->a.z) + camera.y);
     
     tri[1].x = (int)((camera.z * (triangle->b.x-camera.x)) / (camera.z + triangle->b.z) + camera.x);
     tri[1].y = (int)((camera.z * (triangle->b.y-camera.y)) / (camera.z + triangle->b.z) + camera.y);
     
     tri[2].x = (int)((camera.z * (triangle->c.x-camera.x)) / (camera.z + triangle->c.z) + camera.x);
     tri[2].y = (int)((camera.z * (triangle->c.y-camera.y)) / (camera.z + triangle->c.z) + camera.y);
     
     tri[3].x = tri[0].x;
     tri[3].y = tri[0].y;
     
     
     
     SDL_RenderDrawLines(renderer, tri, 4);
     */
}



void Rasterizer::drawObject(TDObject* object){
    
    for(int i=0;i<object->triangles;++i){
        drawTriangle(object->vertex, &object->triangle[i]);
    }
    
}

void Rasterizer::drawPoint(TDPoint* vertex){
    SDL_Point point = projection(vertex);
    SDL_RenderDrawPoint(renderer, point.x, point.y);
}

void Rasterizer::drawLine(TDPoint* a, TDPoint* b){
    
    SDL_Point pa = projection(a);
    SDL_Point pb = projection(b);
    
    SDL_RenderDrawLine(renderer, pa.x, pa.y, pb.x, pb.y);
}

void Rasterizer::setDrawColour(int r,int g, int b, int a){
    
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    
}

void Rasterizer::setDisplay(SDL_Renderer* rend,int width, int height){
    
    renderer = rend;
    adjustDisplay(width, height);
}

void Rasterizer::adjustDisplay(int width, int height){
    
    hWidth = width/2;
    hHeight = height/2;
    aspectRatio = (float)width/(float)height;
    
}

void Rasterizer::clearDisplay(){
    // clear buffer
    SDL_SetRenderDrawColor(renderer, clearCol.r*255, clearCol.g*255, clearCol.b*255, 255);
    SDL_RenderClear(renderer);
}

void Rasterizer::updateDisplay(){
    SDL_RenderPresent(renderer);
}

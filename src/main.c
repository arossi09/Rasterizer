#include "tga.h"
#include "obj_loader.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 100
#define HEIGHT 100

struct TGAColor white = {255, 255, 255};
struct TGAColor red = {255, 0, 0};
struct TGAColor blue = {0, 0, 255};


//Wrote my own TGA library that can read and write tga formatted files
//Used this along with another class I wrote to read .obj files into
//a data structure to be able to draw obj files to the tga file.


void line(int x0, int y0, int x1, int y1, struct TGA_image image, 
        struct TGAColor color){
    int steep = 0;
    if(abs(x0-x1)<abs(y0-y1)){
        swapi(&x0, &y0);
        swapi(&x1, &y1);
        steep = 1;
    }
    if(x0>x1){
        swapi(&x0, &x1);
        swapi(&y0, &y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int D = abs(dy)*2;
    int error = 0;
    int y = y0;
    for(int x=x0; x<=x1; x++){
        if(steep){
            setPixel(image, y, x, color);
        }else{
            setPixel(image, x, y, color);
        }
        error += D;
        if(error > dx){
            y += (y1>y0?1:-1);
            error -= dx*2;
        }
    }
}


void triangle(struct vertexi v0, struct vertexi v1, struct vertexi v2, 
        struct TGA_image image){

    if(v0.y > v1.y){swapv(&v0, &v1);};
    if(v1.y > v2.y){swapv(&v1, &v2);};
    if(v1.y > v2.y){swapv(&v1, &v2);};
    line(v0.x, v0.y, v2.x, v2.y, image, blue);
    line(v0.x, v0.y, v1.x, v1.y, image, red);
    line(v1.x, v1.y, v2.x, v2.y, image, red);

    setPixel(image, (v2.x- v0.x), (v2.y-v0.y), white);

    /*
    //move up from bottom vertex y to top
    for(int i = v0.y; i < v2.y; i++){
        //draw horizontal line from point on one side to other
        //each y level find bounding points of each side and draw
        line(_, i, _, i, image, red);
    }*/
}
int main(int argc, char* argv[]){

    /*
    struct OBJ_Model model;
    if(2 == argc){
        model = loadModel(argv[1]);
    }else{
        model = loadModel("obj/african_head.obj");
    }
    */

    //struct TGA_image image = loadTGA(argv[1]);
    struct TGA_image image = createTGA(WIDTH, HEIGHT, RGB);

    //triangle 1
    struct vertexi v1 = {10, 10};
    struct vertexi v2 = {40, 35};
    struct vertexi v3 = {35, 60};

    //triangle 2 
    struct vertexi v4 = {50, 50};
    struct vertexi v5 = {70, 80};
    struct vertexi v6 = {50, 80};

    //triangle 3
    struct vertexi v7 = {40, 10};
    struct vertexi v8 = {65, 20};
    struct vertexi v9 = {55, 10};

    triangle(v1, v2, v3, image);
    triangle(v4, v5, v6, image);
    triangle(v7, v8, v9, image);



    /*
    //draw triangles based off object file
    for(int i = 0; i < model.nfaces; i++){
        struct face face = model.faces[i];
        for(int j = 0; j < 3;j++){
            //subtract 1 from index because it is relative
            struct vertexf v0 = model.vertices[face.indices[j]-1];
            //second index is moduloed because it will be 4 but needs
            //to be drawn to 1
            struct vertexf v1 = model.vertices[face.indices[(j+1)%3]-1];
            //we need to add 1 to shift to [0, 2] range for pixel cordinates
            //0 is bottom left 1 is middle and 2 is the top right.
            //We multiply by width and height divided by 2 so multiplying
            //by the middle is the middle of the width and height and 
            //mutliplying by the top right cancels the 2 so its
            //just width and height.
            int x0 = (v0.x + 1.)*WIDTH/2;
            int y0 = (v0.y + 1.)*HEIGHT/2;
            int x1 = (v1.x + 1.)*WIDTH/2;
            int y1 = (v1.y + 1.)*HEIGHT/2;
            line(x0, y0, x1, y1, image, white);
        }
    }
*/

    writeTGA(image, "tga/outfile.tga", 0);

    free(image.pixel_bytes);
    //freeObj(model);

    return 0;
}

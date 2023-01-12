#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SDL_MAIN_HANDLED
#include <SDL2/sdl.h>

struct vec3 {
    int x,y,z;
};

struct vec3 newvec3(double x,double y,double z) { // convenience function
    struct vec3 a;
    a.x=x;
    a.y=y;
    a.z=z;
    return a;
} 

struct vec2 {
    int x,y;
};

struct rgb {
    int r,g,b; // cant get chars to work
};

struct rgb rgbnew(int r,int g,int b) {
    struct rgb a;
    a.r=r;
    a.g=g;
    a.b=b;
    return a;
}

struct voxel {
    // vec3 size;
    struct vec3 pos;
    struct rgb color;
};

struct img2 {
    struct vec2 size;
    struct rgb **pix;
};

int ispointinvoxel(struct vec3 point,struct voxel *voxels,int voxel_size) {
    //printf("Ray at %i:%i:%i\n",point.x,point.y,point.z);
    int loop;
    for (loop=0;loop<voxel_size;loop++) {// search throguh all voxels to check if our ray intersects with one
        // check if the ray is in a voxel
        if (voxels[loop].pos.x==point.x && voxels[loop].pos.y==point.y && voxels[loop].pos.z==point.z) { 
            return loop; // return ID of voxel
        }
    }
    return -1; // return nothing
}

struct rgb raycast(struct vec3 start,struct vec3 velocity,struct voxel *voxels,int voxel_size) {
    int keepgoing=0;
    int dist=0;
    struct vec3 current = newvec3(start.x,start.y,start.z);
    int rslt;
    while (1) {
        dist++;
        current = newvec3(current.x+velocity.x,current.y+velocity.y,current.z+velocity.z);
        rslt = ispointinvoxel(current,voxels,voxel_size);
        if (rslt>=0) {
            break;
        }
        if (dist>50) {
            return rgbnew(0,0,0); // white
        }
    }
    return voxels[rslt].color; // use the ID that ispointinvoxel() returned to give a voxels RGB
}

void makescreen(struct rgb a[200][200],struct voxel *voxels,int voxel_size) {
    // make 200*200 image
    int loop,loop2;
    // actually place pixels onto the image by using raycast()
    for (loop=0;loop<200;loop++) { // our x
        for (loop2=0;loop2<200;loop2++) { // our y
            a[loop][loop2] = raycast(newvec3(0,loop-100,loop2-100),newvec3(1,0,0),voxels,voxel_size);
        }
    }
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_CreateWindowAndRenderer(200, 200, 0, &window, &render);
    int loop,loop2;
    // initialize screen
    struct rgb screen[200][200];
    struct voxel *voxels = (struct voxel *) malloc(sizeof(struct voxel)*1); // all our voxels
    voxels[0].pos = newvec3(1,0,0);
    voxels[0].color = rgbnew(255,255,255);
    int voxel_size = 1;
    int close = 0;
    
    SDL_Event e;
    while (close==0) {
        // render
        while(SDL_PollEvent(&e) > 0) {
            switch(e.type) {
                case SDL_QUIT:
                    close=1;
                    break;
            }
        }
        SDL_RenderClear(render);
        // render here
        makescreen(screen,voxels,voxel_size);
        for (loop=0;loop<200;loop++) { //loop our x
            for (loop2=0;loop2<200;loop2++) { // loop2 our y
                SDL_SetRenderDrawColor(render, screen[loop][loop2].r, screen[loop][loop2].b, screen[loop][loop2].g, 255);
                SDL_RenderDrawPoint(render, loop, loop2);
            }
        }
        SDL_RenderPresent(render);
        //SDL_UpdateWindowSurface(window);
    }
}
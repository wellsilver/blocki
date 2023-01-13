#include <stdio.h>
#include <stdlib.h>
#include "blocki.h" // the header
#define SDL_MAIN_HANDLED
#include <SDL2/sdl.h>

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    blocki_init();
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_CreateWindowAndRenderer(200, 200, 0, &window, &render);
    int loop,loop2;
    // initialize screen
    struct rgb screen[200][200];
    struct voxel *voxels = (struct voxel *) malloc(sizeof(struct voxel)*1); // all our voxels
    voxels[0].pos = newvec3(1,0,0);
    voxels[0].size = newvec3(3,3,3);
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
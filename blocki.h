#include <math.h>

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
    struct vec3 size;
    struct rgb color;
};

struct img2 {
    struct vec2 size;
    struct rgb **pix;
};

struct voxel *voxels;
int voxel_size;

void blocki_init() {
    voxels = (struct voxel *) malloc(sizeof(struct voxel)*1);
    voxel_size=1;
}

int ispointinvoxel(struct vec3 point) {
    //printf("Ray at %i:%i:%i\n",point.x,point.y,point.z);
    int loop;
    for (loop=0;loop<voxel_size;loop++) {// search throguh all voxels to check if our ray intersects with one
        // check if the ray is in a voxel
        if (voxels[loop].pos.x <= point.x && voxels[loop].pos.x+voxels[loop].size.x >= point.x) { // if its in the x
            if (voxels[loop].pos.y <= point.y && voxels[loop].pos.y+voxels[loop].size.y >= point.y) { // if its in the y
                if (voxels[loop].pos.z <= point.z && voxels[loop].pos.z+voxels[loop].size.z >= point.z) { // if its in the z
                    return loop;
                }
            }
        }
    }
    return -1; // return nothing
}

struct rgb raycast(struct vec3 start,struct vec3 velocity) {
    int keepgoing=0;
    int dist=0;
    struct vec3 current = newvec3(start.x,start.y,start.z);
    int rslt;
    while (1) {
        dist++;
        current = newvec3(current.x+velocity.x,current.y+velocity.y,current.z+velocity.z);
        rslt = ispointinvoxel(current);
        if (rslt>=0) {
            break;
        }
        if (dist>50) {
            return rgbnew(0,0,0); // white
        }
    }
    return voxels[rslt].color; // use the ID that ispointinvoxel() returned to give a voxels RGB
}

void makescreen(struct rgb a[200][200]) {
    // make 200*200 image
    int loop,loop2;
    // actually place pixels onto the image by using raycast()
    for (loop=0;loop<200;loop++) { // our x
        for (loop2=0;loop2<200;loop2++) { // our y
            a[loop][loop2] = raycast(newvec3(0,loop-100,loop2-100),newvec3(1,0,0));
        }
    }
}

// makes a new voxel and returns its ID
int new_voxel(struct vec3 pos,struct vec3 size,struct rgb color) {
    voxel_size++;
    voxels = (struct voxel *) realloc(voxels,sizeof(struct voxel)*voxel_size);
    voxels[voxel_size-1].pos = pos;
    voxels[voxel_size-1].size = size;
    voxels[voxel_size-1].color = color;
    return voxel_size - 1;
}
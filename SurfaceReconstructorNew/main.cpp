
#include "SurfaceReconstructor.hpp"



int main(){
    printf("Welcome to Surface Reconstruction.\n");
    SurfaceReconstructor surfaceReconstructor;
    
    surfaceReconstructor.LoadParticle("test.txt");
    surfaceReconstructor.SetupGrids();
    
    surfaceReconstructor.ExtractSurface();
    //surfaceReconstructor.OutputMesh("test.obj");
    return 0;
}
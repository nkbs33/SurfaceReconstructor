#pragma once

#include "ParticleData.hpp"
#include "ZIndexGrid.hpp"
#include "SPHHelper.hpp"

class GridVertex{
    public:
    int surfaceIndex;
};

class SurfaceVertex{
    public:
    int gridIndex;
    cint3 vertexIds;
};

class SurfaceGrid{
    public:
    cfloat3 xmin;
    cfloat3 xmax;
    float cellWidth;
    cint3 resolution;
    cint3 vertexResolution;
    vector<GridVertex> vertices;
    vector<SurfaceVertex> surfaceVertices;

    int GetVertexIndex(cint3 coord){
        if(coord.x<0 || coord.x>=vertexResolution.x ||
        coord.y<0 || coord.y>=vertexResolution.y ||
        coord.z<0 || coord.z>=vertexResolution.z)
            return INVALID_CELL;

        return coord.z*(vertexResolution.y*vertexResolution.x)
        + coord.y*vertexResolution.x + coord.x;
    }
    cint3 GetVertexCoord(int index){
        cint3 coord;
        coord.z = index / (vertexResolution.x * vertexResolution.y);
        coord.y = index % (vertexResolution.x * vertexResolution.y) / vertexResolution.x;
        coord.x = index % vertexResolution.x;
        return coord;
    }
    cfloat3 GetVertexPosition(int index){
        return xmin + GetVertexCoord(index)*cellWidth;
    }
    cint3 GetVertexCoord(cfloat3 p){
        cint3 coord;
        coord = floor((p - xmin)/cellWidth);
        return coord;
    }

    void Init(){
        resolution = ceil((xmax-xmin)/cellWidth);
        xmax = xmin + resolution * cellWidth;
        vertexResolution = resolution+cint3(1,1,1);
        vertices.resize(vertexResolution.prod());
        surfaceVertices.clear();
        for(int i=0; i<vertices.size(); i++)
            vertices[i].surfaceIndex = -1;
    }
};

class SurfaceReconstructor{
    private:
    ParticleData particleData;
    ZIndexGrid zGrid;
    SPHHelper sphHelper;
    SurfaceGrid surfaceGrid;
    
    float particleSpacing;
    float infectRadius;
    cfloat3 padding;
    float normThres;
    int neighborThres;
    veci surfaceParticleMark;

    public:
    SurfaceReconstructor(){
        particleSpacing = 1;
        infectRadius = 2;
        padding.Set(infectRadius,infectRadius,infectRadius);
        sphHelper.SetupCubic(infectRadius/2);
        normThres = 0.5;
        neighborThres = 25;
    }
    void LoadParticle(char* filePath){
        particleData.LoadFromFile(filePath);
        particleData.Analyze();
        surfaceParticleMark.resize(particleData.Size());
    }

    void SetupGrids(){
        SetupSurfaceGrid();
        SetupZGrid();
    }
    
    void SetupSurfaceGrid(){
        surfaceGrid.cellWidth = 0.5;  
        surfaceGrid.xmin = particleData.GetXMin()-padding;
        surfaceGrid.xmax = particleData.GetXMax()+padding;
        surfaceGrid.Init();
    }

    void SetupZGrid();
    
    void ExtractSurface();

    void SortParticles();
    void ReorderParticles();
    
    void ExtractSurfaceParticles();
    void ComputeColorFieldAndMarkParticles();

    void ExtractSurfaceVertices();

    void ComputeScalarValues();

    void Triangulate();

    void OutputMesh(char* filePath){

    }
};
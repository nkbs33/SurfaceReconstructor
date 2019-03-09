#pragma once

#include "ParticleData.hpp"
#include "ZIndexGrid.hpp"
#include "SPHHelper.hpp"
#include "SurfaceGrids.h"

#include "MarchingCube.h"
#include "catpaw/cpXMLhelper.h"

class SurfaceReconstructor{
public:

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

    
    SurfaceReconstructor(){
        particleSpacing = 1;
        infectRadius = 2;
        padding.Set(infectRadius,infectRadius,infectRadius);
        sphHelper.SetupCubic(infectRadius/2);
        normThres = 0.5;
        neighborThres = 25;
		//LoadConfig();
    }

	void LoadConfig(const char* config) {
		XMLDocument doc;
		int xmlState = doc.LoadFile("config.xml");
		Tinyxml_Reader reader;

		XMLElement* param = doc.FirstChildElement("SurfaceReconstruction")->FirstChildElement(config);
		reader.Use(param);
		particleSpacing = reader.GetFloat("particleSpacing");
		infectRadius = reader.GetFloat("infectRadius");
		float paddingx = reader.GetFloat("padding");
		padding.Set(paddingx, paddingx, paddingx);
		
		sphHelper.SetupCubic(infectRadius*0.5);
		normThres = reader.GetFloat("normThreshold");
		neighborThres = reader.GetInt("neighborThreshold");
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
    
    void SetupSurfaceGrid();
    void SetupZGrid();
    
    void ExtractSurface();

    void SortParticles();
    
    void ExtractSurfaceParticles();
    void ComputeColorFieldAndMarkParticles();

    void ExtractSurfaceVertices();
	

    void ComputeScalarValues();

    void Triangulate();

    void OutputMesh(char* filePath){

    }
};
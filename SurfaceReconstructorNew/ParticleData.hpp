#pragma once

#include "catpaw/vec_define.h"
#include "catpaw/geometry_helper.h"

struct Particle{
    cfloat3 pos;
};

class ParticleData{
    private:
    vector<Particle> particles;

    cfloat3 xmin;
    cfloat3 xmax;

    public:
    vector<Particle>& GetParticleArray(){
        return particles;
    }
    Particle& GetParticle(int id){
        return particles[id];
    }
    cfloat3 GetXMin(){
        return xmin;
    }
    cfloat3 GetXMax(){
        return xmax;
    }
    int Size(){
        return particles.size();
    }

    void Analyze(){
        xmin.Set(99999,99999,99999);
        xmax.Set(-99999,-99999,-99999);
        for(int i=0; i<particles.size(); i++){
            xmin = minfilter(particles[i].pos, xmin);
            xmax = maxfilter(particles[i].pos, xmax);
        }
        
        printf3("xmin",xmin);
        printf3("xmax",xmax);
    }

    void LoadFromFile(const char* filePath){
        FILE* fp = fopen(filePath, "r");
        if(fp==NULL){
            printf("Error opening files.\n");
            return;
        }
        char buf[1024];
        fgets(buf, sizeof(buf), fp);
        printf("%s",buf);

        int vId;
        Particle tmpParticle;

        while(fgets(buf,sizeof(buf),fp)!=NULL){
            sscanf(buf, "%d %f %f %f",
            &vId, &tmpParticle.pos.x, &tmpParticle.pos.y, &tmpParticle.pos.z);
            particles.push_back(tmpParticle);
        }

        fclose(fp);
    }
};


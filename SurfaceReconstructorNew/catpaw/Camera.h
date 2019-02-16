#pragma once

#include "geometry.h"
#include "GLFW/glfw3.h"

class KeyStack{
public:
	int buffer[32];
	int maxlen,lenth;
	KeyStack(){
		maxlen = 32;
		lenth = 0;
		memset(buffer, 0, sizeof(buffer));
	}
	int findkey(uchar key){
		int tmp = -1;
		for (int i=0; i<lenth; i ++) {
			if (buffer[i]==key) {
				return i;
			}
		}
		return tmp;
	}
	void push(uchar key){
		if(lenth==31){
			printf("key stack full\n");
			return;
		}
		int id = findkey(key);
		if (id>=0)
			printf("key exists.\n");
		else{
			buffer[lenth] = key;
			lenth += 1;
		}	
	}

	void pop(uchar key){
		int id = findkey(key);
		if(id<0)
			printf("key to pop unfound.\n");
		else{
			for(int i=id; i<lenth-1;i++){
				buffer[i] = buffer[i+1];
			}
			lenth -= 1;
		}
	}
};

class cCamera{
public:

	cmat4 projmat;
	cmat4 viewmat;

	//projection matrix
	float fovy, aspect_ratio, nearclip, farclip;

	cfloat3 dir,up,right,forceup;
	cfloat3 pos, target;
	
	//keyboard translate
	cfloat3 vel;
	float velf, velMax;
	KeyStack wasd;

	//drag rotation
	cfloat2 rotatexy;
	cfloat2 transxy;
	//drag zoom
	float zoomin;
	
	cCamera(){
		velf = 0.0f;
		rotatexy.Set(0,0);
		transxy.Set(0,0);
	}

	//ViewMatrix Utility
	cmat4 CameraRotateMat();
	void CameraTranslateMat(cmat4& rotation, cfloat3& pos);
	
	//ProjectionMatrix Utility
	void SetProjParam(float f,float a,float nc,float fc){
		fovy=f; aspect_ratio=a; nearclip=nc; farclip=fc;
	}
	void ProjectionMat(){
		projmat = CreateProjectionMatrix(fovy, aspect_ratio, nearclip, farclip);
	}


	void lookat(cfloat3 _pos, cfloat3 _target);

	//Moving Utility
	void MoveCamera(cfloat3 dx){
		pos = pos+dx;
		target = target+dx;
		lookat(pos, target);
	}

#define FRONT_VEL 0
#define RIGHT_VEL 1
#define UP_VEL 2

	void getVelDir(){
		if(wasd.lenth==0)
			return;

		switch (wasd.buffer[wasd.lenth-1]) {
		case GLFW_KEY_W:
			vel = dir * (-1);
			break;
		case GLFW_KEY_S:
			vel = dir ;
			break;
		case GLFW_KEY_D:
			vel = right ;
			break;
		case GLFW_KEY_A:
			vel = right * (-1);
			break;
		case GLFW_KEY_Q:
			vel = up;
			break;
		case GLFW_KEY_Z:
			vel = up * (-1);
			break;
		}
	}

	void SetVelocty(int key){
		wasd.push(key);
	}

	void UnSetVelocity(int key){
		wasd.pop(key);
	}

	void AdvanceCamera(float dt);


};
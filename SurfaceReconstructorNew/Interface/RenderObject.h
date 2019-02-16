#pragma once
#include "catpaw/glUtils.h"
#include "catpaw/vec_define.h"



class RenderObjectBase {
protected:
	varr vbuffer;
	uarr bufferids;
	shaderObject shaderobj;
	int maxlen;
	int len;

	GLuint ModelMatrixUniformLocation;
	GLuint ViewMatrixUniformLocation;
	GLuint ProjectionMatrixUniformLocation;

	cmat4 ProjectionMatrix;
	cmat4 ViewMatrix;
	cmat4 ModelMatrix;

public:
	int GetShaderProgram() {
		return shaderobj.getProgramId();
	}
};




class ParticleRO :public RenderObjectBase {
private:
	GLuint ParticleSizeUniformLocation;
	GLuint texture;
	float particlesz;
public:
	ParticleRO();
	~ParticleRO();
	void SetupBuffer();
	void SetupShader();
	void SetupTexture();
	void Draw(cfloat3* x_p, cfloat4* color_p, cCamera& camera, int sz);
	void setParticleSize(float sz){particlesz = sz;}
};






class CubeRO :public RenderObjectBase {
private:
	GLuint ParticleSizeUniformLocation;
public:
	CubeRO();
	void SetupBuffer();
	void SetupShader();
	void Draw(vertex* data, cmat4* marr, cCamera& camera, int sz);
};




struct GeometryEntity {
	varr vertices;
	uarr indices;
};

class GeometryRO: public RenderObjectBase {
public:
	GeometryRO();
	void SetupBuffer();
	void SetupShader();
	void Draw(GeometryEntity* geoE, cCamera& camera);
};






class TriangleRO :public RenderObjectBase {
private:
	GLuint ParticleSizeUniformLocation;
	GLuint texture;
	float particlesz;
	int indexedvbo_lenth;
public:
	TriangleRO();
	~TriangleRO();
	void SetupBuffer();
	void UpdateIndexedVBO(vector<unsigned int>& indexedvbo);
	void SetupShader();
	void SetupTexture();
	void Draw(cfloat3* x_p, cfloat4* color_p, cCamera& camera, int sz);
	void setParticleSize(float sz) { particlesz = sz; }
};
#include "RenderObject.h"
#include "SOIL.h"
#include "catpaw/geometry.h"

ParticleRO::ParticleRO() {
	SetupBuffer();
	SetupTexture();
	SetupShader();

	ModelMatrix = IDENTITY_MAT;
}

void ParticleRO::SetupBuffer() {
	bufferids.resize(3);

	glGenVertexArrays(1, &bufferids[0]);
	glBindVertexArray(bufferids[0]);
	glEnableVertexAttribArray(0);//pos
	glEnableVertexAttribArray(1);//color

	maxlen = len = 1;
	glGenBuffers(2, &bufferids[1]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat3)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat4)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void ParticleRO::SetupShader() {
	shaderobj.loadShader("shader/SimpleShader.vertex.glsl", GL_VERTEX_SHADER);
	shaderobj.loadShader("shader/SimpleShader.geometry.glsl", GL_GEOMETRY_SHADER);
	shaderobj.loadShader("shader/SimpleShader.fragment.glsl", GL_FRAGMENT_SHADER);

	shaderobj.LinkProgram();
	
	ModelMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ModelMatrix");
	ViewMatrixUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "ViewMatrix");
	ProjectionMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ProjectionMatrix");
	ParticleSizeUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "particle_size");

	ExitOnGLError("ERROR: Could not get the shader uniform locations");

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");
}

void ParticleRO::SetupTexture() {
	int width, height;
	unsigned char* image = SOIL_load_image("drop.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleRO::Draw(cfloat3* x_p, cfloat4* color_p, cCamera& camera, int sz) {
	
	if(sz == 0)
		return;
	
	ProjectionMatrix = camera.projmat;
	ViewMatrix = camera.viewmat;

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.data);
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.data);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, camera.projmat.data);
	glUniform1f(ParticleSizeUniformLocation, particlesz);
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(bufferids[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat3)*sz, x_p, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat4)*sz, color_p, GL_STATIC_DRAW);

	//glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex)*(sz), data);


	glDrawArrays(GL_POINTS, 0, sz);
	ExitOnGLError("ERROR: Could not draw the Particle System");

	glBindVertexArray(0);
	glUseProgram(0);
}

ParticleRO::~ParticleRO() {
	shaderobj.Release();

	glDeleteBuffers(1, &bufferids[1]);
	glDeleteVertexArrays(1, &bufferids[0]);
	glDeleteTextures(1, &texture);
}




//==========================================
//
//
//            CUBE Render Object
//
//
//==========================================


CubeRO::CubeRO() {
	SetupBuffer();
	SetupShader();
}

void CubeRO::SetupBuffer() {
	bufferids.resize(3);

	glGenVertexArrays(1, &bufferids[0]);
	glBindVertexArray(bufferids[0]);
	glEnableVertexAttribArray(0);//pos
	glEnableVertexAttribArray(1);//color
	glEnableVertexAttribArray(2);//vec1
	glEnableVertexAttribArray(3);//vec2
	glEnableVertexAttribArray(4);//vec3
	glEnableVertexAttribArray(5);//vec4


	maxlen = len = 1;
	glGenBuffers(2, &bufferids[1]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)sizeof(cfloat3));
	
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cmat4)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(cmat4), 0);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(cmat4), (void*)(sizeof(cfloat4)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(cmat4), (void*)(sizeof(cfloat4)*2));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(cmat4), (void*)(sizeof(cfloat4)*3));

	glBindVertexArray(0);
}

void CubeRO::SetupShader() {
	shaderobj.loadShader("shader/Cubic.vertex.glsl", GL_VERTEX_SHADER);
	shaderobj.loadShader("shader/Cubic.geometry.glsl", GL_GEOMETRY_SHADER);
	shaderobj.loadShader("shader/Cubic.fragment.glsl", GL_FRAGMENT_SHADER);

	shaderobj.LinkProgram();

	ModelMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ModelMatrix");
	ViewMatrixUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "ViewMatrix");
	ProjectionMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ProjectionMatrix");
	ParticleSizeUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "particle_size");

	ExitOnGLError("ERROR: Could not get the shader uniform locations");

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");
}

void CubeRO::Draw(vertex* data, cmat4* marr, cCamera& camera, int sz) {
	ModelMatrix = IDENTITY_MAT;

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.data);
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.data);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, camera.projmat.data);
	glUniform1f(ParticleSizeUniformLocation, 1.0f);
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(bufferids[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	//Update Particle Data
	if (maxlen < sz)
		maxlen = sz;

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*maxlen, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex)*sz, data);

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cmat4)*maxlen, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cmat4)*sz, marr);

	glDrawArrays(GL_POINTS, 0, sz);
	ExitOnGLError("ERROR: Could not draw the Particle System");

	glBindVertexArray(0);
	glUseProgram(0);
}



//==========================================
//
//
//            Geometry Render Object
//
//
//==========================================

GeometryRO::GeometryRO() {
	SetupBuffer();
	SetupShader();
}

void GeometryRO::SetupBuffer() {
	bufferids.resize(3);

	glGenVertexArrays(1, &bufferids[0]);
	glBindVertexArray(bufferids[0]);
	glEnableVertexAttribArray(0);//pos
	glEnableVertexAttribArray(1);//color

	maxlen = len = 100;
	glGenBuffers(2, &bufferids[1]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)sizeof(cfloat3));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(maxlen), NULL, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void GeometryRO::SetupShader() {
	shaderobj.loadShader("shader/box.vertex.glsl", GL_VERTEX_SHADER);
	shaderobj.loadShader("shader/box.fragment.glsl", GL_FRAGMENT_SHADER);

	shaderobj.LinkProgram();

	ModelMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ModelMatrix");
	ViewMatrixUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "ViewMatrix");
	ProjectionMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ProjectionMatrix");
	
	ExitOnGLError("ERROR: Could not get the shader uniform locations");
}



void GeometryRO::Draw(GeometryEntity* geoE, cCamera& camera) {
	ModelMatrix = IDENTITY_MAT;
	ViewMatrix = camera.viewmat;
	

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.data);
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.data);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, camera.projmat.data);
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(bufferids[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*geoE->vertices.size(), geoE->vertices.data(), GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glPointSize(3.0f);
	
	//glDrawArrays(GL_POINTS, 0, 8);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*geoE->indices.size(), geoE->indices.data(), GL_STATIC_DRAW);

	glDrawElements(GL_LINES, geoE->indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);
	glUseProgram(0);
}


//==========================================
//
//
//           Triangle Render Object
//
//
//==========================================

TriangleRO::TriangleRO() {
	SetupBuffer();
	SetupTexture();
	SetupShader();

	ModelMatrix = IDENTITY_MAT;
}

void TriangleRO::SetupBuffer() {
	bufferids.resize(4);

	glGenVertexArrays(1, &bufferids[0]);
	glBindVertexArray(bufferids[0]);
	glEnableVertexAttribArray(0);//pos
	glEnableVertexAttribArray(1);//color

	maxlen = len = 1;
	glGenBuffers(3, &bufferids[1]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat3)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat4)*maxlen, NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//Indexed VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferids[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*maxlen, NULL, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TriangleRO::UpdateIndexedVBO(vector<unsigned int>& indexedvbo) {

	indexedvbo_lenth = indexedvbo.size();
	glBindVertexArray(bufferids[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferids[3]);
	if(indexedvbo.size()>0)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indexedvbo.size(), &indexedvbo[0], GL_STATIC_DRAW);
	else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void TriangleRO::SetupShader() {
	shaderobj.loadShader("shader/TriangleShader.vertex.glsl", GL_VERTEX_SHADER);
	//shaderobj.loadShader("shader/TriangleShader.geometry.glsl", GL_GEOMETRY_SHADER);
	shaderobj.loadShader("shader/TriangleShader.fragment.glsl", GL_FRAGMENT_SHADER);

	shaderobj.LinkProgram();

	ModelMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ModelMatrix");
	ViewMatrixUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "ViewMatrix");
	ProjectionMatrixUniformLocation =	glGetUniformLocation(shaderobj.getProgramId(), "ProjectionMatrix");
	ParticleSizeUniformLocation =		glGetUniformLocation(shaderobj.getProgramId(), "particle_size");

	ExitOnGLError("ERROR: Could not get the shader uniform locations");

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");
}

void TriangleRO::SetupTexture() {
	int width, height;
	unsigned char* image = SOIL_load_image("drop.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TriangleRO::Draw(cfloat3* x_p, cfloat4* color_p, cCamera& camera, int sz){
	if(sz==0) return;

	ProjectionMatrix = camera.projmat;
	ViewMatrix = camera.viewmat;

	glUseProgram(shaderobj.getProgramId());
	ExitOnGLError("ERROR: Could not use the shader program");

	glUniformMatrix4fv(ModelMatrixUniformLocation, 1, GL_FALSE, ModelMatrix.data);
	glUniformMatrix4fv(ViewMatrixUniformLocation, 1, GL_FALSE, ViewMatrix.data);
	glUniformMatrix4fv(ProjectionMatrixUniformLocation, 1, GL_FALSE, camera.projmat.data);
	glUniform1f(ParticleSizeUniformLocation, particlesz);
	ExitOnGLError("ERROR: Could not set the shader uniforms");

	glBindVertexArray(bufferids[0]);
	ExitOnGLError("ERROR: Could not bind the VAO for drawing purposes");

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat3)*sz, x_p, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bufferids[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cfloat4)*sz, color_p, GL_STATIC_DRAW);

	
	//glPointSize(10.0f);
	//glDrawArrays(GL_POINTS, 0,8);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, indexedvbo_lenth, GL_UNSIGNED_INT ,(void*)0);
	ExitOnGLError("ERROR: Could not draw the Particle System");
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
	glUseProgram(0);
}

TriangleRO::~TriangleRO() {
	shaderobj.Release();

	glDeleteBuffers(3, &bufferids[1]);
	glDeleteVertexArrays(1, &bufferids[0]);
	glDeleteTextures(1, &texture);
}
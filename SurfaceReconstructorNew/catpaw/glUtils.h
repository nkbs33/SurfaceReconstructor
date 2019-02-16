#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define GLFW_INCLUDE_GLU
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "vec_define.h"
#include "Camera.h"


void ExitOnGLError(const char* error_message);
GLuint LoadShader(const char* filename, GLenum shader_type);



class shaderObject {
private:
	GLuint programid;
	GLuint shaderid[3];//vertex, geometry, fragment
public:
	shaderObject() {
		programid = glCreateProgram();
		shaderid[0] = 0;
		shaderid[1] = 0;
		shaderid[2] = 0;
	}

	int getProgramId(){return programid;}

	void Release() {
		printf("A shader program is being destroyed.\n");
		for (int i=0; i<3; i++) {
			if (shaderid[i]>0) {
				glDetachShader(programid, shaderid[i]);
				glDeleteShader(shaderid[i]);
			}
		}
		glDeleteProgram(programid);
	}

	void loadShader(const char* filename, GLuint sdtype) {
		switch (sdtype) {
		case GL_VERTEX_SHADER:
			shaderid[0] = LoadShader(filename, sdtype);
			break;
		case GL_GEOMETRY_SHADER:
			shaderid[1] = LoadShader(filename, sdtype);
			break;
		case GL_FRAGMENT_SHADER:
			shaderid[2] = LoadShader(filename, sdtype);
			break;
		}
	}
	void LinkProgram() {
		for (int i=0; i<3; i++) {
			if (shaderid[i]>0) {
				glAttachShader(programid, shaderid[i]);
			}
		}
		glLinkProgram(programid);
		ExitOnGLError("ERROR: Could not link the shader program");
	}
};

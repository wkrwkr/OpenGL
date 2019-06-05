#pragma once
#include<GL/glew.h>
#include<GL/glut.h>
#include<GL/GL.h>
#include<GL/GLU.h>
#include<iostream>

class Graphics
{
public:
	Graphics(int, char *[]);
	~Graphics();
	void run();
private:
	int glInit(int argc, char *argv[]);
	GLuint makeProgram();
	GLuint compileShader(const char *filename, GLuint shaderType);
	char * readFile(const char *filename);

};


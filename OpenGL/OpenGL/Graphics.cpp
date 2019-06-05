#include "pch.h"
#include "Graphics.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int glInit(int argc, char *argv[]);
void display();
void idle();
bool compileErrorCheck(GLuint shader);

GLuint renderingProgram;
GLuint vertexArrayObject;
Graphics::Graphics(int argc, char *argv[])
{
	glInit(argc, argv);

}

Graphics::~Graphics()
{

}

void Graphics::run()
{
	glutMainLoop();
}

GLuint Graphics::makeProgram()
{
	GLuint program = glCreateProgram();
	std::vector<GLuint> shaders;
	GLuint shader;

	shaders.push_back(compileShader("vs.txt", GL_VERTEX_SHADER));
	shaders.push_back(compileShader("tcs.txt", GL_TESS_CONTROL_SHADER));
	shaders.push_back(compileShader("tes.txt", GL_TESS_EVALUATION_SHADER));
	//shaders.push_back(compileShader("gs.txt", GL_GEOMETRY_SHADER));
	shaders.push_back(compileShader("fs.txt", GL_FRAGMENT_SHADER));

	while (!shaders.empty()) {
		shader = shaders.back();
		shaders.pop_back();
		if (shader > 0)
			glAttachShader(program, shader);
		glDeleteShader(shader);
	}

	glLinkProgram(program);
	return program;
}

GLuint Graphics::compileShader(const char *fileName, GLuint shaderType)
{
	GLuint shader;
	const char * code = readFile(fileName);
	std::cout << "shader \"" << fileName << "\"";
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);

	if (compileErrorCheck(shader))
		std::cout << "is compiled successfully." << std::endl;
	else
		std::cout << "is failed to compile." << std::endl;

	delete code;
	return shader;
}

char *Graphics::readFile(const char *fileName)
{
	std::ifstream fin;
	auto fileLength = 0;
	char *data;
	char *cursor;
	uint32_t cnt = 0;

	fin.open(fileName, std::ios::in);
	fin.seekg(0, std::ios::end);
	fileLength = fin.tellg();
	fin.seekg(0, std::ios::beg);

	data = new char[fileLength + 1];
	cursor = data;

	while (fin.good())
	{
		fin.getline(cursor, fileLength);
		cursor += strnlen(cursor, fileLength);
		if ((int)(cursor - data) == fileLength) break;
		*(cursor++) = '\r';
		*(cursor++) = '\n';
	}
	*cursor = '\0';
	std::cout << "read " << (int)(cursor - data) << "/" << fileLength << " byte - ";
	fin.close();
	return data;
}

int Graphics::glInit(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1024, 768);
	glutCreateWindow("OpenGL");
	glutDisplayFunc(display);

	glewInit();
	//glutIdleFunc(idle);
	std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;


	glMatrixMode(GL_MODELVIEW);
	renderingProgram = makeProgram();
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutSwapBuffers();
	return 1;
}

bool compileErrorCheck(GLuint shader)
{
	GLint isCompiled;
	GLint maxLength = 0;
	std::vector<GLchar> errorLog;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

	return isCompiled == GL_TRUE;
}

void display()
{
	static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	glVertexAttrib4fv(1, red);
	glUseProgram(renderingProgram);
	glDrawArrays(GL_PATCHES, 0, 3);

	glutSwapBuffers();
}

void idle()
{
	;
}


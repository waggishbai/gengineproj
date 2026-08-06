#ifndef OGL_FUNCS_H
#define OGL_FUNCS_H



#include <glad/glad.h>

#include <simpfuncs/hpp/fileloader.hpp>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

GLuint compileShader(const char* shaderText, GLenum type, bool errorCheck) {
    if (errorCheck) {
        std::cout << shaderText << std::endl;	
    }
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderText, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {	    
	    
	    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
	    infoLog << std::endl;

    }
    return shader;
}

GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader) {

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return program;
}

//uniform setters

void mat4ToProgram(GLuint program, glm::mat4 mat, const char* varName) {

    GLint uVar = glGetUniformLocation(program, varName);
    glUniformMatrix4fv(uVar, 1, GL_FALSE, glm::value_ptr(mat));
}

void vec3ToProgram(GLuint program, glm::vec3 vec, const char* varName) {
    GLint uVar = glGetUniformLocation(program, varName);
    glUniform3f(uVar, vec.x, vec.y, vec.z);
}
GLuint compileShaderProgram(const char* vtext, const char* ftext) {
	GLuint vertex = compileShader(vtext, GL_VERTEX_SHADER, false);
	GLuint fragment = compileShader(ftext, GL_FRAGMENT_SHADER, false);

	GLuint outprog = compileProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return outprog;
}

GLuint compileShaderProgram(std::string vfile, std::string ffile) {
	std::string vtext = getFileText(vfile);
	std::string ftext = getFileText(ffile);

	const char* v = vtext.c_str();
	const char* f = ftext.c_str();

	GLuint vertex = compileShader(v, GL_VERTEX_SHADER, false);
	GLuint fragment = compileShader(f, GL_FRAGMENT_SHADER, false);

	GLuint outprog = compileProgram(vertex, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
	return outprog;
}	
#endif

/*===================================================================================*//**
	Shaders
	
	Utility class for loading shaders.
    
    Copyright 2017 Erick Fernandez de Arteaga. All rights reserved.
        https://www.linkedin.com/in/erick-fda
        https://bitbucket.org/erick-fda

	@author Chris Leclair, Erick Fernandez de Arteaga
	@version 0.0.0
	@file
	
	@see Shaders
	@see Shaders.h
	
*//*====================================================================================*/

/*========================================================================================
	Dependencies
========================================================================================*/
#include "Shaders.h"
#include "FileHelper.h"

/*========================================================================================
	Class Fields
========================================================================================*/
std::string Shaders::VSH_01_PATH = "ResourceFiles/Shaders/shader01.vsh";
std::string Shaders::FSH_01_PATH = "ResourceFiles/Shaders/shader01.fsh";
std::string Shaders::VSH_02_PATH = "ResourceFiles/Shaders/shader02.vsh";
std::string Shaders::FSH_02_PATH = "ResourceFiles/Shaders/shader02.fsh";

/*----------------------------------------------------------------------------------------
	Class Methods
----------------------------------------------------------------------------------------*/
///
///
///
GLuint Shaders::LoadShaders()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	std::string vsh = FileHelper::loadFileFromString(Shaders::VSH_01_PATH);
	const char* vshContent = vsh.c_str();
	glShaderSource(vertexShader, 1, &(vshContent), NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	std::string fsh = FileHelper::loadFileFromString(Shaders::FSH_01_PATH);
	const char* fshContent = fsh.c_str();
	glShaderSource(fragmentShader, 1, &(fshContent), NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

///
///
///
GLuint Shaders::LoadShadersFBDraw()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vertexShader, 1, &(Shaders::vertexShader2Source), NULL);
	std::string vsh = FileHelper::loadFileFromString(Shaders::VSH_02_PATH);
	const char* vshContent = vsh.c_str();
	glShaderSource(vertexShader, 1, &(vshContent), NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fragmentShader, 1, &fragmentShader2Source, NULL);
	std::string fsh = FileHelper::loadFileFromString(Shaders::FSH_02_PATH);
	const char* fshContent = fsh.c_str();
	glShaderSource(fragmentShader, 1, &(fshContent), NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}

	// link shaders
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		SDL_Log("%s", infoLog);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

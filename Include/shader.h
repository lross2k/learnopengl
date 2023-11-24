#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#pragma warning(disable:6386)

struct shader {
	unsigned int id;
	void (*Use)(struct shader* shader);
	void (*setBool)(struct shader* shader, const char* name, bool value);
	void (*setInt)(struct shader* shader, const char* name, int value);
	void (*setFloat)(struct shader* shader, const char* name, float value);
};

void shader_use(struct shader* shader)
{
	glUseProgram(shader->id);
}

void shader_set_bool(struct shader* shader, const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(shader->id, name), (int)value);
}

void shader_set_int(struct shader* shader, const char* name, int value)
{
	glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void shader_set_float(struct shader* shader, const char* name, float value)
{
	glUniform1f(glGetUniformLocation(shader->id, name), value);
}

void shader_init(struct shader* shader, const char* vertex_path, const char* fragment_path)
{
	shader->setBool = &shader_set_bool;
	shader->setFloat = &shader_set_float;
	shader->setInt = &shader_set_int;
	shader->Use = &shader_use;

	// Vertex file reading
	FILE* vertex_file;
	fopen_s(&vertex_file, vertex_path, "rb");
	if (!vertex_file) {
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		return;
	}
	fseek(vertex_file, 0, SEEK_END);
	long fsize = ftell(vertex_file);
	fseek(vertex_file, 0, SEEK_SET);  /* same as rewind(f); */

	char* vertex_code = (char*)malloc(fsize + 1);
	if (!vertex_code) {
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		return;
	}

	fread(vertex_code, fsize, 1, vertex_file);
	fclose(vertex_file);

	vertex_code[fsize] = 0;

	// Fragment file reading
	FILE* fragment_file;
	fopen_s(&fragment_file, fragment_path, "rb");
	if (!fragment_file) {
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		return;
	}
	fseek(fragment_file, 0, SEEK_END);
	fsize = ftell(fragment_file);
	fseek(fragment_file, 0, SEEK_SET);  /* same as rewind(f); */

	char* fragment_code = (char*)malloc(fsize + 1);
	if (!fragment_code) {
		printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
		return;
	}

	fread(fragment_code, fsize, 1, fragment_file);
	fclose(fragment_file);

	fragment_code[fsize] = 0;

	// compile the shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_code, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n");
		printf("%s\n", infoLog);
	};

	// similiar for Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_code, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n");
		printf("%s\n", infoLog);
	};

	// shader Program
	shader->id = glCreateProgram();
	glAttachShader(shader->id, vertex);
	glAttachShader(shader->id, fragment);
	glLinkProgram(shader->id);
	// print linking errors if any
	glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader->id, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

};

#endif//SHADER_H

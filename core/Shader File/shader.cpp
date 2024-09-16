#include "shader.h"
#include <GLFW/glfw3.h>

namespace shaderFile 
{
	
		Shader::Shader(const char* vertexPath, const char* fragmentPath)
		{
			
			// 1. retrieve the vertex/fragment source code from the filePath
			std::string vertexCode;
			std::string fragmentCode;
			std::ifstream vShaderFile;
			std::ifstream fShaderFile;

			// ensure ifstream objects can throw exceptions
			vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				// open files 
				vShaderFile.open(vertexPath);
				fShaderFile.open(fragmentPath);
				std::stringstream vShaderStream, fShaderStream;
				// read files buffer contents into streams
				vShaderStream << vShaderFile.rdbuf();
				fShaderStream << fShaderFile.rdbuf();
				// close file handlers
				vShaderFile.close();
				fShaderFile.close();
				// convert the stream into a string
				vertexCode = vShaderStream.str();
				fragmentCode = fShaderStream.str();
			}
			catch (std::ifstream::failure e)
			{
				std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
			}
			const char* vShaderCode = vertexCode.c_str();
			const char* fShaderCode = fragmentCode.c_str();

			unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vShaderCode, NULL);
			glCompileShader(vertexShader);

			
			int success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
			}

			// create and compile fragment shader
			unsigned int fragmentShader;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
			glCompileShader(fragmentShader);

			if (!success)
			{
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
			}

			// Create shader program, link
			unsigned int shaderProgram;
			shaderProgram = glCreateProgram();
			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);
			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

			ID = glCreateProgram();
			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);
			glLinkProgram(ID);
			
			if (!success)
			{
				glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
				printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
			}

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			
		};

		void Shader::use()
		{
			glUseProgram(ID);
		}
		
		void Shader::setBool(const std::string& name, bool value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
		}
		void Shader::setInt(const std::string& name, int value) const
		{
			glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
		}
		void Shader::setFloat(const std::string& name, float value) const
		{
			glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
		}
}
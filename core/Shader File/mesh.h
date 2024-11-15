#ifndef MESH_H
#define MESH_H


#include <glm/ext.hpp>

#include "../ew/ewMath/ewMath.h"

#include "../Shader File/shader.h"

#include <vector>
#include <string>
using namespace std; 

namespace ShaderFile
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture
	{
		unsigned int id;
		string type;
		string path;
	};

	class Mesh
	{
	public:
		vector<Vertex>		  vertices; 
		vector<unsigned int>  indices;
		vector<Texture>		  textures;
		unsigned int VAO;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		void Draw(shaderFile::Shader &shader);
	private:
		unsigned int VBO, EBO; 

		void setupMesh();

	};
}

#endif
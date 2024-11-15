#include "mesh.h"



namespace ShaderFile
{
	

	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		// sets the mesh public vars to the mesh constructor arguments
		this->vertices = vertices; 
		this->indices  = indices;
		this->textures = textures;

		setupMesh();
	}

	void Mesh::Draw(shaderFile::Shader &shader)
	{
		unsigned int diffuseNr  = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr   = 1; 
		unsigned int heightNr   = 1;
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding 
			// retrive texture number (the N in diffuse_texture0) 
			string number; 
			string name = textures[i].type; 
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++); // transfer unsigned int to string
			else if (name == "texture_specular")
				number = number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++); // transfer unsigned int to string
			else if (name == "texture_height")
				number = std::to_string(heightNr++); // transfer unsigned int to string

			// now set the sampler to the correct texture unit 
			shader.setInt(("material." + name + number).c_str(), i);
			// bind the texture
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		
		// draw mesh 
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// its good practice to set everything back to defuals once configured
		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::setupMesh()
	{
		// creates buffer arrays
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		// load data into vertex buffers
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// Since a structs memory layout is sequential  then we can just pass a pointer to the struct and it translates 
		// to glm::vec3/2 array which translates to 3/2 floats whihc then translates to a byte array
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		// vertex normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		// texture coords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		glBindVertexArray(0);
	}

	
}
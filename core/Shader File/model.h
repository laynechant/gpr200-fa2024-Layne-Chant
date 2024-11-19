#ifndef MODEL_H
#define MODEL_H


#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "mesh.h"
#include "shader.h"
#include "../ew/ewMath/ewMath.h"


#include <glm/gtc/matrix_transform.hpp>


#include <map>
#include <vector>
using namespace std;

namespace ShaderFile
{
	class Model
	{ 
	public: 
		vector<Texture> textures_loaded;

		vector<Mesh> meshes;
		string directory;
		Model(string const& path, bool gamma = false); 
		
		void draw(shaderFile::Shader &shader);

	private: 

		vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
		void loadModel(string const &path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	};
}
#endif // !MODEL_H

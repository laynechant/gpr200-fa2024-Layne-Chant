#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "../ew/external/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <iostream>

using namespace std;

namespace ShaderFile
{
	class Cubemap
	{
	public: 
		Cubemap(std::vector<std::string> texture_faces);
		~Cubemap();
		unsigned int GetID();
	private: 
		unsigned int m_id; // gl texture handle
	};
}

#endif // !CUBEMAP_H
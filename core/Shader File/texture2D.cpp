#include "texture2d.h"
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

namespace shaderFile
{
	shaderFile::Texture2D::Texture2D(const char* filePath, int filterMode, int warpMode)
	{
		int width, height, nrChannels; 
		/**data = stbi_load(filePath, &width, &height, &nrChannels, 0);*/
	}

	Texture2D::~Texture2D()
	{
	}

	/*Texture2D::voidBind(unsigned int slot)
	{
	}*/


}

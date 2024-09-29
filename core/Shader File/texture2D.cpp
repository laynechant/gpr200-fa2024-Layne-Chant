#include "texture2d.h"
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

namespace shaderFile
{
	shaderFile::Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode)
	{
		
		
		float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
		if (data) {

				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			
			
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			std::cout << "Failed to load texture" << std::endl;
		}
		
		

		m_width = width;
		m_height = height;
		m_id = texture;

		stbi_image_free(data);
	}

	Texture2D::~Texture2D()
	{
	}

	unsigned int Texture2D::GetID()
	{
		return m_id;
	}

	void Texture2D::Bind(unsigned int slot)
	{
		glGenTextures(1, &m_id);
		glBindTexture(slot, m_id);
	}


}

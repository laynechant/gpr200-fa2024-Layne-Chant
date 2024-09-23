#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace shaderFile
{
	class Texture2D 
	{
		public: 
			Texture2D(const char* filePath, int filterMode, int warpMode);
			~Texture2D();
		//	voidBind(unsigned int slot = 0);
		// Bind to a specific texture unit
		private: 
			unsigned int m_id; // GL texture handle
			unsigned char* data; 
			int m_width, m_height; 
	};
}

#endif
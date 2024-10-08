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
			Texture2D(const char* filePath, int filterMode, int wrapMode, bool transparent = true);
			~Texture2D();
			unsigned int GetID();
			void Bind(unsigned int slot = 0);
		private: 
			unsigned int m_id; // GL texture handle 
			int m_width, m_height; 
	};
}

#endif
#include "cubemap.h"

#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

namespace ShaderFile
{
    Cubemap::Cubemap(vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        int width, height, nrChannels; 
        stbi_set_flip_vertically_on_load(false);
        for (unsigned int i = 0; i < faces.size(); i++)
        {
           unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
           if (data)
           {
               glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
               stbi_image_free(data);

           }
           else
           {
               cout << "Cubemap tex failed to load at path: " << faces[i] << endl;
               stbi_image_free(data);
           }
            
        }
        stbi_set_flip_vertically_on_load(true);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        m_id = textureID; 
    }

    Cubemap::~Cubemap() {};

    unsigned int Cubemap::GetID()
    {
        return m_id;
    }
}
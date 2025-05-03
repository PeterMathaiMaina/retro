#include "../third_party/imageprocessing/stb_image.h"
#include <GL/glew.h>
#include <iostream>
#include "textureLoader.hpp"
#include <filesystem>

unsigned int TextureFromFile(const char *path, const std::string &directory)
{
    std::filesystem::path texPath(path);
    std::string fullPath;

    if (texPath.is_absolute()) {
        fullPath = texPath.string();
    } else {
        fullPath = (std::filesystem::path(directory) / texPath).string();
    }

    std::cout << "Loading texture from: " << fullPath << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        else
            format = GL_RGB; // Default fallback

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    else
    {
        std::cerr << "Texture failed to load at path: " << fullPath << std::endl;
        stbi_image_free(data);
        return 0;
    }


    return textureID;
}

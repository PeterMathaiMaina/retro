#include "../third_party/imageprocessing/stb_image.h"
#include "../third_party/imageprocessing/gli/gli.hpp"
#include "../third_party/imageprocessing/gli/gl.hpp"

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include "textureLoader.hpp"

unsigned int TextureFromFile(const char* path, const std::string& directory)
{
    std::filesystem::path texPath(path);
    std::string fullPath = texPath.is_absolute() ? texPath.string() : (std::filesystem::path(directory) / texPath).string();

    std::cout << "Loading texture from: " << fullPath << std::endl;

    std::string ext = texPath.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".dds" || ext == ".ktx" || ext == ".kmg" || ext == ".basis") {
        return LoadCompressedTexture(fullPath);
    } else {
        return LoadTextureWithSTB(fullPath);
    }
}

unsigned int LoadTextureWithSTB(const std::string& fullPath)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);

    if (!data) {
        std::cerr << "Failed to load image using stb: " << fullPath << std::endl;
        return 0;
    }

    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    else
        format = GL_RGB;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

unsigned int LoadCompressedTexture(const std::string& path)
{
    gli::texture tex = gli::load(path);
    if (tex.empty()) {
        std::cerr << "Failed to load compressed texture: " << path << std::endl;
        return 0;
    }

    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format format = GL.translate(tex.format(), tex.swizzles());
    GLenum target = GL.translate(tex.target());

    if (target != GL_TEXTURE_2D) {
        std::cerr << "Unsupported texture target (only GL_TEXTURE_2D is supported currently)." << std::endl;
        return 0;
    }

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(target, texID);

    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glm::tvec3<GLsizei> extent(tex.extent());
    GLsizei levels = static_cast<GLsizei>(tex.levels());

    for (std::size_t level = 0; level < tex.levels(); ++level) {
        GLsizei width = std::max(1, extent.x >> level);
        GLsizei height = std::max(1, extent.y >> level);

        if (gli::is_compressed(tex.format())) {
            glCompressedTexImage2D(
                target,
                static_cast<GLint>(level),
                format.Internal,
                width, height, 0,
                static_cast<GLsizei>(tex.size(level)),
                tex.data(0, 0, level)
            );
        } else {
            glTexImage2D(
                target,
                static_cast<GLint>(level),
                format.Internal,
                width, height, 0,
                format.External, format.Type,
                tex.data(0, 0, level)
            );
        }
    }

    glBindTexture(target, 0);
    return texID;
}

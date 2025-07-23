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
    std::string fullPath = texPath.is_absolute() 
        ? texPath.generic_string() 
        : (std::filesystem::path(directory) / texPath).generic_string();

    std::cout << fullPath << std::endl;


    std::string ext = texPath.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    if (ext == ".dds" || ext == ".ktx") {
        gli::texture tex = gli::load(fullPath);
        if (tex.target() == gli::TARGET_CUBE) {
            return loadKTXCubemap(fullPath);
        } else {
            return LoadCompressedTexture(fullPath); // For 2D compressed textures
        }
    }
    else{
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}

GLuint LoadCompressedTexture(const std::string& path) {
    gli::texture texture = gli::load(path);
    if (texture.empty()) {
        std::cerr << "Failed to load Compressed texture: " << path << std::endl;
        return 0;
    }

    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format format = GL.translate(texture.format(), texture.swizzles());

    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    

    glTexStorage2D(GL_TEXTURE_2D, static_cast<GLint>(texture.levels()),
                   format.Internal,
                   static_cast<GLsizei>(texture.extent().x),
                   static_cast<GLsizei>(texture.extent().y));

    for (std::size_t level = 0; level < texture.levels(); ++level) {
        glCompressedTexSubImage2D(
            GL_TEXTURE_2D,
            static_cast<GLint>(level),
            0, 0,
            static_cast<GLsizei>(texture.extent(level).x),
            static_cast<GLsizei>(texture.extent(level).y),
            format.Internal,
            static_cast<GLsizei>(texture.size(level)),
            texture.data(0, 0, level)
        );
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    return texID;
}
unsigned int loadKTXCubemap(const std::string& ktxPath) {
    gli::texture tex = gli::load(ktxPath);
    if (tex.empty()) {
        std::cerr << "Failed to load KTX cubemap: " << ktxPath << std::endl;
        return 0;
    }

    if (tex.target() != gli::TARGET_CUBE || tex.faces() != 6 || tex.layers() != 1) {
        std::cerr << "Error: Not a valid cubemap texture!" << std::endl;
        return 0;
    }

    gli::texture_cube texCube(tex);
    gli::gl GL(gli::gl::PROFILE_GL33);
    gli::gl::format formatInfo = GL.translate(tex.format(), tex.swizzles());

    GLenum internalFormat = formatInfo.Internal;
    GLenum externalFormat = formatInfo.External;
    GLenum type = formatInfo.Type;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (int face = 0; face < 6; ++face) {
        for (std::size_t level = 0; level < texCube.levels(); ++level) {
            glm::tvec2<GLsizei> extent = texCube.extent(level);
            const void* data = texCube.data(0, face, level);
            GLsizei size = static_cast<GLsizei>(texCube.size(level));

            if (gli::is_compressed(tex.format())) {
                glCompressedTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    static_cast<GLint>(level),
                    internalFormat,
                    extent.x, extent.y,
                    0,
                    size,
                    data
                );
            } else {
                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                    static_cast<GLint>(level),
                    internalFormat,
                    extent.x, extent.y,
                    0,
                    externalFormat,
                    type,
                    data
                );
            }
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    if (texCube.levels() == 1) {
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    std::cout << ktxPath << '\n';
    return textureID;
}

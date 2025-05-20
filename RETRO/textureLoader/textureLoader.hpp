#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <string>
#include <GL/glew.h>

unsigned int TextureFromFile(const char* path, const std::string& directory);
unsigned int LoadCompressedTexture(const std::string& path);
unsigned int LoadTextureWithSTB(const std::string& fullPath);

#endif
#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <string>
#include <GL/glew.h>
#include <vector>

unsigned int TextureFromFile(const char* path, const std::string& directory);
unsigned int LoadCompressedTexture(const std::string& path);
unsigned int LoadTextureWithSTB(const std::string& fullPath);
unsigned int loadKTXCubemap(const std::string& ktxPath) ;
unsigned int LoadHeightMap(const std::string& HeightMapPath) ;


#endif
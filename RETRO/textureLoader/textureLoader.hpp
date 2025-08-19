#ifndef TEXTURE_LOADER_HPP
#define TEXTURE_LOADER_HPP

#include <string>
#include <GL/glew.h>
#include <vector>

// struct CPUTexture {
//     int width, height, channels;
//     std::vector<unsigned char> pixels;    
// };



unsigned int TextureFromFile(const std::string& fullPath);
unsigned int LoadCompressedTexture(const std::string& path);
unsigned int LoadTextureWithSTB(const std::string& fullPath);
unsigned int loadKTXCubemap(const std::string& ktxPath) ;
unsigned int LoadHeightMapTexture(const std::string& HeightMapPath) ;
// CPUTexture LoadtextureinCPU(const std::string& path);

#endif
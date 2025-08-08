#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include "../../third_party/imageprocessing/stb_image.h"
#include "../Shader/ShaderSetup.h"
#include "GlobalDef.h"

struct HeightMap {
    // CPU-side
    int width = 0, height = 0;            // original image width/height
    int texWidth = 0, texHeight = 0;      // downsampled texture width/height (after step)
    std::vector<float> heightValues;      // float heights (size = texWidth * texHeight)

    // geometry for rendering (interleaved: x, y, z, u, v)
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // GPU resources (created only after uploadToGPU())
    unsigned int VAO = 0, VBO = 0, EBO = 0;
    unsigned int heightTexture = 0;
    bool gpuUploaded = false;

    // Load only on CPU — safe to call before creating GL context / GLEW init
    HeightMap(const std::string& path, float maxHeight, int step = 4) {
        LoadHeightMap(path, maxHeight, step);
        if (vertices.empty() || indices.empty()) {
            std::cerr << "HeightMap: warning — no geometry created from " << path << '\n';
        }
        // IMPORTANT: do NOT call setupHeightMap() or any GL function here.
        // Call uploadToGPU() after you have created the GL context and initialized GLEW.
    }

    // CPU-only loader. 'step' controls downsampling (stride).
    void LoadHeightMap(const std::string& path, float maxHeight, int step = 4) {
        int nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 1); // force grayscale
        if (!data) {
            std::cerr << "Failed to load heightmap: " << path << std::endl;
            return;
        }

        std::cout << "Heightmap loaded: " << path << " (" << width << "x" << height << ", channels: " << nrChannels << ")\n";

        // Build downsampled grid
        texWidth = width / step;
        texHeight = height / step;
        if (texWidth <= 0) texWidth = 1;
        if (texHeight <= 0) texHeight = 1;

        heightValues.clear();
        heightValues.reserve(texWidth * texHeight);

        // Build vertices (positions + u,v) and heightValues in same sampling (step)
        vertices.clear();
        indices.clear();

        for (int z = 0; z < height; z += step) {
            for (int x = 0; x < width; x += step) {
                int sampleIdx = z * width + x;
                float y = (data[sampleIdx] / 255.0f) * maxHeight - 0.3f;

                // Save vertex: x, y, z, u, v
                vertices.push_back((float)x);
                vertices.push_back(y);
                vertices.push_back((float)z);

                float u = (float)x / (float)(width - 1);
                float v = (float)z / (float)(height - 1);
                vertices.push_back(u);
                vertices.push_back(v);

                // Save height value for texture (in same sampling order)
                heightValues.push_back(y);
            }
        }

        int numCols = texWidth;
        int numRows = texHeight;

        // Build indices for the downsampled grid
        for (int z = 0; z < numRows - 1; ++z) {
            for (int x = 0; x < numCols - 1; ++x) {
                int topLeft     = z * numCols + x;
                int topRight    = topLeft + 1;
                int bottomLeft  = (z + 1) * numCols + x;
                int bottomRight = bottomLeft + 1;

                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }

        stbi_image_free(data);
    }

    // Call this AFTER GLFW window/context creation and glewInit()
    // It creates VAO/VBO/EBO and uploads the height texture to GPU.
    void uploadToGPU() {
        if (gpuUploaded) return; // already done

        // Create and fill VAO/VBO/EBO from CPU data
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // layout: 0 -> position (vec3), 1 -> texcoord (vec2) ; stride = 5 floats
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Create height texture (R32F)
        glGenTextures(1, &heightTexture);
        glBindTexture(GL_TEXTURE_2D, heightTexture);

        // Ensure heightValues is the right size
        if (heightValues.size() != (size_t)texWidth * (size_t)texHeight) {
            std::cerr << "HeightMap::uploadToGPU: heightValues size mismatch (expected "
                      << texWidth * texHeight << ", got " << heightValues.size() << ")\n";
        }

        // Upload as floats (GL_FLOAT). heightValues.data() -> const void*
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, texWidth, texHeight, 0, GL_RED, GL_FLOAT, heightValues.data());

        // basic sampling params — tweak as needed
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        gpuUploaded = true;
    }

    // Draw uses VAO and shader; assumes shader knows how to sample/use the height texture if needed.
    void Draw(Shader& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const {
        if (!gpuUploaded) {
            std::cerr << "HeightMap::Draw called before uploadToGPU()\n";
            return;
        }

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, heightTexture);
        shader.setInt("heightMapTexture", 0); // make sure shader uses the same uniform name

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Accessors (const)
    const std::vector<float>& GetVertices() const { return vertices; }
    const std::vector<unsigned int>& GetIndices() const { return indices; }
    unsigned int GetHeightMapVAO() const { return VAO; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetTexWidth() const { return texWidth; }
    int GetTexHeight() const { return texHeight; }

    // Clean up GL resources only if they were created (and assume GL context is current)
    ~HeightMap() {
        if (gpuUploaded) {
            if (heightTexture) glDeleteTextures(1, &heightTexture);
            if (VAO) glDeleteVertexArrays(1, &VAO);
            if (VBO) glDeleteBuffers(1, &VBO);
            if (EBO) glDeleteBuffers(1, &EBO);
        }
    }
};

#endif

#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP

#include <GL/glew.h>
#include <vector>
#include <string>
#include <iostream>
#include "../../third_party/imageprocessing/stb_image.h"
#include "../Shader/ShaderSetup.h"

struct HeightMap {
    int width, height;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    HeightMap(const std::string& path, float maxHeight) {
        LoadHeightMap(path, maxHeight);
        if (!vertices.empty() && !indices.empty()) {
            setupHeightMap();
        } else {
            std::cerr << "Failed to initialize heightmap geometry.\n";
        }
    }

    void LoadHeightMap(const std::string& path, float maxHeight) {
        int nrChannels;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 1); // Force grayscale

        if (!data) {
            std::cerr << "❌ Failed to load heightmap: " << path << std::endl;
            return;
        }

        std::cout << "Heightmap loaded: " << path << " (" << width << "x" << height << ", channels: " << nrChannels << ")\n";
        int step = 4;

        for (int z = 0; z < height; z += step) {
            for (int x = 0; x < width; x += step) {
                float y = data[z * width + x] / 255.0f * maxHeight;

                
                vertices.push_back((float)x);
                vertices.push_back(y);
                vertices.push_back((float)z);

                float u = (float)x / (width - 1);  
                float v = (float)z / (height - 1);  
                vertices.push_back(u);
                vertices.push_back(v);
            }
        }


        int numCols = width / step;
        int numRows = height / step;

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

    void Draw(Shader& shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) const {
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    const std::vector<float>& GetVertices() const {
        return vertices;
    }

    const std::vector<unsigned int>& GetIndices() const {
        return indices;
    }

    unsigned int GetHeightMapVAO() const {
        return VAO;
    }

    ~HeightMap() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    unsigned int VAO = 0, VBO = 0, EBO = 0;

    void setupHeightMap() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);



        glBindVertexArray(0);
    }
};

#endif 

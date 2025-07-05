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

        std::cout << "✅ Heightmap loaded: " << path << " (" << width << "x" << height << ", channels: " << nrChannels << ")\n";

        // Generate vertices
        for (int z = 0; z < height; ++z) {
            for (int x = 0; x < width; ++x) {
                float y = data[z * width + x] / 255.0f * maxHeight;
                vertices.push_back((float)x);
                vertices.push_back(y);
                vertices.push_back((float)z);
            }
        }

        // Generate indices
        for (int z = 0; z < height - 1; ++z) {
            for (int x = 0; x < width - 1; ++x) {
                int topLeft     = z * width + x;
                int topRight    = topLeft + 1;
                int bottomLeft  = (z + 1) * width + x;
                int bottomRight = bottomLeft + 1;

                // First triangle
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);

                // Second triangle
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

        // Vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        // Index buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Vertex attribute: position (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
    }
};

#endif // HEIGHTMAP_HPP

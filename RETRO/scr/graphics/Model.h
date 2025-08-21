#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "../core/Shader.hpp"
#include "Mesh.hpp"
#include "../../textureLoader/textureLoader.hpp"

class Model 
{
public:
    // model data 
    std::vector<Texture> textures_loaded; 
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;

    // Constructors
    Model() = default;
    Model(const std::string& path, bool gamma = false);


    // Rendering
    void Draw(Shader &shader);

    // Transformations
    void Translate(Shader &shader, glm::mat4 &model, glm::vec3 position);
    void RotateX(Shader& shader, glm::mat4& model, float angle);
    void RotateY(Shader& shader, glm::mat4& model, float angle);
    void RotateZ(Shader& shader, glm::mat4& model, float angle);
    void Scale(Shader& shader, glm::mat4 model, float scaleFactor);

private:
    // Internal loading
    void loadModel(const std::string& path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif

#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>
#include <string>
#include <iostream>
#include "../core/Shader.hpp"
#include "Mesh.hpp"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "../../third_party/imageprocessing/stb_image.h" 
#include "../../textureLoader/textureLoader.hpp"


class Model 
{
public:
    // model data 
    vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    meshes;
    string directory;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader &shader)
    {
        shader.use();
        for(unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }
    void Translate(Shader &shader,glm::mat4 &Model,glm::vec3 Position)
    {
            Model= glm::translate(Model , Position);
            shader.setMat4("u_Model",Model);
        
    }
    void RotateX(Shader& shader, glm::mat4& model, float angle) {
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("u_Model", model);

    }

    void RotateY(Shader& shader, glm::mat4& model, float angle) {
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("u_Model", model);
    }  

    void RotateZ(Shader& shader, glm::mat4& model, float angle) {
        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("u_Model", model);
    }
    void Scale(Shader& shader,glm::mat4 model,float ScaleFactor){
        model = glm::scale(model,glm::vec3(ScaleFactor));
        shader.setMat4("u_Model",model);
    }
private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const &path)
    {
        // read file via ASSIMP
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
            return;
        }
        // retrieve the directory path of the filepath
        directory = path.substr(0, path.find_last_of('/'));

        // process ASSIMP's root node recursively
        processNode(scene->mRootNode, scene);
        std::cout <<  path <<std::endl;
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene)
    {
        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene)
    {
        // data to fill
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;
       // vector<Mesh> meshes{processMesh(scene->mMeshes,scene}: 

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x; 
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
            
            if (mesh->mMaterialIndex >= scene->mNumMaterials || !scene->mMaterials[mesh->mMaterialIndex]) {
                std::cerr << "Invalid material index: " << mesh->mMaterialIndex << std::endl;
                return Mesh(vertices, indices, textures); // return with no textures
            }
            }
            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for(unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);        
            }
            // process materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];    


            // 1. diffuse  maps
            
            // 1. Base Color / Diffuse
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "texture_diffuse");
            if (diffuseMaps.empty()) {
                // fallback for older formats (like OBJ/FBX)
                diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            }
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            
            // 2. Specular (legacy) — not used in PBR, but supported
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            
            // 3. Normal Map
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMAL_CAMERA, "texture_normal");
            //if (normalMaps.empty()) {
            //    // fallback (legacy)
            //    normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
            //}
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            
            // 4. Metallic Map (PBR)
            std::vector<Texture> metallicMaps = loadMaterialTextures(material, aiTextureType_METALNESS, "texture_metallic");
            textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
            
            // 5. Roughness Map (PBR)
            std::vector<Texture> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
            textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
            
            // 6. Ambient Occlusion
            std::vector<Texture> aoMaps = loadMaterialTextures(material, aiTextureType_LIGHTMAP, "texture_ao");
            textures.insert(textures.end(), aoMaps.begin(), aoMaps.end());
            
            // 7. Emissive Map
            std::vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
            textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());

            // return a mesh object created from the extracted mesh data
            return Mesh(vertices, indices, textures);
        }

        // checks all material textures of a given type and loads the textures if they're not loaded yet.
        // the required info is returned as a Texture struct.
        vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) 
        {
            vector<Texture> textures;
        
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
                aiString str;
                mat->GetTexture(type, i, &str);
            
                // Check if texture was loaded before to avoid duplication
                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                        textures.push_back(textures_loaded[j]);
                        skip = true;
                        break;
                    }
                }
            
                if (!skip) {
                    Texture texture;
                    string fullPath = directory + '/' + string(str.C_Str());
                    texture.id = TextureFromFile(fullPath.c_str(),directory);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture); // store it to avoid reloading
                }
            }
            return textures;
        }
        
        
};  

#endif

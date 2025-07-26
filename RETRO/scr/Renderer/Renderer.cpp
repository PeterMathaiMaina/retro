// #include "Renderer.h"




// glm::mat4 GetRotationAroundYPoint(glm::vec3 pivotPoint, float radius, float speed, float scale) {
//     float time = glfwGetTime();
//     float angle = time * speed;
//     glm::mat4 model = glm::mat4(1.0f);
//     model = glm::translate(model, pivotPoint);
//     model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
//     model = glm::translate(model, glm::vec3(radius, 0.0f, 0.0f));
//     model = glm::scale(model, glm::vec3(scale));
//     return model;
// }

// std::vector<glm::vec3> CalculateTangents(const float* vertices, size_t vertexCount) {
//     std::vector<glm::vec3> tangents(vertexCount);
//     for (size_t i = 0; i < vertexCount; i += 3) {
//         glm::vec3 pos1(vertices[i * 8 + 0], vertices[i * 8 + 1], vertices[i * 8 + 2]);
//         glm::vec2 uv1(vertices[i * 8 + 3], vertices[i * 8 + 4]);

//         glm::vec3 pos2(vertices[(i + 1) * 8 + 0], vertices[(i + 1) * 8 + 1], vertices[(i + 1) * 8 + 2]);
//         glm::vec2 uv2(vertices[(i + 1) * 8 + 3], vertices[(i + 1) * 8 + 4]);

//         glm::vec3 pos3(vertices[(i + 2) * 8 + 0], vertices[(i + 2) * 8 + 1], vertices[(i + 2) * 8 + 2]);
//         glm::vec2 uv3(vertices[(i + 2) * 8 + 3], vertices[(i + 2) * 8 + 4]);

//         glm::vec3 edge1 = pos2 - pos1;
//         glm::vec3 edge2 = pos3 - pos1;
//         glm::vec2 deltaUV1 = uv2 - uv1;
//         glm::vec2 deltaUV2 = uv3 - uv1;

//         float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
//         glm::vec3 tangent;
//         tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
//         tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
//         tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
//         tangent = glm::normalize(tangent);

//         tangents[i + 0] = tangent;
//         tangents[i + 1] = tangent;
//         tangents[i + 2] = tangent;
//     }
//     return tangents;
// }

// float cubeVertices[] = 
// {
//     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
//     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
//     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
//     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
//     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
//     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
//     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
//     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
//     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
//     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
//     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
// };

// void RETRO_RENDERER::RenderScene(GLFWwindow* window,float deltaTime)
// {
//         glEnable(GL_DEPTH_TEST);
//         glDepthFunc(GL_LESS);
//         glDepthMask(GL_TRUE);
//         glEnable(GL_MULTISAMPLE);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         glm::mat4 view = camera.GetViewMatrix();

//         TreeShader.use();
//         TreeShader.setMat4("projection", projectionMatrix);
//         TreeShader.setMat4("view", view);

//         for (unsigned int i = 0; i < 3; i++)
//         {
//             glm::mat4 treeMat = glm::translate(glm::mat4(1.0f), lightPositions[i]);
//             TreeShader.setMat4("model", treeMat);
//             Tree.Draw(TreeShader);
//         }

//         BunnyShader.use();
//         BunnyShader.setMat4("view", view);
//         BunnyShader.setvec3("viewPos", camera.Position);
//         Bunny.Draw(BunnyShader);

//         HouseShader.use();
//         HouseShader.setMat4("view", view);
//         HouseShader.setvec3("viewpos", camera.Position);
//         setSpotLight(HouseShader, "spotlight", camera.Position, camera.Front,
//                     glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f),
//                     1.0f, 0.09f, 0.032f,
//                     glm::cos(glm::radians(15.5f)), glm::cos(glm::radians(20.5f)),
//                     FlashLight::flashlightOn);
//         House.Draw(HouseShader);

//         glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//         shadowscene.Draw(HeightMapShader, glm::mat4(1.0f), view, projectionMatrix);
//         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
// }

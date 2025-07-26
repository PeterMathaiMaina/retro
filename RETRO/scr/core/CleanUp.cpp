#include <GL/glew.h> // or glew.h depending on your setup
#include "CleanUp.h"

namespace GL_cleanUp {
    void cleanup_array_buffer(unsigned int& VAO, unsigned int& VBO) {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
}

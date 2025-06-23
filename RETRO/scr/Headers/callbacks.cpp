#include "Callbacks.h" // Make sure this is clean now

// Global or static variables are often used for simplicity with callbacks,
// but passing data via glfwSetWindowUserPointer is generally better for OOP.
// Let's go with glfwSetWindowUserPointer.

// Define your callback functions globally or as static class members
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Retrieve the Camera object from the window's user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    // You might also need to keep track of the last mouse position
    // if your ProcessMouseMovement expects a delta.
    // This example assumes Camera::ProcessMouseMovement handles current x,y.
    // If it expects delta, you'd need to store previous_x, previous_y globally/statically
    // or within the Camera class. For simplicity, let's assume it handles raw position
    // or you'll adapt it to calculate delta internally.
    if (camera) { // Always good to check if the pointer is valid
        // You might need to manage first_mouse and last_x, last_y state
        // This logic is often part of the Camera class or an InputManager
        // For now, let's assume ProcessMouseMovement takes xpos, ypos directly.
        // If your ProcessMouseMovement takes xoffset, yoffset:
        // You'll need static variables here for lastX, lastY to calculate offset.
        // Or, more commonly, handle this state directly in the Camera class itself.

        // Example for delta calculation (if needed by your ProcessMouseMovement)
        static bool firstMouse = true;
        static float lastX = 0.0f;
        static float lastY = 0.0f;

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Reversed since Y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Retrieve the Camera object from the window's user pointer
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera) {
        camera->ProcessMouseScroll(yoffset);
    }
}

// The setupcallbacks function should now take the GLFWwindow* and Camera*
// so it can set the user pointer.
void setupcallbacks(GLFWwindow* window, Camera* camera) {
    // Store the camera pointer in the window's user data
    glfwSetWindowUserPointer(window, camera);

    // Set the actual callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}
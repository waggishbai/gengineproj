#include <gengine/gengine.hpp>

#include <iostream>

namespace gnj {

    //----------------------window---------------------
    void Window::framebuffer_size_callback(GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    }

    Window::Window(int w, int h, const char* title) {

        if (!glfwInit()) {
            std::cerr << "\033[31mglfw error: glfw failed to initialize\033[0m\n";
        }

        isOpen = true;

        width = w;
        height = h;

        handle = glfwCreateWindow(w, h, title, NULL, NULL);
        if (handle == 0) {
            std::cout << "\033[31mglfw error: failed to create window\033[0m\n";
        }
        glfwMakeContextCurrent(handle);

        glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);

        gladLoadGL();
    }

    Window::~Window() {
        glfwTerminate();
        std::cout << "terminated";
    }

    void Window::Update() {
        glfwPollEvents();
        glfwSwapBuffers(handle);

        isOpen = !glfwWindowShouldClose(handle);
    }
    //----------------------window---------------------
}

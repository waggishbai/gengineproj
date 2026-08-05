#include <gengine/gengine.hpp>

#include <iostream>

namespace gnj {

    bool init() {
        bool success;
        success = true;
        if (!glfwInit()) {
            success = false;
            std::cerr << "\033[31mglfw error: glfw failed to initialize\033[0m\n";
        }
        return success;
    }

    Monitor getPrimaryMonitor() {
        Monitor monitor;

        GLFWmonitor* handle = glfwGetPrimaryMonitor();
        if (handle == NULL) {
            std::cerr << " \033[31mglfw error: could not get monitor, possibly glfw is not initialized, run gnj::init() before calling gnj functions.\033[0m\n";
        }
        monitor.handle = handle;

        const GLFWvidmode* mode = glfwGetVideoMode(monitor.handle);

        monitor.width = mode->width;
        monitor.height = mode->height;

        return monitor;
    }



    //----------------------window---------------------
    void Window::framebuffer_size_callback(GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    }

    Window::Window(int w, int h, const char* title) {

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

        glViewport(0, 0, w, h);
    }

    Window::Window(int w, int h, const char* title, GLFWmonitor* monitorHandle) {

        isOpen = true;

        width = w;
        height = h;


        handle = glfwCreateWindow(w, h, title, monitorHandle, NULL);
        if (handle == 0) {
            std::cout << "\033[31mglfw error: failed to create window\033[0m\n";
        }
        glfwMakeContextCurrent(handle);

        gladLoadGL();

        glViewport(0, 0, w, h);
    }

    Window::Window(Monitor mon, const char* title) {

        isOpen = true;

        width = mon.width;
        height = mon.height;

        handle = glfwCreateWindow(mon.width, mon.height, title, mon.handle, NULL);
        if (handle == 0) {
            std::cout << "\033[31mglfw error: failed to create window\033[0m\n";
        }
        glfwMakeContextCurrent(handle);

        gladLoadGL();

        glViewport(0, 0, width, height);
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

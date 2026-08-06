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

    //-------------------demo cube---------------------
    DemoCube::DemoCube() {
        const char* vs = "#version 330 core"
        "layout(location = 0) in vec3 aPos;"
        "layout(location = 1) in vec3 aNormal"
        "layout(location = 2) in vec2 aTexCoord"

        "out vec3 pos;"

        "uniform mat4 proj;"
        "uniform mat4 model"
        "uniform mat4 view"

        "void main () {"
        "   gl_Location = vec4(aPos, 1.0);"
        "   pos = aPos;"
        "}";
        const char* fs = "#version 330 core"
        "in vec3 pos;"
        "out vec4 out_color;"
        "void main {"
        "   out_color = vec4(normalize((pos + 1 * .5)));"
        "}";

        shaderProgram = compileShaderProgram(vs, fs);

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(0));
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    DemoCube::~DemoCube() {
        glDeleteProgram(shaderProgram);
    }
}

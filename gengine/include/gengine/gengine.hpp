#ifndef GENGINE_H
#define GENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>



namespace gnj {
    bool init();

    struct Monitor {
    public:
        int width;
        int height;

        GLFWmonitor* handle;
    };
    Monitor getPrimaryMonitor();

    class Window {
    private:
        static void framebuffer_size_callback(GLFWwindow*, int, int);
    public:
        GLFWwindow* handle;

        int width;
        int height;

        bool isOpen;

        bool fullscreen;


        Window(int, int, const char*);

        Window(int, int, const char*, GLFWmonitor*);

        Window(Monitor, const char*);

        ~Window();


        void Update();
    };

    class DemoCube {
    private:

        static constexpr float vertices[] =
        {1, 1, -1, 0, 1, 0, 0, 0, -1, 1, -1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, -1, 1, -1, 0, 1, 0, 0, 0, -1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, -1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, -1, -1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, -1, 1, 1, 0, 0, 1, 0, 1, -1, -1, 1, 0, 0, 1, 0, 1, -1, -1, 1, -1, 0, 0, 0, 0, -1, 1, 1, -1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, -1, 1, 1, -1, 0, 0, 0, 0, -1, 1, -1, -1, 0, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, -1, 0, -1, 0, 0, 0, 1, -1, -1, 0, -1, 0, 0, 0, -1, -1, 1, 0, -1, 0, 0, 0, 1, -1, -1, 0, -1, 0, 0, 0, 1, -1, 1, 0, -1, 0, 0, 0, -1, -1, 1, 0, -1, 0, 0, 0, 1, -1, -1, 1, 0, 0, 0, 0, 1, 1, -1, 1, 0, 0, 0, 0, 1, -1, 1, 1, 0, 0, 0, 0, 1, 1, -1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, -1, 1, 1, 0, 0, 0, 0, -1, -1, -1, 0, 0, -1, 0, 0, -1, 1, -1, 0, 0, -1, 0, 0, 1, -1, -1, 0, 0, -1, 0, 0, -1, 1, -1, 0, 0, -1, 0, 0, 1, 1, -1, 0, 0, -1, 0, 0, 1, -1, -1, 0, 0, -1, 0, 0};

        GLuint vao;
        GLuint vbo;

        GLuint shaderProgram;

    public:

        DemoCube();

        ~DemoCube();
    };
};

#endif

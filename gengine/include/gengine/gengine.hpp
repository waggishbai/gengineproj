#ifndef GENGINE_H
#define GENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gnj {
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

        void Update();

        ~Window();
    };
};

#endif

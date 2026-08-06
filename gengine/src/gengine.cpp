#include <gengine/gengine.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

std::string getFileText(std::string filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint compileShader(const char* shaderText, GLenum type, bool errorCheck) {
    if (errorCheck) {
        std::cout << shaderText << std::endl;
    }
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderText, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success != GL_TRUE) {

        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        if(type == GL_VERTEX_SHADER) {
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        } else {
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

    }
    return shader;
}

GLuint compileProgram(GLuint vertexShader, GLuint fragmentShader) {

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return program;
}

//uniform setters

void mat4ToProgram(GLuint program, glm::mat4 mat, const char* varName) {

    GLint uVar = glGetUniformLocation(program, varName);
    glUniformMatrix4fv(uVar, 1, GL_FALSE, glm::value_ptr(mat));
}

void vec3ToProgram(GLuint program, glm::vec3 vec, const char* varName) {
    GLint uVar = glGetUniformLocation(program, varName);
    glUniform3f(uVar, vec.x, vec.y, vec.z);
}
GLuint compileShaderProgram(const char* vtext, const char* ftext) {
    GLuint vertex = compileShader(vtext, GL_VERTEX_SHADER, false);
    GLuint fragment = compileShader(ftext, GL_FRAGMENT_SHADER, false);

    GLuint outprog = compileProgram(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return outprog;
}

GLuint compileShaderProgram(std::string vfile, std::string ffile) {
    std::string vtext = getFileText(vfile);
    std::string ftext = getFileText(ffile);

    const char* v = vtext.c_str();
    const char* f = ftext.c_str();

    GLuint vertex = compileShader(v, GL_VERTEX_SHADER, false);
    GLuint fragment = compileShader(f, GL_FRAGMENT_SHADER, false);

    GLuint outprog = compileProgram(vertex, fragment);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return outprog;
}
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
        const char* vs = "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aNormal;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"

        "out vec3 pos;\n"

        "uniform mat4 proj;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"

        "void main () {\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   pos = aPos;\n"
        "}";
        const char* fs = "#version 330 core\n"
        "in vec3 pos;\n"
        "out vec4 out_color;\n"
        "void main() {\n"
        "   out_color = vec4(normalize((pos + 1 * .5)), 1.0);\n"
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
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}

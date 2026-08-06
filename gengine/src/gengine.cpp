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

        glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);
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

        glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);


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

        glEnable(GL_DEPTH_TEST);
        glClearColor(0, 0, 0, 1);
    }

    Window::~Window() {
        glfwDestroyWindow(handle);
        glfwTerminate();
        std::cout << "terminated\n";
    }

    void Window::Update() {
        glfwPollEvents();
        glfwSwapBuffers(handle);
        if (glfwWindowShouldClose(handle)) {
            isOpen = false;
        }
    }

    void Window::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    //----------------------window---------------------

    //-------------------demo cube---------------------
    DemoCube::DemoCube() {

        modelMat = glm::mat4(1);
        modelOrigin = glm::mat4(1);

        rotation = glm::quat();
        scale = glm::vec3(1, 1, 1);
        position = glm::vec3(0, 0, 0);

        std::cout << vertLen << std::endl;
        const char* vs = "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec3 aNormal;\n"
        "layout(location = 2) in vec2 aTexCoord;\n"

        "out vec3 pos;\n"

        "uniform mat4 proj;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"

        "void main () {\n"
        "   gl_Position = proj * view * model * vec4(aPos, 1.0);\n"
        "   pos = aPos;\n"
        "}";
        const char* fs = "#version 330 core\n"
        "in vec3 pos;\n"
        "out vec4 out_color;\n"
        "void main() {\n"
        "   out_color = vec4(normalize((pos + 2 * .2)), 1.0);\n"
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

    void DemoCube::setPosition(float x, float y, float z) {
        position = glm::vec3(x, y, z);
    }
    void DemoCube::setRotation(float x, float y, float z) {
        //warning, using this method can result in gimbal lock.

        glm::quat a = glm::angleAxis(glm::radians(x), glm::vec3(1, 0, 0));
        glm::quat b = glm::angleAxis(glm::radians(y), glm::vec3(0, 1, 0));
        glm::quat c = glm::angleAxis(glm::radians(z), glm::vec3(0, 0, 1));

        rotation = a * b * c;
    }
    void DemoCube::setScale(float x, float y, float z) {
        scale = glm::vec3(x, y, z);
    }

    void DemoCube::Update() {
        glm::mat4 trans = glm::translate(modelOrigin, position);
        glm::mat4 rot = glm::mat4_cast(rotation);
        glm::mat4 rotMat = modelOrigin * rot;
        glm::mat4 sca = glm::scale(modelOrigin, scale);

        modelMat =  trans * rot * sca;

        //modelMat *= glm::scale(modelOrigin, scale);
        //modelMat *= glm::toMat4(rotation) * modelOrigin;
        //modelMat = glm::translate(modelOrigin, position);
    }


    void DemoCube::testDraw() {

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glm::mat4 view = glm::lookAt(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
        glm::mat4 proj = glm::perspective(glm::radians(90.f), ((float)mode->width / (float)mode->height), .01f, 100.f);

        glBindVertexArray(vao);
        glUseProgram(shaderProgram);

        mat4ToProgram(shaderProgram, modelMat, "model");
        mat4ToProgram(shaderProgram, view, "view");
        mat4ToProgram(shaderProgram, proj, "proj");

        glDrawArrays(GL_TRIANGLES, 0, vertLen / 8);

        glUseProgram(0);
        glBindVertexArray(0);
    }

}

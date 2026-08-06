#include <gengine/gengine.hpp>
#include <iostream>

struct Keyboard {
    bool esc;
};

Keyboard keys;

void key_callback(GLFWwindow*, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        keys.esc = true;
    }
}

int main() {
    gnj::init();

    gnj::Window window(320, 180, "twt", gnj::getPrimaryMonitor().handle);

    gnj::DemoCube cube;

    glm::vec3 pos(1, 1, 1);
    glm::vec3 rot(0, 0, 0);

    cube.setScale(2, 2, 2);

    glfwSetKeyCallback(window.handle, key_callback);
    while (window.isOpen) {
        if (keys.esc) {
            window.isOpen = false;
            std::cout << "die\n";
        }
        //pos.x += .01f;
        rot.y += .1f;

        window.Clear();

        cube.setPosition(pos.x, pos.y, pos.z);
        cube.setRotation(rot.x, rot.y, rot.z);

        cube.Update();
        cube.testDraw();

        window.Update();
    }
}

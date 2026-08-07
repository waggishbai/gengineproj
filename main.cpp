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

    gnj::Window window(gnj::getPrimaryMonitor(), "twt");

    gnj::DemoCube cube;

    gnj::Camera cam(90.f, ((float)window.width / (float)window.height), 0.01f, 100.f);

    glm::vec3 pos(1, 1, 1);
    glm::vec3 rot(0, 0, 0);

    cube.setScale(2, 2, 2);

    cam.setPosition(2, 2, 10);
    cam.Update();

    glfwSetKeyCallback(window.handle, key_callback);
    while (window.isOpen) {
        if (keys.esc) {
            window.isOpen = false;
            std::cout << "die\n";
        }
        pos.z += .01f;
        //rot.y += .1f;

        window.Clear();

        cube.setPosition(pos.x, pos.y, pos.z);
        cube.setRotation(rot.x, rot.y, rot.z);

        cube.Draw(cam);

        cube.Update();


        window.Update();
    }
}

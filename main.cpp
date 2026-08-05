#include <gengine/gengine.hpp>

int main() {
    gnj::init();

    gnj::Window window(100, 100, "twt", gnj::getPrimaryMonitor().handle);

    while (window.isOpen) {
        window.Update();
    }
}

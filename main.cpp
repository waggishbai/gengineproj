#include <gengine/gengine.hpp>

int main() {
    gnj::init();

    gnj::Window window(gnj::getPrimaryMonitor(), "twt");

    while (window.isOpen) {
        window.Update();
    }
}

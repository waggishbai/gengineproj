#include <gengine/gengine.hpp>

int main() {
    gnj::Window window(100, 100, "twt");

    while (window.isOpen) {
        window.Update();
    }
}

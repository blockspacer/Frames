#ifndef FENGINE_H
#define FENGINE_H

#include <string>

class FEngine {
public:
    FEngine() {};

    void init(std::string title, unsigned int width = 1280, unsigned int height = 720);
    void start();
    void update();
    bool running();
    void cleanup();
};

#endif // FENGINE_H

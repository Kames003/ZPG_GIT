#include "Application.h"
#include <cstdlib>

int main()
{
    Application* app = new Application();
    
    app->initialization();
    app->createShaders();
    app->createModels();
    app->createScenes();
    app->run();
    
    delete app;
    return EXIT_SUCCESS;
}
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

#include "App.h"

int main()
{
    App app;

    try
    {
        app.run();
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
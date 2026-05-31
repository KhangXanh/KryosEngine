/**
 * @file Engine/Core/Entrypoint.hpp
 * This is a Kryos Engine Library header
 */

#pragma once

#include "Application.hpp"

/// @details 
/// We tell the compiler this function is in another place
/// So it won't throw an error
extern Kryos::Application *Kryos::CreateApplication();

int main(int argc, char **argv)
{
    auto app = Kryos::CreateApplication();
    app->Run();
    delete app;
}
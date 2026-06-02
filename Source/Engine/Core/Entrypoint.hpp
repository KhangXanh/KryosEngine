/**
 * @file Engine/Core/Entrypoint.hpp
 * This is a Kryos Engine Library header
 */


#pragma once

#include "Application.hpp"

extern Kryos::Application *Kryos::CreateApplication();

int main(int argc, char *argv[])
{
    auto Application = Kryos::CreateApplication();
    Application->Run();
    delete Application;
    return 0;
}
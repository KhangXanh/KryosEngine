/**
 * @file Studio/Layers/Launcher/LauncherLayer.cpp
 * This is a Kryos Application Implementation file
 */

#include "LauncherLayer.hpp"

namespace Kryos
{
    LauncherLayer::LauncherLayer(WindowManager *winMan)
    {
        mWinMan = winMan;
    }

    void LauncherLayer::OnAttach()
    {
        mWindowID = mWinMan->AddWindow(WindowProperties{"Kryos Launcher", 1280, 720});
    }

    void LauncherLayer::OnDetach()
    {
    }

    void LauncherLayer::OnUpdate(Float32 dt)
    {

    }

    void LauncherLayer::OnEvent(IEvent &event)
    {
    }
}
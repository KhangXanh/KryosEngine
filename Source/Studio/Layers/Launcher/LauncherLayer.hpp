/**
 * @file Studio/Layers/Launcher/LauncherLayer.hpp
 * This is a Kryos Application Header file
 */

#pragma once

#include "Core/Layer.hpp"
#include "Core/Window.hpp"

namespace Kryos
{
    /**
     * @brief
     * A layer for managing Launcher
     */
    class LauncherLayer : public ILayer
    {
    public:
        LauncherLayer(WindowManager *winMan);
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Float32 dt) override;
        virtual void OnEvent(IEvent& event) override;

        WindowID GetWindowID() { return mWindowID; }
    
    private:
        WindowManager *mWinMan;
        WindowID mWindowID;
    };
}
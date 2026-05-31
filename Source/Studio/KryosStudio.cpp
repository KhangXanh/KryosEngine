/**
 * @file Studio/KryosStudio.cpp
 * This is a Kryos Application Implementation file
 */

#define USE_CORE_LOG
#include <Engine/Kryos.hpp>

#include "Studio/Layers/Launcher/LauncherLayer.hpp"
#include "Studio/Layers/Editor/EditorLayer.hpp"

/**
 * @note
 * ```text
 * - This file is the application file
 * - This application is using Kryos Engine
 * ```
 */

namespace Kryos
{
    /**
     * @brief
     * An application made with Kryos Engine
     */
    class KryosStudio : public Application
    {
    public:
        KryosStudio() : Application()
        {
            GetLayerStack()->PushLayer(CreateUniquePtr<LauncherLayer>(GetWindowManager()));
            GetLayerStack()->PushLayer(CreateUniquePtr<EditorLayer>(GetWindowManager()));
            KS_CORE_INFO("Kryos Studio has been initialized.");
        }

        ~KryosStudio()
        {
            
        }

        virtual void pCustomOnUpdate(Float32 dt) override
        {
            
        }

        virtual void pCustomShutdown() override
        {
            KS_CORE_INFO("Kryos Studio has been closed.");
        }

        virtual void pCustomOnEvent(IEvent &event) override
        {

        }
    };

    Application *CreateApplication()
    {
        return new KryosStudio;
    }
}
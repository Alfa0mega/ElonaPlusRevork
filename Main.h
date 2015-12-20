//
// Created by AMEGA on 17-Oct-15.
//
#include "stdafx.h"
#include "SFML/SFMLWindow.h"
//=========================SFML2=============================
#include <sfml/Graphics.hpp>
#include <sfml/OpenGL.hpp>
#include <sfml/System.hpp>
#include <sfml/Window.hpp>
#include <sfml/Main.hpp>
//===========================================================
#include <thread>

#pragma once
#ifndef OPENGLAPP_MAIN_H
#define OPENGLAPP_MAIN_H
namespace AppCore {
    namespace WindowsManager {
        static AppCore::sfml_window* _mainWindow;
        //=========================SFML Windows=================
        static std::shared_ptr<AppCore::sfml_window> _window = nullptr;
        static std::shared_ptr<AppCore::sfml_window> _secondWindow = nullptr;
        //=====================================================
        void CreateSFWindows();
        void SetSFMainWindow(AppCore::sfml_window *window);
        void EnterAppLoop();
        bool isDirectory(const string& path);
    }
}
#endif //OPENGLAPP_MAIN_H

//
// Created by AMEGA on 01-Oct-15.
//

#pragma once

#include <stdio.h>
#include <shlwapi.h>
#include "../stdafx.h"
//=========================SFML2=============================
#include <sfml/Graphics.hpp>
#include <sfml/OpenGL.hpp>
#include <sfml/System.hpp>
#include <sfml/Window.hpp>
#include <sfml/Main.hpp>
//===========================================================
#include <thread>
#include <queue>

#ifndef OPENGLAPP_APPCORE_SFML_H
#define OPENGLAPP_APPCORE_SFML_H

namespace AppCore {
    class sfml_window {
    public:
        enum SFWindowEvents:unsigned short {
            SfHide,
            SfShow,
            SfClose
        };
        void CreateSFWindow();
        void SetWindowSize(std::uint16_t _width, std::uint16_t _heigth);
        void SetBitPerPixel(std::uint8_t bitsPerPixel);
        void SetWindowMode(const sf::VideoMode &_mode);
        void SetWindowTitle(const std::string &_title);
        void SetWindowTitle(const char *_title, uint16_t _length = 0);
        void EnterEventLoop();
        void RenderSFContent();
        void ShowSFWindow();
        void HideSFWindow();
        void CloseSFWindow();
        void CreateAndShow();
        bool SetAsMain(bool replaceExsMain = false);
        bool IsMain();
        static sfml_window *GetCurrentMain();
        static std::string InvalidTitle;
        static const std::uint8_t DefaultFramerate = 60;
        void FreeWID();
    private:
        void HandleCustomEvents();
        void HandleEvents();
        sf::RenderWindow _sfWindow;
        std::string _wTitle;
        sf::VideoMode _windowVMode;
        sf::Event _event;
        std::uint8_t _bitsPerPixel;
        bool _hasExplicitColor = false;
        bool _isMain = false;
        bool _isVisible = true;
        bool _isMakeVisibleFlagSet = false;
        bool _isMakeHiddenFlagSet = false;
        bool _isActive = true;
        bool _isMakeActiveFlagSet = false;
        static bool _isMainSet;
        static sfml_window* _currentMain;
        static sf::Color _defaultColor;
        static const std::uint8_t _startingWindowID = 1;
        static std::uint16_t _currentFreeID;
        std::uint16_t _windowID;
        static const std::uint8_t _mainWindowID = 0;
        std::queue<AppCore::sfml_window::SFWindowEvents> _customEventQueue;
    };
}

#endif //OPENGLAPP_APPCORE_SFML_H

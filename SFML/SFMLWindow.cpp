//
// Created by AMEGA on 01-Oct-15.
//

#include <omp.h>
#include "SFMLWindow.h"

sf::Color AppCore::sfml_window::_defaultColor = sf::Color(200, 200, 200, 0);
AppCore::sfml_window* AppCore::sfml_window::_currentMain = nullptr;
bool AppCore::sfml_window::_isMainSet = false;
std::string AppCore::sfml_window::InvalidTitle = "INVALID_TITLE";
std::uint16_t  AppCore::sfml_window::_currentFreeID = 1;

void AppCore::sfml_window::CreateSFWindow() {
    this->_sfWindow.create (
            this->_windowVMode,
            this->_wTitle,
            sf::Style::Default
    );
    this->_sfWindow.setFramerateLimit(this->DefaultFramerate);
}

void AppCore::sfml_window::SetWindowSize(std::uint16_t _width, std::uint16_t _heigth) {

}

void AppCore::sfml_window::SetWindowMode(const sf::VideoMode &_mode) {
    if (!_mode.isValid()) {
        fprintf(stderr, "EROR! VideoMode is invalid! Function: %s; line: %d",
                __FUNCTION__,
                __LINE__
        );
        std::exit(EXIT_FAILURE);
    }
  this->_windowVMode = _mode;
}

void AppCore::sfml_window::SetWindowTitle(const std::string &_title) {
    if (!_title.empty() || ! _title.compare(" "))
        this->_wTitle = std::string(_title);
    else {
        this->_wTitle = this->InvalidTitle;
    }
}

void AppCore::sfml_window::SetWindowTitle(const char *_title, uint16_t _length) {
    if (_length != 0)
    {
        _length = strlen(_title);
        if (_length == 0)
        {
            fprintf(stderr, "EROR! Window's title length is zero. Line: %d; function: %s; file: %s",
            __LINE__, __FUNCTION__, __FILE__);
        }
    }
    char* _tTitle = static_cast<char*>(calloc(_length, sizeof(_title[0])));
    memcpy(_tTitle, _title, _length * sizeof(_title[0]));
    this->_wTitle = std::string(_tTitle);
}

void AppCore::sfml_window::EnterEventLoop() {
    while (this->_sfWindow.isOpen()) {
        this->HandleEvents();
        this->HandleCustomEvents();
        this->_sfWindow.clear(sfml_window::_defaultColor);
        this->RenderSFContent();
        this->_sfWindow.display();
    }
}

void AppCore_sfml::sfml_window::RenderSFContent() {
}

void AppCore_sfml::sfml_window::ShowSFWindow() {
    this->_customEventQueue.push(SFWindowEvents::SfShow);
    this->_isMakeVisibleFlagSet = true;
}

void AppCore_sfml::sfml_window::HideSFWindow() {
    this->_customEventQueue.push(SFWindowEvents::SfHide);
    this->_isMakeHiddenFlagSet = true;
}

void AppCore_sfml::sfml_window::SetBitPerPixel(std::uint8_t bitsPerPixel) {
    if (bitsPerPixel == 0)
    {
        fprintf(stderr, "ERROR! Argument \"bitsPerPixel\" is zero. Function: %s; line: %s; file: %s", __FUNCTION__, __LINE__, __FILE__);
        std::exit(EXIT_FAILURE);
    }
    this->_bitsPerPixel = bitsPerPixel;
}

void AppCore_sfml::sfml_window::CreateAndShow() {
    this->CreateSFWindow();
    this->EnterEventLoop();
    if (this->IsMain())
    {
        std::exit(EXIT_SUCCESS);
    }
}

bool AppCore_sfml::sfml_window::IsMain() {
    if (AppCore_sfml::sfml_window::_currentMain == nullptr) {
        fprintf(stderr, "WARNING! The Main window doesn't been set yet. Function: %s; line: %d; file: %s",
                __FUNCTION__, __LINE__, __FILE__
        );
        return false;
    }
    return this->_isMain && (this == AppCore_sfml::sfml_window::_currentMain);
}

bool AppCore_sfml::sfml_window::SetAsMain(bool replaceExsMain){
    if (AppCore_sfml::sfml_window::_isMainSet && !replaceExsMain)
    {
        fprintf(stderr, "WARNING! Main window has been set already. Function: %s; line: %d; file: %s",
        __FUNCTION__, __LINE__, __FILE__);
        return false;
    }
    else
    {
        this->_isMain = true;
        AppCore_sfml::sfml_window::_currentMain = this;
        this->_isMainSet = true;
        this->_windowID = this->_mainWindowID;
        return true;
    }
}

AppCore_sfml::sfml_window *AppCore_sfml::sfml_window::GetCurrentMain()  {
    return AppCore::sfml_window::_currentMain;
}

void AppCore_sfml::sfml_window::HandleCustomEvents() {
    while (!this->_customEventQueue.empty()) {
        auto _customEvent = this->_customEventQueue.front();
        switch (_customEvent) {
            case SFWindowEvents::SfHide: {
                this->_isVisible = false;
                this->_sfWindow.setVisible(false);
                this->_isMakeHiddenFlagSet = false;
                break;
            }
            case SFWindowEvents::SfShow: {
                this->_isVisible = true;
                this->_sfWindow.setVisible(true);
                this->_isMakeVisibleFlagSet = false;
                break;
            }
            case SFWindowEvents::SfClose: {
                this->_sfWindow.close();
                break;
            }
        }
        this->_customEventQueue.pop();
    }
}


void AppCore_sfml::sfml_window::HandleEvents() {
    while (this->_sfWindow.pollEvent(this->_event)) {
        switch (this->_event.type) {
            case sf::Event::Closed: {
                this->CloseSFWindow();
                break;
            }
            case sf::Event::Resized: {
                break;
            }
            default: {
                break;
            }
        }
    }
}

void AppCore_sfml::sfml_window::CloseSFWindow() {
    if (this->IsMain()) {
        this->_customEventQueue.push(SFWindowEvents::SfClose);
        auto _texture = new sf::Texture();
    }
    else
        this->HideSFWindow();
}

void AppCore_sfml::sfml_window::FreeWID() {
    if (this->_windowID == this->_mainWindowID) {
        fprintf(stderr, "ERROR! Sorry, but you can't free ID of main window. File: %s; line: %d; function: %s\n",
                __FILE__, __LINE__, __FUNCTION__);
        exit(EXIT_FAILURE);
    }
    else {
        if (this->_currentFreeID < this->_currentFreeID)
            sfml_window::_currentFreeID = this->_windowID;
        this->CloseSFWindow();
    }
}

//
// Created by AMEGA on 17-Oct-15.
//

#include <unistd.h>
#include "Main.h"

void AppCore::WindowsManager::SetSFMainWindow(AppCore::sfml_window *window) {
    window->SetAsMain(true);
    AppCore::WindowsManager::_mainWindow = AppCore::sfml_window::GetCurrentMain();
}

/*!
*\brief Create all app's windows.
*/
void ::AppCore::WindowsManager::CreateSFWindows() {
    auto _VModeSecond = sf::VideoMode(1440, 900, sf::VideoMode::getDesktopMode().bitsPerPixel);
    AppCore::WindowsManager::_secondWindow = std::make_shared<AppCore::sfml_window>();
    AppCore::WindowsManager::_secondWindow->SetWindowMode(_VModeSecond);
    auto _SSecondTitle = std::string("Second Window");
    AppCore::WindowsManager::_secondWindow->SetWindowTitle(_SSecondTitle);
    auto _thread_second = std::thread {
            &AppCore::sfml_window::CreateAndShow,
            AppCore::WindowsManager::_secondWindow.get()
    };
    _thread_second.detach();

    auto _VMode = sf::VideoMode(1440, 900, sf::VideoMode::getDesktopMode().bitsPerPixel);
    AppCore::WindowsManager::_window = std::make_shared<AppCore::sfml_window>();
    AppCore::WindowsManager::_window->SetWindowMode(_VMode);
    auto _STitle = std::string("Main Window");
    AppCore::WindowsManager::_window->SetWindowTitle(_STitle);
    auto _thread = std::thread {
            &AppCore::sfml_window::CreateAndShow,
            AppCore::WindowsManager::_window.get()
    };
    _thread.detach();

    SetSFMainWindow(_window.get());
}

void ::AppCore_sfml::WindowsManager::EnterAppLoop() {
    //TODO: implement application logic. Currently does nothing.
    auto _pseudoThreadHandler = GetCurrentThread();

    SuspendThread(_pseudoThreadHandler);
}

bool ::AppCore_sfml::WindowsManager::isDirectory(const string& path) {
        bool _result = false;
        try {
            _result = boost::filesystem::is_directory(path);
        }
        catch (boost::filesystem::filesystem_error &error) {
            fprintf(stderr, "ERROR! Exception is occured! Function: %s; exception's message: %s; line: %d; file: %s",
                    __FUNCTION__, error.what(), __LINE__, __FILE__);
            std::exit(EXIT_FAILURE);
        }
        return _result;
    }
}

#include <thread>
#include <time.h>
#include <unistd.h>
#include "stdafx.h"
#ifndef _USE_SFML
    #include "AppCore.h"
#else
    #include "Main.h"
#endif

int main(int _Argc, char **_Argv, char **_Env) {
    #ifndef _DEBUG
    ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif
#ifndef _USE_SFML
    AppCore::Main->ProgramInitialization(_Argc, _Argv);
#else
    AppCore::WindowsManager::CreateSFWindows();
    AppCore::WindowsManager::EnterAppLoop();
#endif
    return EXIT_SUCCESS;
}

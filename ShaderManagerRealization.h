#pragma once

#include "ShaderWrapper.h"
#include "Utilities.h"
#include <iostream>
#include <vector>
#include <string>

using Utilities::DefaultShaderExt;

namespace ShaderManager {
    using WorkingDirectories = Utilities::Directories;

    static class ShaderManagerRealization {
    public:
        static ShaderManagerRealization *GetInstance();
        static const WorkingDirectories managerType = WorkingDirectories::shaders;
        void LoadAtRequest(const string &_shaderName);
        void LoadShadersParams();
        void LoadAllValidShaders();
        void UnloadAtRequest(const string &_shaderName);
        void UnloadAll();
        bool InitShaderManager();
        void ReLinkProgramm();
        void InstallProgramm();
    private:
        GLuint _shaderProgram;
        static ShaderManagerRealization *sInstance;
        static bool isAlreadyInitialized;
        static const string _shadersConfigName;
        std::vector<ShaderWrapper> _shadersList;
        std::vector<std::vector<string>> _ShadersText;
        std::vector<string> _shaderConfigString;
        std::vector<string> _shadersFullQulifier;
        void ReadShadersText(fstream &stream);
        ShaderManagerRealization() { };
        ShaderManagerRealization(const ShaderManagerRealization &_instance) { };
        ShaderManagerRealization &operator=(const ShaderManagerRealization &_instance) { return *this; };
        char *_linkLog = nullptr;
        GLint _linkLogLenght = 0;
        GLint _linkResult = 0;
        bool _isLoadShadersIsCalled = false;
    protected:
    } *ShdrManager = ShaderManagerRealization::GetInstance();
}

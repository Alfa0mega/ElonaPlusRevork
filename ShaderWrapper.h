#pragma once

#include "stdafx.h"
#include <shlwapi.h>
#include "Utilities.h"
#include <memory>
#include <vector>

using Utilities::ShaderType;
using Utilities::DefaultShaderExt;
using Utilities::GetTypeByExtension;

namespace ShaderManager {
    struct ShaderWrapper {
    public:
        static const string InvalidShaderPath;
        static const string InvalidShaderName;
        static bool isDefaultShaderExtension(const string &extension);
        void RedefineFlag(std::vector<string> &ShaderText, const string &shaderName, bool isForLoad, ShaderType _type,
                          const string &extension, const string &pathTo);
        void SetPathToShader(const string &path);
        void SetShaderType(ShaderType _type, bool _setDefShdrExt = false);
        void SetShaderType(ShaderType type, const string &_newExtension);
        void SetShaderExtension(const string &extension);
        void SetShaderName(const string &name);
        bool VerifyFlag(bool resetIfInvalid = false, bool printMessage = false);
        bool IsLoadable();
        bool IsLoaded();
        bool isDefaultShaderExtension();
        bool isFileExists();
        bool isInvalid();
        bool isForLoad();
        bool isUnloaded();
        string GetDefaultShaderExtension(ShaderType type);
        string GetPathToShader();
        ShaderType GetShaderType();
        string GetShaderExtension();
        std::vector<string> GetShaderText();
        const string &GetShaderName();
        void CompileShader();
        void DetachShader(GLuint _programm);
        void AttachShader(GLuint _programm);
        void SetShaderText(const std::vector<string> &ShaderText);
        void DeleteShader();
        void MarkForLoad();
        void MarkAsNotLoadable();
        const GLuint *const GetShaderID();
        ShaderWrapper(std::vector<string> &ShaderText, const string &shaderName = InvalidShaderName,
                      bool isForLoad = true, ShaderType _type = ShaderType::none,
                      const string &extension = DefaultShaderExt[0], const string &pathTo = InvalidShaderPath);
        inline ShaderWrapper() { BehaviorWhenTypeIsNone(); }
        ~ShaderWrapper();
    private:
        string _shaderExtension;
        string _pathTo;
        string _shaderName;
        ShaderType type = ShaderType::none;
        bool _isLoadable = false;
        bool _isLoaded = false;
        bool _isUnloaded = true;
        bool _isInvalid = true;
        char **_cTyleShaderText = nullptr;
        char *_log = nullptr;
        GLint _logLength = 0;
        GLuint _shaderTextLenght = 0;
        GLint _cmplResult = 0;
        GLuint _shaderID = 0;
        bool _isForLoad = false;
        void BehaviorWhenTypeIsNone(bool _message = false);
        bool isFileExists(const string &path);
        int _getNumericShaderType(ShaderType _type);
        void printShaderErrors();
    };
}

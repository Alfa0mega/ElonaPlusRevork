#define _CRT_SECURE_NO_WARNINGS

#include "ShaderWrapper.h"
#include <boost/filesystem.hpp>

const std::string ShaderManager::ShaderWrapper::InvalidShaderPath = "INVALID_PATH";
const string ShaderManager::ShaderWrapper::InvalidShaderName = "INVALID_NAME";

std::string ShaderManager::ShaderWrapper::GetDefaultShaderExtension(ShaderType _type) {
    return DefaultShaderExt[(unsigned char) _type];
}

bool ShaderManager::ShaderWrapper::IsLoadable() {
    return !_isInvalid && _isLoadable && VerifyFlag();
}

inline void ShaderManager::ShaderWrapper::BehaviorWhenTypeIsNone(bool _message) {
#ifdef _DEBUG
    if (_message)
        fprintf(stderr, "WARNING! Type of shader is \"none\".\n");
#endif
    this->type = ShaderType::none;
    this->_shaderExtension = GetDefaultShaderExtension(this->type);
    this->_pathTo = InvalidShaderPath;
    this->_isLoadable = false;
    this->_isLoaded = false;
    this->_isUnloaded = true;
    this->_isInvalid = true;
    this->_shaderID = NULL;
    this->_isForLoad = false;
    if (this->_shaderTextLenght != 0) {
        for (auto i = 0; i < this->_shaderTextLenght; i++)
            delete[] this->_cTyleShaderText[i];
        delete[] this->_cTyleShaderText;
    }
    if (this->_logLength != 0)
        delete[] this->_log;
    this->_logLength = 0;
    this->_shaderTextLenght = 0;
    this->_cmplResult = 0;
}

bool ShaderManager::ShaderWrapper::isDefaultShaderExtension() {
    for (auto i = 0; i < Utilities::CountOfExt; i++) {
        auto var = DefaultShaderExt[i];
        if (var.compare(this->_shaderExtension) == 0)
            return true;
    }
    return false;
}

bool ShaderManager::ShaderWrapper::isDefaultShaderExtension(const string &extension) {
    for (auto i = 0; i < Utilities::CountOfExt; i++) {
        auto var = DefaultShaderExt[i];
        if (var.compare(extension) == 0)
            return true;
    }
    return false;
}

ShaderType ShaderManager::ShaderWrapper::GetShaderType() {
    return this->type;
}

void ShaderManager::ShaderWrapper::SetShaderType(ShaderType _type, bool _setDefShdrExt) {
    this->type = _type;
    if (_setDefShdrExt)
        _shaderExtension = GetDefaultShaderExtension(this->type);
}

void ShaderManager::ShaderWrapper::SetShaderType(ShaderType _type, const string &_newExtension) {
    if (_type == ShaderType::none && !_newExtension.empty() && !(_newExtension == "")) {
        this->type = GetTypeByExtension(_newExtension);
#ifdef _DEBUG
    fprintf(stderr, "%s is called but the _newExtension is empty. Shaders's extension will be deducting from given _type.\n",
            __FUNCTION__);
#endif
    }
    else if (_type == ShaderType::none)
        BehaviorWhenTypeIsNone();
    else if (_newExtension.empty() && _newExtension == "") {
        this->_shaderExtension = GetDefaultShaderExtension(_type);
        this->type = _type;
    }
}

string ShaderManager::ShaderWrapper::GetPathToShader() {
    if (this->type == ShaderType::none) {
        BehaviorWhenTypeIsNone();
        return this->_pathTo;
    }
    else if (this->_pathTo.compare(InvalidShaderPath) == 0 || this->_pathTo == "" || this->_pathTo.empty())
        return this->InvalidShaderPath;
    return _pathTo;
}

void ShaderManager::ShaderWrapper::SetPathToShader(const string &path) {
    if (this->type == ShaderType::none) {
        BehaviorWhenTypeIsNone();
        return;
    }
    else if (path.empty() || path.compare(InvalidShaderPath) == 0)
        _pathTo = InvalidShaderPath;
    else if (isFileExists(path))
        _pathTo = path;
    else
        _pathTo = InvalidShaderPath;
}

string ShaderManager::ShaderWrapper::GetShaderExtension() {
    if (this->type == ShaderType::none)
        return GetDefaultShaderExtension(this->type);
    return _shaderExtension;
}

void ShaderManager::ShaderWrapper::CompileShader() {
    if (!VerifyFlag(true)) {
        fprintf(stderr, "VerifyFlag is failed. Line: %d. File: %s. Function: %s.\n", __LINE__, __FILE__,
                __FUNCTION__);
        system("PAUSE");
        exit(EXIT_FAILURE);
    }
    else if (!this->_isForLoad) {
        fprintf(stderr, "ERROR! Shader wasn't marked for load. Shader's path: %s; fucntion: %s\n",
                this->_pathTo.c_str(), __FUNCTION__);
        return;
    }
    else {
        switch (this->type) {
#ifdef _USE_OPENGL_4
		case Utilities::ShaderType::TessControlSh:
			this->_shaderID = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case Utilities::ShaderType::TessEvalSh:
			this->_shaderID = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case Utilities::ShaderType::ComputeSh:
			this->_shaderID = glCreateShader(GL_COMPUTE_SHADER);
			break;
#else
            case Utilities::ShaderType::ComputeSh:
            case Utilities::ShaderType::TessEvalSh:
            case Utilities::ShaderType::TessControlSh:
                fprintf(stderr, "WARNING! Sorry, but some features of OpenGL 4.* doesn't supports yet. Maybe in a future releases. \
				Line: %d; file: %s; function: %s", __LINE__, __FILE__, __FUNCTION__);
                break;
#endif
            case Utilities::ShaderType::VertexSh:
                this->_shaderID = glCreateShader(GL_VERTEX_SHADER);
                break;
            case Utilities::ShaderType::GeometrySh:
                this->_shaderID = glCreateShader(GL_GEOMETRY_SHADER);
                break;
            case Utilities::ShaderType::FragmentSh:
                this->_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
                break;
        }
#ifdef _DEBUG
        if(!this->_shaderID)
        {
            fprintf(stderr, "ERROR! glCreateShader is failed. Shader ID is zero.Function: %s; line: %d\n",
                    __FUNCTION__, __LINE__);
            system("PAUSE");
            std::exit(EXIT_FAILURE);
        }
#endif
        glShaderSource(this->_shaderID, this->_shaderTextLenght,
                       static_cast<const GLchar *const *>(this->_cTyleShaderText), NULL);
        glCompileShader(this->_shaderID);
        glGetShaderiv(this->_shaderID, GL_COMPILE_STATUS, &this->_cmplResult);
        glGetShaderiv(this->_shaderID, GL_INFO_LOG_LENGTH, &this->_logLength);
        this->_log = new char[this->_logLength]();
        glGetShaderInfoLog(this->_shaderID, this->_logLength, NULL, this->_log);
    }
    if (this->_cmplResult != GL_TRUE) {
        fprintf(stderr, "ERROR! Shader compilation failed! Compilation results: %i; log: %s; path: %s.\n",
                this->_cmplResult, this->_log, this->_pathTo.c_str());
        system("PAUSE");
        exit(EXIT_FAILURE);
    }
    this->_isLoadable = true;
}

void ShaderManager::ShaderWrapper::DetachShader(GLuint _programm) {

    if (this->_isUnloaded) {
        fprintf(stderr, "ERROR! Shader was already detached.\n");
        return;
    }
    else if (this->_isForLoad) {
        fprintf(stderr,
                "ERROR! The shader wasn't marked for load. It doesn't have a _shaderID. Nothing to detach. Name; %s; path: %s.\n",
                this->_shaderName.c_str(), this->_pathTo.c_str());
        return;
    }
    else if (this->_isLoadable && !this->_isLoaded) {
        fprintf(stderr,
                "WARNING! It's a valid shader, but it wasn't attached to a programm. Or it's was already detached.\n");
        return;
    }
    glDetachShader(_programm, this->_shaderID);
    this->_isUnloaded = true;
    this->_isLoaded = false;
    delete[] this->_log;
    this->_cmplResult = 0;
    this->_logLength = 0;
}

void ShaderManager::ShaderWrapper::DeleteShader() {
    if (this->_isLoaded) {
#ifdef _DEBUG
        fprintf(stderr,
                "WARNING! Shader wasn't detached from shader's programm. Call DetachShader first. Function: %s.\n",
                __FUNCTION__);
#endif
    }
    else if (!this->_isLoadable) {
        fprintf(stderr, "ERROR! The Shader wasn't compiled. Use CompileShader first. Function: %s.\n", __FUNCTION__);
        return;
    }
    else if (this->_isInvalid) {
        fprintf(stderr, "ERROR! Shader's data is invalid. See logs above for details. Function: %s.\n", __FUNCTION__);
        this->printShaderErrors();
        system("PAUSE");
        exit(EXIT_FAILURE);
    }
    glDeleteShader(this->_shaderID);
    this->_isLoadable = false;
}

void ShaderManager::ShaderWrapper::MarkForLoad() {
    if (!this->VerifyFlag(false), true) {
        fprintf(stderr, "ERROR! Something is goes wrong. See the logs above. Function: %s\n", __FUNCTION__);
        return;
    }
    else if (this->_isLoaded && this->_isForLoad) {
#ifdef _DEBUG
        fprintf(stderr,
                "WARNING! This shader was already loaded. There is no need for calling MarkForLoad. Function: %s.\n",
                __FUNCTION__);
#endif
        return;
    }
    else if (this->_isForLoad) {
#ifdef _DEBUG
        fprintf(stderr, "WARNING! The shader already was marked for load. \n");
#endif
        return;
    }
    this->_isForLoad = true;
}

void ShaderManager::ShaderWrapper::MarkAsNotLoadable() {
    this->_isLoadable = false;
}

bool ShaderManager::ShaderWrapper::isUnloaded() {
    return this->_isUnloaded;
}

const GLuint *const ShaderManager::ShaderWrapper::GetShaderID() {
    return &this->_shaderID;
}

void ShaderManager::ShaderWrapper::AttachShader(GLuint _programm) {
    if (this->_isInvalid) {
        this->printShaderErrors();
        return;
    }
    else if (!this->_isLoadable) {
        fprintf(stderr,
                "ERROR! Cannot attach the shader, because it wasn't compiled properly. Shader path: %s. Code: %d. Log: %s\n",
                this->_pathTo.c_str(), this->_cmplResult, this->_log);
        system("PAUSE");
        exit(EXIT_SUCCESS);
    }
    else if (!this->_isForLoad) {
        fprintf(stderr, "ERROR! This shader wasn't marked for load. Use MarkForLoad first. Function: %s.\n",
                __FUNCTION__);
        return;
    }
    else if (!this->isUnloaded()) {
        fprintf(stderr, "ERROR! The shader already attached. Function: %s.\n", __FUNCTION__);
        return;
    }
    glAttachShader(_programm, this->_shaderID);
}

void ShaderManager::ShaderWrapper::SetShaderText(const std::vector<string> &ShaderText) {
    if (ShaderText.size() == 0 || ShaderText.empty()) {
#ifdef _DEBUG
        fprintf(stderr, "ERROR! shader's text is missing. Line: %d. File: %s. Function: %s. \n", __LINE__, __FILE__,
                __FUNCTION__);
        system("PAUSE");
        exit(EXIT_FAILURE);
#endif
    }
    auto _size = ShaderText.size();
    //TODO: fix or review it
    this->_shaderTextLenght = _size;
    this->_cTyleShaderText = new char *[_size]();
    for (auto i = 0; i < _size - 1; i++) {
        auto _strLenght = ShaderText.at(i).length() + 2;
        _cTyleShaderText[i] = new char[_strLenght]();
        strncpy(_cTyleShaderText[i], ShaderText.at(i).c_str(), (_strLenght - 2));
        _cTyleShaderText[i][_strLenght - 2] = '\n';
        _cTyleShaderText[i][_strLenght - 1] = '\0';
    }
    auto _strLenght = ShaderText.at(_size - 1).length() + 1;
    _cTyleShaderText[_size - 1] = new char[_strLenght]();
    strncpy(_cTyleShaderText[_size - 1], ShaderText.at(_size - 1).c_str(), (_strLenght - 1));
    _cTyleShaderText[_size - 1][_strLenght - 1] = '\0';
}

void ShaderManager::ShaderWrapper::SetShaderExtension(const string &extension) {
    auto _extCopy = string(extension);
    std::transform(_extCopy.begin(), _extCopy.end(), _extCopy.begin(), ::tolower);
    if (_extCopy.empty()) {
        _shaderExtension = GetDefaultShaderExtension(this->type);
#ifdef _DEBUG
        fprintf(stderr, "WARNING! Extension is empty.\n");
#endif
    }
    else if (_extCopy.compare(DefaultShaderExt[0]) == 0) {
        _shaderExtension = GetDefaultShaderExtension(this->type);
#ifdef _DEBUG
        fprintf(stderr, "WARNING! Shader's sextension is missing. It's a right behavior?.\n");
#endif
    }
    else if (this->type == ShaderType::none)
        BehaviorWhenTypeIsNone();
    _shaderExtension = _extCopy;
}

void ShaderManager::ShaderWrapper::SetShaderName(const string &name) {
    if (name.compare(this->InvalidShaderName) == 0)
        return;
    else if (name.empty())
        this->_shaderName = InvalidShaderName;
    else if (this->type == ShaderType::none)
        BehaviorWhenTypeIsNone(true);
    else
        this->_shaderName = name;
}

bool ShaderManager::ShaderWrapper::isFileExists(const string &path) {
    if (this->type == ShaderType::none || path.empty())
        return false;
    auto _result = boost::filesystem::exists(path);
    return _result != 0;
}

bool ShaderManager::ShaderWrapper::isInvalid() {
    return this->_isInvalid;
}

bool ShaderManager::ShaderWrapper::isForLoad() {
    return VerifyFlag() && !this->_isInvalid && this->_isForLoad;
}

int ShaderManager::ShaderWrapper::_getNumericShaderType(ShaderType _type) {
    return (int) _type;
}

void ShaderManager::ShaderWrapper::printShaderErrors() {
    fprintf(stderr, "Error! Shader's data is invalid.\n");
    fprintf
            (
                    stdout,
                    "Path: %s; extension: %s; isLoaded: %i; isUnloaded: %i; log: %s; compilation result: %i; type: %i",
                    this->_pathTo.c_str(),
                    this->_shaderExtension.c_str(),
                    this->_isLoaded,
                    this->_isUnloaded,
                    (this->_logLength > 0 ? this->_log : NULL),
                    this->_cmplResult,
                    this->_getNumericShaderType(this->type)
            );
}


//TODO: implement destructor
ShaderManager::ShaderWrapper::~ShaderWrapper() {
}

bool ShaderManager::ShaderWrapper::isFileExists() {
    return this->isFileExists(this->_pathTo);
}

void ShaderManager::ShaderWrapper::RedefineFlag(std::vector<string> &ShaderText /*= INVALID_TEXT*/,
                                                const string &shaderName, bool isForLoad /*= true*/,
                                                ShaderType _type /*= ShaderType::none*/,
                                                const string &extension /*= DefaultShaderExt[0]*/,
                                                const string &pathTo /*= InvalidShaderPath*/) {
    SetShaderType(_type);
    SetShaderExtension(extension);
    SetPathToShader(pathTo);
    SetShaderName(shaderName);
    SetShaderText(ShaderText);
    if (!VerifyFlag(false))
        fprintf(stderr, "ERROR! Something is goes wrong. See logs above. Function: %s\n", __FUNCTION__);
    this->_isForLoad = isForLoad;
}


//TODO: review it
bool ShaderManager::ShaderWrapper::VerifyFlag(bool resetIfInvalid /*= false*/, bool printMessage /*= false*/) {
    bool _isValid = true;
    auto _reset = [=, &_isValid]() {
        if (printMessage)
            this->printShaderErrors();
        if (resetIfInvalid) {
            BehaviorWhenTypeIsNone();
            _isValid = false;
        }
        else
            _isValid = false;
    };
    if (this->type == ShaderType::none)
        _reset();
    else if (this->_shaderExtension.empty() || this->_shaderExtension.compare(DefaultShaderExt[0]) == 0)
        _reset();
    else if (this->_pathTo.compare(InvalidShaderPath) == 0 || this->_pathTo.empty() || !isFileExists(this->_pathTo))
        _reset();
    else if (_isLoaded && _isUnloaded)
        _reset();
    else if (_isLoadable && _isInvalid)
        _reset();
    else if (this->_cTyleShaderText == nullptr || this->_shaderTextLenght == 0)
        _reset();
    else
        this->_isInvalid = false;
    return _isValid;
}

bool ShaderManager::ShaderWrapper::IsLoaded() {
    return _isLoaded && (this->_cmplResult == GL_TRUE);
}

ShaderManager::ShaderWrapper::ShaderWrapper(std::vector<string> &ShaderText,
                                            const string &shaderName /*= InvalidShaderName*/, bool isForLoad /*= true*/,
                                            ShaderType _type /*= ShaderType::none*/,
                                            const string &extension /*= DefaultShaderExt[0]*/,
                                            const string &pathTo /*= InvalidShaderPath*/) {
    if (_type == ShaderType::none) {
        BehaviorWhenTypeIsNone();
    }
    else {
        SetShaderType(_type, extension);
        SetShaderExtension(extension);
        SetShaderName(shaderName);
        SetPathToShader(pathTo);
        SetShaderText(ShaderText);
        this->_isForLoad = isForLoad;
    }
}

std::vector<string> ShaderManager::ShaderWrapper::GetShaderText() {
    auto _tempVector = std::vector<std::string>(_shaderTextLenght);
    if (this->_shaderTextLenght <= 0) {
        fprintf(stderr, "ERROR! Shader's text lenght is 0. Function: %s\n", __FUNCTION__);
        return _tempVector;
    }
    for (auto i = 0; i < this->_shaderTextLenght; i++)
        _tempVector.push_back(this->_cTyleShaderText[i]);
    return _tempVector;
}

const string &ShaderManager::ShaderWrapper::GetShaderName() {
    return this->_shaderName;
}
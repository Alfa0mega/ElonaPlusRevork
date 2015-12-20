#include "ShaderManagerRealization.h"
using ios = ::std::ios;

ShaderManager::ShaderManagerRealization *ShaderManager::ShaderManagerRealization::sInstance = nullptr;
bool ShaderManager::ShaderManagerRealization::isAlreadyInitialized = false;
const string ShaderManager::ShaderManagerRealization::_shadersConfigName = "shaders.shlst";

void ShaderManager::ShaderManagerRealization::LoadAtRequest(const string &_shaderName) {
    for (auto &&var : this->_shadersList) {
        if (_shaderName.compare(var.GetShaderName()) == 0)
            if (!var.isInvalid())
                if (var.IsLoadable())
                    if (!var.IsLoaded())
                        var.AttachShader(this->_shaderProgram);
    }
}

void ShaderManager::ShaderManagerRealization::UnloadAtRequest(const string &_shaderName) {
    for (auto &&var : this->_shadersList) {
        if (_shaderName.compare(var.GetShaderName()) == 0)
            if (!var.isInvalid())
                if (var.IsLoadable())
                    if (var.IsLoaded())
                        var.DetachShader(this->_shaderProgram);
    }
}

void ShaderManager::ShaderManagerRealization::LoadShadersParams() {
    fstream _stream;
    std::ios_base::sync_with_stdio(false);
#ifdef _DEBUG
    #ifdef _MSC_VER
        boost::filesystem::copy_file(_shadersConfigName, _pathToShadesList,
                                 boost::filesystem::copy_option::overwrite_if_exists);
    #endif
#endif
    auto _pathToShadersConfigFile = (Utilities::GetProgramFolder(this->managerType).append("\\").append(_shadersConfigName));
    _stream.open(_pathToShadersConfigFile, ios::in);

    if (!_stream.is_open()) {
        ShowWindow(GetConsoleWindow(), SW_SHOW);
        fprintf(stderr, "%s", "ERROR: shaders file is not found. The program will be terminated.\n");
        system("pause");
        exit(EXIT_FAILURE);
    }

    while (_stream.good()) {
        auto _tempString = string("");
        getline(_stream, _tempString);
        _shaderConfigString.push_back(_tempString);
        _tempString.clear();
    }

    _stream.close();

    if (_shaderConfigString.size() == 0) {
        fprintf(stderr, "ERROR! Shader's sheet is empty! Function: %s.\n", __FUNCTION__);
        system("PAUSE");
        exit(EXIT_FAILURE);
    }

    this->_shadersList.resize(_shaderConfigString.size());
    std::vector<std::tuple<string, string, bool>> _shadersFiles(_shaderConfigString.size());
    auto iIndex = 0;
    for (auto &&var: _shaderConfigString) {
        auto _shaderName = string("");
        auto _shaderExt = string("");
        auto _isLoadable = false;
        auto _firstDot = var.find_first_of('.');
        auto _secondDot = var.find_first_of('.', _firstDot + 1);
        _shaderName = std::string(var.begin(), (var.begin() + _firstDot));
        _shaderExt = std::string(var.begin() + _firstDot, (var.begin() + _secondDot));
        _isLoadable = std::string((var.begin() + (_secondDot + 1)), (var.end() - 1)) == string("true");
        _shadersFiles[iIndex] = std::make_tuple(_shaderName, _shaderExt, _isLoadable);
#ifdef _DEBUG
        fprintf(stderr, "File name is: %s. Extension is: %s. isLoadable: %hu\n", _shaderName.c_str(),
                _shaderExt.c_str(), _isLoadable);
#endif
        iIndex++;
        this->_shadersFullQulifier.push_back(_shaderName.append(_shaderExt));
    }

    iIndex = 0;

    this->_ShadersText.resize(_shadersFullQulifier.size());
    this->ReadShadersText(_stream);

    for (auto &&var: _shadersList) {
        auto _isForLoad = std::get<2>(_shadersFiles[iIndex]);
        auto _type = Utilities::GetTypeByExtension(std::get<1>(_shadersFiles[iIndex]));
        auto _ext = std::get<1>(_shadersFiles[iIndex]);
        auto _name = std::get<0>(_shadersFiles[iIndex]);
        auto _path = Utilities::GetProgramFolder(Utilities::Directories::shaders).append( "\\").append( _shadersFullQulifier[iIndex]);
        var.RedefineFlag(this->_ShadersText[iIndex], _name, _isForLoad, _type, _ext, _path);
        iIndex++;
    }
}


//TODO: review it or completly rewrite
void ShaderManager::ShaderManagerRealization::LoadAllValidShaders() {
    if (this->_shadersList.size() == 0) {
        fprintf(stderr,
                "ERROR! Shader list is empty. Nothing to load. Possibly, call of LoadShaders has been missed.\n Also there is" \
                "possibility that the \"shaders.shlst\" is incorrect. Function: %s.\n", __FUNCTION__);
        system("PAUSE");
        exit(EXIT_FAILURE);
    }
    else if (this->_isLoadShadersIsCalled) {
        fprintf(stderr,
                "ERROR! Use %s only on starup, please. In case of shaders reloading, you must call ReLinkProgram instead.\n",
                __FUNCTION__);
        std::system("PAUSE");
        std::exit(EXIT_FAILURE);
    }
    this->_shaderProgram = glCreateProgram();
    for (auto &&var : this->_shadersList) {
        if (var.isForLoad()) {
            var.CompileShader();
            var.AttachShader(this->_shaderProgram);
        }
    }
    glLinkProgram(this->_shaderProgram);
    glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &this->_linkResult);
    glGetProgramiv(this->_shaderProgram, GL_INFO_LOG_LENGTH, &this->_linkLogLenght);
    this->_linkLog = new char[this->_linkLogLenght]();
    glGetProgramInfoLog(this->_shaderProgram, this->_linkLogLenght, NULL, this->_linkLog);
    if (this->_linkResult != GL_TRUE) {
        fprintf(stderr, "ERROR! Shader program linkage has failed. Linkage log: %s. Function: %s.\n",
                this->_linkLog, __FUNCTION__);
        std::system("PAUSE");
        std::exit(EXIT_FAILURE);
    }
    this->_isLoadShadersIsCalled = true;
#ifdef _DEBUG
    GLint _validLogLenght = 0;
    glValidateProgram(this->_shaderProgram);
    glGetProgramiv(this->_shaderProgram, GL_INFO_LOG_LENGTH, &_validLogLenght);
    auto _validLog = new char[_validLogLenght];
    glGetProgramInfoLog(this->_shaderProgram, _validLogLenght, NULL, _validLog);
    GLint _validStat = 0;
    glGetProgramiv(this->_shaderProgram, GL_VALIDATE_STATUS, &_validStat);
#endif
}

void ShaderManager::ShaderManagerRealization::UnloadAll() {
    for (auto &&var : this->_shadersList) {
        if (var.IsLoaded() && !var.isUnloaded()) {
            var.DetachShader(this->_shaderProgram);
            var.DeleteShader();
        }
        else if (var.VerifyFlag(false, true) && var.IsLoadable()) {
            var.DeleteShader();
        }
    }
}

bool ShaderManager::ShaderManagerRealization::InitShaderManager() {
    if (isAlreadyInitialized)
        return EXIT_FAILURE;
    isAlreadyInitialized = true;
    return EXIT_SUCCESS;
}

void ShaderManager::ShaderManagerRealization::ReLinkProgramm() {
    if (this->_shaderConfigString.size() == 0) {
        fprintf(stderr, "ERROR! LoadShaders was not called. Function: %s.\n", __FUNCTION__);
        system("PAUSE");
        exit(EXIT_FAILURE);
    }

    this->UnloadAll();

    for (auto &&var: this->_shadersList) {
        ATTACH:
        if (var.isForLoad() && var.IsLoadable() && var.isUnloaded()) {
            var.AttachShader(this->_shaderProgram);
        }
        else if (var.isForLoad() && var.isUnloaded() && var.IsLoadable()) {
            var.CompileShader();
            goto ATTACH;
        }
    }

    glDeleteProgram(this->_shaderProgram);
    this->_shaderProgram = glCreateProgram();

    glLinkProgram(this->_shaderProgram);
    glGetProgramiv(this->_shaderProgram, GL_LINK_STATUS, &this->_linkResult);
    glGetProgramiv(this->_shaderProgram, GL_INFO_LOG_LENGTH, &this->_linkLogLenght);
    glGetProgramInfoLog(this->_shaderProgram, this->_linkLogLenght, NULL, this->_linkLog);
#ifdef _DEBUG
    fprintf(stderr, "Result of calling glLinkProgram: code = %i; log = %s.\n", this->_linkResult, this->_linkLog);
#endif
}

void ShaderManager::ShaderManagerRealization::InstallProgramm() {
    glUseProgram(this->_shaderProgram);
}

ShaderManager::ShaderManagerRealization *ShaderManager::ShaderManagerRealization::GetInstance() {
    if (!sInstance) {
        sInstance = new ShaderManagerRealization();
    }
    return sInstance;
}

void ShaderManager::ShaderManagerRealization::ReadShadersText(fstream &stream) {
    if (stream.is_open()) {
        stream.close();
        stream.clear();
    }

    auto iIndex = 0;
    for (auto &&var: this->_shadersFullQulifier) {
        stream.open(Utilities::GetProgramFolder(Utilities::Directories::shaders).append("\\").append(var));
        if (!stream.is_open()) {
#ifdef _DEBUG
            fprintf(stderr,
                    "Error: Couldn't open a file. It's missing or path to it is incorrect. Name: %s Line: %d. Function: %s.\n",
                    var.c_str(), __LINE__, __FUNCTION__);
            stream.clear();
#endif
            continue;
        }

        auto jIndex = 0;
        while (stream.good()) {
            auto _tempStr = string("");
            getline(stream, _tempStr);
            this->_ShadersText.at(iIndex).push_back(_tempStr);
            jIndex++;
        }
        stream.close();
        stream.clear();
        iIndex++;
    }
}

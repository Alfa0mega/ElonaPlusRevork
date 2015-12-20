#pragma once

#include "stdafx.h"
#include <locale>
#include "boost/algorithm/string.hpp"

namespace Utilities {
#ifndef _DEBUG
	static const string AppDirs[] =
	{
		"bin",
		"texture",
		"shaders",
		"sound",
		"font",
		"database",
		"setings",
		"logs"
	};
#elif defined _DEBUG
    static const string TestDir = "TestDirectories";
    static const string AppDirs[] =
            {
                    string(TestDir.c_str()).append("\\bin"),
                    string(TestDir.c_str()).append("\\texture"),
                    string(TestDir.c_str()).append("\\shaders"),
                    string(TestDir.c_str()).append("\\sound"),
                    string(TestDir.c_str()).append("\\font"),
                    string(TestDir.c_str()).append("\\database"),
                    string(TestDir.c_str()).append("\\setings"),
                    string(TestDir.c_str()).append("\\logs"),
                    string(TestDir.c_str()).append("\\scripts")
            };
#endif

    enum class ShaderType : unsigned char {
        none = 0,
        ComputeSh,
        VertexSh,
        TessControlSh,
        TessEvalSh,
        GeometrySh,
        FragmentSh
    };

    static const string DefaultShaderExt[] =
            {
                    "none",
                    ".compt",
                    ".vrtx",
                    ".contrltess",
                    ".evaltess",
                    ".geomtr",
                    ".fragm"
            };

    static const unsigned short int CountOfDirs = 9;
    static constexpr unsigned short int CountOfExt = (sizeof(DefaultShaderExt) / sizeof(DefaultShaderExt[0]));

    static ShaderType GetTypeByExtension(const string &extension) {
        auto _extCopy = string(extension);
        boost::algorithm::to_lower(_extCopy);
        for (auto i = 0; i < CountOfExt; i++) {
            if (DefaultShaderExt[i].compare(_extCopy) == 0) {
                return (ShaderType) i;
            }
        }
        return ShaderType::none;
#ifdef _DEBUG
        fprintf(stderr, "WARNING! Uknown or custom shader extension: %s. Line: %d; file: %s; function: %s;\n",
                extension.c_str(), __LINE__, __FILE__, __FUNCTION__);
#endif
    }

    enum class Directories : unsigned char {
        bin = 0,
        texture,
        shaders,
        sound,
        font,
        database,
        setings,
        logs
    };

    static string GetProgramFolder(Directories _dir) {
        return AppDirs[(unsigned char) _dir];
    }
    static const std::string _logsFileName = "errorLog"
    static const std::string PathToLogFile = string.append(GetProgramFolder(Directories::logs)).append("\\").append(_logsFileName);
}
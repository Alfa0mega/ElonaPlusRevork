#pragma once

#include "../stdafx.h"
#include "UDS.h"

class OScriptInterpreter {
public:
    static const string InvalidWorkingDirectory;
    const char *_scriptExtension = ".osci";
    OScriptInterpreter();
    ~OScriptInterpreter();
private:
    string workingDirectory;
protected:
};


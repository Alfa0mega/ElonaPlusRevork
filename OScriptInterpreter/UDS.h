//
// Created by AMEGA on 29-Jul-15.
//
#include "../stdafx.h"
#pragma once
#ifndef OPENGLAPP_UDS_H
#define OPENGLAPP_UDS_H

#ifdef _USE_WIDE_CHAR
    #define _u_char wchar_t
    #define _u_string std::wstring
#else
    #define _u_char char
    #define _u_string std::string
#endif

struct UDS_DATA_STRUCT
{
    union {
        unsigned int _bit: 1; //also bool
        unsigned int _byte: 8; //also char
        int _integer;
        double _double;
    };
    void setText(const _u_char*);
    void setText(const _u_char*, const unsigned int);
    void setText(const _u_string&);
private:
    unsigned int _textLength = 0;
    _u_char* _text = nullptr;
};

struct UDS {
private:
    unsigned int _id;
    UDS_DATA_STRUCT _data;
public:
    unsigned int GetId();
    UDS() = delete;
    UDS(const UDS&) = delete;
    UDS(unsigned int id, const UDS_DATA_STRUCT& _inpData);
};


#endif //OPENGLAPP_UDS_H

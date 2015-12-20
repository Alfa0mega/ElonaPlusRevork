#pragma once
#define _USE_SFML
#define _CRT_NONSTDC_NO_DEPRECATE
#define cimg_use_openmp
#define cimg_display 2
#include <GL/glew.h>
#include <windows.h>
#include <GL/wglew.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <GL/wglext.h>
#include <boost/filesystem.hpp>
#include <typeinfo>
#include <string>
#include <CImg/CImg.h>
#ifndef _USE_SFML
    #define AppCore AppCore
#else
    #define AppCore AppCore_sfml
#endif

using string = ::std::string;
using fstream = ::std::fstream;

#define _DEBUG
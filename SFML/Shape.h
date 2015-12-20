//
// Created by AMEGA on 14-Dec-15.
//

#ifndef OPENGLAPP_SHAPE_H
#define OPENGLAPP_SHAPE_H

#include "../stdafx.h"
//=========================SFML2=============================
#include <sfml/Graphics.hpp>
#include <sfml/OpenGL.hpp>
#include <sfml/System.hpp>
#include <sfml/Window.hpp>
#include <sfml/Main.hpp>
//===========================================================
#include <memory>

namespace cimg = cimg_library;

class SFShape {
    protected:
        std::uint16_t _ID;
        std::uint16_t _heigth;
        std::uint16_t _width;
    public:
        SFShape(SFShape * = nullptr);
        SFShape() = delete;
    private:
        std::shared_ptr<SFShape> _parrent;
        sf::Texture _spriteSF;
        cimg::CImg<std::uint8_t> _sprite;
};


#endif //OPENGLAPP_SHAPE_H

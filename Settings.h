//
// Created by AMEGA on 04-Sep-15.
//

#include <glm/matrix.hpp>

#ifndef OPENGLAPP_SETTINGS_H
#define OPENGLAPP_SETTINGS_H
namespace AppCore {
    namespace Settings {
        static glm::vec3 camDirection = glm::vec3(0, 1, 0);
        static float FoV = 70.f;
        static float nearZClipping = 0.1f;
        static float farZClipping = 100.f;
        static float _Width = 16.f;
        static float _Heigth = 9.f;
        static float AspectRatio = _Width / _Heigth;
    }
}
#endif //OPENGLAPP_SETTINGS_H

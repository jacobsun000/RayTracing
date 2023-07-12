#pragma once

#include "Camera.h"
#include "Common.h"
#include "GeometryList.h"
#include "Sphere.h"

class Scene {
   public:
    Scene(Camera camera, GeometryList objects)
        : camera(camera), objects(objects) {}

   public:
    Camera camera;
    GeometryList objects;
};
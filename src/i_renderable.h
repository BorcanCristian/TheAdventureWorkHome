#pragma once

#include "i_thing.h"
#include "map.h"
#include "renderer.h"

class IRenderable
{
public:
    virtual ~IRenderable() = default;

public:
    virtual void render(Renderer &renderer, const Map::Viewport &viewport) = 0;
    virtual int  z() const
    {
        return 0;
    }
};
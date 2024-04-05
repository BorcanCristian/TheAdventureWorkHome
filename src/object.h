#pragma once

#include "i_collidable.h"
#include "i_renderable.h"
#include "i_thing.h"
#include "renderer.h"
#include "sprite.h"

#include <cstdint>
#include <iostream>

class Object
  : public IThing
  , public IRenderable
  , public ICollidable
{
public:
    Object(Renderer           &renderer,
           float               xx,
           float               yy,
           const std::uint8_t *image,
           const std::size_t   image_size)
      : m_sprite{ image, image_size, renderer }
    {
        x() = xx;
        y() = yy;

        width()  = m_sprite.width();
        height() = m_sprite.height();

        set_collision_box({ width() / 2, height() / 2, width(), height() });
    }

public:
    void update(Game &game, float attenuation = 1.F) override;

    void render(Renderer &renderer, const Map::Viewport &viewport) override;
    bool allow_passthrough() const override
    {
        return false;
    }

private:
    Sprite m_sprite;
};

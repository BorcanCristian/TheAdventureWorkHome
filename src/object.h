#pragma once

#include "i_collidable.h"
#include "i_renderable.h"
#include "i_thing.h"
#include "renderer.h"
#include "sprite.h"

#include <cstdint>

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

        m_sprite.x() = xx;
        m_sprite.y() = yy;

        set_collision_box(
            { m_sprite.width() / 2, m_sprite.height() / 2, m_sprite.width(), m_sprite.height() });
    }

public:
    void update(Game &game, float attenuation = 1.F) override;

    void   render(Renderer &renderer) override;
    float &render_x() override;
    float &render_y() override;

protected:
    float c_x() const override;
    float c_y() const override;

private:
    Sprite m_sprite;
};

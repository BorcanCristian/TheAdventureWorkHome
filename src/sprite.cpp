#include "sprite.h"

#include <cmath>

Sprite::Sprite(const std::uint8_t *data, std::size_t size, Renderer &renderer)
  : m_width{ 0 }
  , m_height{ 0 }
  , m_image_id{ renderer.load_image(data, size, &m_width, &m_height) }
{}

void Sprite::render(Renderer &renderer, float x, float y)
{
    renderer.draw_image(
        m_image_id,
        0,
        0,
        static_cast<std::int32_t>(std::round(x + static_cast<float>(m_width) / 2.F)),
        static_cast<std::int32_t>(std::round(y + static_cast<float>(m_height) / 2.F)),
        false,
        false,
        m_width,
        m_height,
        m_width,
        m_height);
}

float Sprite::width() const
{
    return static_cast<float>(m_width);
}

float Sprite::height() const
{
    return static_cast<float>(m_height);
}

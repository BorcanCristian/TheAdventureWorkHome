#include "object.h"

void Object::update(Game &game, float attenuation)
{}

void Object::render(Renderer &renderer)
{
    m_sprite.render(renderer);
}

float &Object::render_x()
{
    return m_sprite.x();
}

float &Object::render_y()
{
    return m_sprite.y();
}

float Object::c_x() const
{
    return m_sprite.x();
}

float Object::c_y() const
{
    return m_sprite.y();
}

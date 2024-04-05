#include "object.h"

void Object::update(Game &game, float attenuation)
{}

void Object::render(Renderer &renderer)
{
    m_sprite.render(renderer, x(), y());
}

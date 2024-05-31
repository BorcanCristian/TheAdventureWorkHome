#include "object.h"
#include "map.h"

void Object::update(Game &game, float attenuation)
{}

void Object::render(Renderer &renderer, const Map::Viewport &viewport)
{
    m_sprite.render(renderer, x() - viewport.x, y() - viewport.y);
}

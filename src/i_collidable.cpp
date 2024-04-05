#include "i_collidable.h"
#include "i_thing.h"

#include <cmath>
#include <cstdint>

const Rect &ICollidable::get_collision_box() const
{
    return m_collision_box;
}

void ICollidable::set_collision_box(Rect collision_box)
{
    m_collision_box = collision_box;
}

void ICollidable::render_collision_box(Renderer &renderer, bool is_colliding)
{
    const auto *self = dynamic_cast<IThing *>(this);
    if (self == nullptr)
    {
        return;
    }

    if (!is_colliding)
    {
        renderer.set_color({ 0, 0, 255, 255 });
    }
    else
    {
        renderer.set_color({ 255, 0, 0, 255 });
    }
    renderer.draw_rect(static_cast<std::int32_t>(std::round(self->x() + m_collision_box.x)),
                       static_cast<std::int32_t>(std::round(self->y() + m_collision_box.y)),
                       m_collision_box.width,
                       m_collision_box.height);

    if (!is_colliding)
    {
        renderer.set_color({ 0, 0, 255, 64 });
    }
    else
    {
        renderer.set_color({ 255, 0, 0, 64 });
    }
    renderer.fill_rect(static_cast<std::int32_t>(std::round(self->x() + m_collision_box.x)),
                       static_cast<std::int32_t>(std::round(self->y() + m_collision_box.y)),
                       m_collision_box.width,
                       m_collision_box.height);
}

bool ICollidable::is_colliding(const ICollidable &other)
{
    const auto *self = dynamic_cast<IThing *>(this);
    if (self == nullptr)
    {
        return false;
    }

    const auto other_thing = dynamic_cast<const IThing *>(&other);
    if (other_thing == nullptr)
    {
        return false;
    }

    auto r1 = Rect{ self->x() + m_collision_box.x,
                    self->y() + m_collision_box.y,
                    m_collision_box.width,
                    m_collision_box.height };

    auto r2 = Rect{ other_thing->x() + other.m_collision_box.x,
                    other_thing->y() + other.m_collision_box.y,
                    other.m_collision_box.width,
                    other.m_collision_box.height };

    return r1.x < r2.x + r2.width && r1.x + r1.width > r2.x && r1.y < r2.y + r2.height &&
           r1.y + r1.height > r2.y;
}

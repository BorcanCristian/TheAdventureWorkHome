#include "i_aggravatable.h"

#include <iostream>

class CollidableThing : public ICollidable
{
public:
    CollidableThing(float x, float y)
      : m_x{ x }
      , m_y{ y }
    {}

    float c_x() const override
    {
        return m_x;
    }
    float c_y() const override
    {
        return m_y;
    }

private:
    float m_x;
    float m_y;
};

void IAggravatable::set_aggro_area(Rect aggro_area)
{
    m_aggravation_zone.set_collision_box(aggro_area);
}

void IAggravatable::set_aggravated_by(ICollidable &thing)
{
    m_aggravated_by = &thing;
}

ICollidable *IAggravatable::aggravated_by() const
{
    return m_aggravated_by;
}

bool IAggravatable::has_aggro()
{
    return m_aggravation_zone.is_colliding(*m_aggravated_by);
}

void IAggravatable::render_aggro_area(Renderer &renderer)
{
    m_aggravation_zone.render_collision_box(renderer, has_aggro());
}

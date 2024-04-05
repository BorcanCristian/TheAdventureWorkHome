#include "i_aggravatable.h"

void IAggravatable::set_aggro_area(Rect aggro_area)
{
    m_aggravation_zone = aggro_area;
}

void IAggravatable::set_aggravated_by(IThing &thing)
{
    m_aggravated_by = &thing;
}

IThing *IAggravatable::aggravated_by() const
{
    return m_aggravated_by;
}

bool IAggravatable::has_aggro()
{
    if (m_aggravated_by == nullptr)
    {
        return false;
    }

    const auto *self = dynamic_cast<IThing *>(this);
    if (self == nullptr)
    {
        return false;
    }

    return m_aggravated_by->x() >= m_aggravation_zone.x + self->x() &&
           m_aggravated_by->x() <= m_aggravation_zone.x + m_aggravation_zone.width + self->x() &&
           m_aggravated_by->y() >= m_aggravation_zone.y + self->y() &&
           m_aggravated_by->y() <= m_aggravation_zone.y + m_aggravation_zone.height + self->y();
}

void IAggravatable::render_aggro_area(Renderer &renderer)
{
    const auto *self = dynamic_cast<IThing *>(this);
    if (self == nullptr)
    {
        return;
    }

    renderer.set_color({ 255, 0, 0, 255 });
    renderer.draw_rect(static_cast<std::int32_t>(self->x() + m_aggravation_zone.x),
                       static_cast<std::int32_t>(self->y() + m_aggravation_zone.y),
                       static_cast<std::int32_t>(m_aggravation_zone.width),
                       static_cast<std::int32_t>(m_aggravation_zone.height));
}

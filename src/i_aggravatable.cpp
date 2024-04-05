#include "i_aggravatable.h"

#include <iostream>

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

    const auto aggr_x = m_aggravated_by->x() + (m_aggravated_by->width() / 2);
    const auto aggr_y = m_aggravated_by->y() + (m_aggravated_by->height() / 2);

    const auto aggr_zone_min_x = m_aggravation_zone.x + self->x();
    const auto aggr_zone_min_y = m_aggravation_zone.y + self->y();
    const auto aggr_zone_max_x = m_aggravation_zone.width + aggr_zone_min_x;
    const auto aggr_zone_max_y = m_aggravation_zone.height + aggr_zone_min_y;

    return aggr_x >= aggr_zone_min_x && aggr_x <= aggr_zone_max_x && aggr_y >= aggr_zone_min_y &&
           aggr_y <= aggr_zone_max_y;
}

void IAggravatable::render_aggro_area(Renderer &renderer, const Map::Viewport &viewport)
{
    const auto *self = dynamic_cast<IThing *>(this);
    if (self == nullptr)
    {
        return;
    }

    renderer.set_color({ 255, 0, 0, 255 });
    renderer.draw_rect(static_cast<std::int32_t>(self->x() + m_aggravation_zone.x - viewport.x),
                       static_cast<std::int32_t>(self->y() + m_aggravation_zone.y - viewport.y),
                       static_cast<std::int32_t>(m_aggravation_zone.width),
                       static_cast<std::int32_t>(m_aggravation_zone.height));
}

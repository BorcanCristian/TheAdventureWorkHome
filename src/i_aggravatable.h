#pragma once

// Adjective
// aggravatable (comparative more aggravatable, superlative most aggravatable)
//    Capable of being aggravated.

#include "i_collidable.h"
#include "i_thing.h"

class IAggravatable
{
public:
    virtual ~IAggravatable() = default;

public:
    void    set_aggro_area(Rect aggro_area);
    void    set_aggravated_by(IThing &thing);
    IThing *aggravated_by() const;
    bool    has_aggro();

    void render_aggro_area(Renderer &renderer, const Map::Viewport &viewport);

private:
    IThing *m_aggravated_by{ nullptr };
    Rect    m_aggravation_zone;
};
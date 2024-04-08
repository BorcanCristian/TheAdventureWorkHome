#pragma once

#include "map.h"
#include "renderer.h"

struct Rect
{
    float x;
    float y;
    float width;
    float height;
};

class ICollidable
{
public:
    virtual ~ICollidable() = default;

public:
    Rect                    get_collision_box() const;
    void                    set_collision_box(Rect collision_box);
    std::pair<float, float> get_center() const;

    virtual void render_collision_box(Renderer            &renderer,
                                      const Map::Viewport &viewport,
                                      bool                 is_colliding = false);
    virtual bool is_colliding(const ICollidable &other);

    virtual bool allow_passthrough() const
    {
        return true;
    }

private:
    Rect m_collision_box;
};
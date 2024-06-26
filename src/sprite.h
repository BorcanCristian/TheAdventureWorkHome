#pragma once

#include "renderer.h"

#include <cstdint>

class Sprite
{
public:
    struct Rect
    {
        float x;
        float y;
        float width;
        float height;
    };

public:
    Sprite(const std::uint8_t *data, std::size_t size, Renderer &renderer);

    virtual ~Sprite() = default;

    // Copy constructor
    // ex: Sprite s1;
    //     Sprite s2{ s1 };
    Sprite(const Sprite &) = default;
    // Move constructor
    // ex: Sprite s1;
    //     Sprite s2{ std::move(s1) };
    Sprite(Sprite &&) = default;
    // Copy assignment operator
    // ex: Sprite s1{ data, size, renderer };
    //     Sprite s2;
    //     s2 = s1;
    Sprite &operator=(const Sprite &) = default;
    // Move assignment operator
    // ex: Sprite s1;
    //     Sprite s2;
    //     s2 = std::move(s1);
    Sprite &operator=(Sprite &&) = default;

    virtual void render(Renderer &renderer, float x, float y);

    float width() const;
    float height() const;

protected:
    inline std::int32_t image_id() const
    {
        return m_image_id;
    }

private:
    std::int32_t m_width;
    std::int32_t m_height;

    std::int32_t m_image_id;
};

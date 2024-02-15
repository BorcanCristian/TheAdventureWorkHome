#pragma once

class Game;

class IEntity
{
    public:
    virtual ~IEntity() = default;

    public:
    virtual void update(Game &game, float attenuation = 1.F) = 0;
    
    public:
    float &x();
    const float x() const;
    float &y();
    const float &&y() const;
    float &scale_x();
    const float &scale_x() const;
    float &scale_y();
    const float &scale_y() const;

    private:
    float m_x{ 0.F};
    float m_y{ 0.F};

    float m_scale_x{ 1.F};
    float m_scale_y{ 1.F};
};

#pragma once

#include "animated_sprite.h"
#include "events.h"
#include "i_attacker.h"
#include "i_collidable.h"
#include "i_destroyable.h"
#include "i_input_handler.h"
#include "i_renderable.h"
#include "i_thing.h"
#include "sound.h"

class Game;

class Hero
  : public IThing
  , public IRenderable
  , public IInputHandler
  , public ICollidable
  , public IAttacker
  , public IDestroyable
{
private:
    enum class Orientation
    {
        Up,
        Down,
        Left,
        Right
    };

public:
    Hero(Renderer &renderer, Sound &sound);

    void  attack();
    float speed() const;

    bool  is_attacking() const override;
    float attack_power() const override;

    void update(Game &game, float attenuation = 1.F) override;
    void render(Renderer &renderer) override;
    void on_key_pressed(const KeyPressEvent &event) override;
    void on_key_released(const KeyReleaseEvent &event) override;

    void take_damage(float damage) override;
    bool should_be_destroyed() override;

private:
    AnimatedSprite m_sprite;
    Orientation    m_orientation{ Orientation::Down };
    float          m_speed{ 80.F };

    bool         m_is_attacking{ false };
    bool         m_is_moving{ false };
    std::int32_t m_attack_sound_id{ -1 };

    float m_health{ 10.F };

    Sound &m_sound;
};
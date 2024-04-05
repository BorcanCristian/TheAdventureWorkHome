#include "slime.h"
#include "codes.h"
#include "game.h"

#include <resources.h>

#include <iostream>
#include <random>

enum SpriteSet
{
    IdleRight,
    JumpingRight,
    AttackingRight,
    TakingDamageRight,
    Death
};

static constexpr std::int32_t MAX_FRAMES    = 7;
static constexpr std::int32_t IDLE_FRAMES   = 4;
static constexpr std::int32_t JUMP_FRAMES   = 6;
static constexpr std::int32_t ATTACK_FRAMES = 7;
static constexpr std::int32_t DAMAGE_FRAMES = 3;
static constexpr std::int32_t DEATH_FRAMES  = 5;

Slime::Slime(Renderer &renderer, Sound &sound)
  : m_sprite{ resource_slime, resource_slime_size, renderer }
  , m_generator{ std::random_device{}() }
  , m_distribution{ 0, 10 }
  , m_sound{ sound }
{
    width()  = m_sprite.width() / MAX_FRAMES;
    height() = m_sprite.height() / MAX_FRAMES;

    set_collision_box({ -9, -7, 19, 18 });
    set_aggro_area({ -36, -32, 76, 72 });

    m_sprite.set_sprite_set(SpriteSet::IdleRight);
    m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - IDLE_FRAMES);
    m_sprite.set_frame_time(std::chrono::milliseconds{ 100 });

    m_attack_sound_id =
        sound.load_sample(resource__04_slime_attack, resource__04_slime_attack_size);
}

void Slime::attack()
{
    m_is_attacking = true;

    switch (m_orientation)
    {
    case Orientation::Left: {
        m_sprite.set_sprite_set(SpriteSet::AttackingRight, true);
        break;
    }
    case Orientation::Right: {
        m_sprite.set_sprite_set(SpriteSet::AttackingRight);
        break;
    }
    }

    m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - ATTACK_FRAMES);
    m_sprite.reset();
    m_sound.play_sample(m_attack_sound_id);
}

void Slime::update(Game &game, float attenuation)
{
    if (m_is_attacking)
    {
        if (m_sprite.current_frame() == ATTACK_FRAMES - 1)
        {
            m_is_attacking = false;
        }
        else
        {
            return;
        }
    }
    else if (has_aggro())
    {
        const auto *aggro_for = dynamic_cast<ICollidable *>(aggravated_by());
        if (aggro_for == nullptr)
        {
            return;
        }

        if (!is_colliding(*aggro_for))
        {
            const auto *aggravated_by = this->aggravated_by();

            if (aggravated_by->x() < x())
            {
                x() -= m_speed * attenuation;
                m_orientation = Orientation::Left;
            }
            else
            {
                x() += m_speed * attenuation;
                m_orientation = Orientation::Right;
            }

            if (aggravated_by->y() < y())
            {
                y() -= m_speed * attenuation;
            }
            else
            {
                y() += m_speed * attenuation;
            }

            m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
        }
        else
        {
            attack();
        }

        return;
    }

    const auto now = std::chrono::steady_clock::now();
    if (now - m_last_gen_time >= std::chrono::seconds{ 1 })
    {
        m_last_gen_time = now;

        switch (m_distribution(m_generator))
        {
        case 0: {
            m_direction = Direction::Up;
            m_is_moving = true;
            break;
        }
        case 1: {
            m_direction = Direction::Down;
            m_is_moving = true;
            break;
        }
        case 2: {
            m_direction = Direction::Left;
            m_is_moving = true;
            break;
        }
        case 3: {
            m_direction = Direction::Right;
            m_is_moving = true;
            break;
        }
        default:
            m_is_moving = false;
            break;
        }
    }

    if (m_is_moving)
    {
        switch (m_direction)
        {
        case Direction::Up: {
            y() -= m_speed * attenuation;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);

            m_is_moving = true;
            break;
        }
        case Direction::Down: {
            y() += m_speed * attenuation;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight, m_orientation == Orientation::Left);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);

            m_is_moving = true;
            break;
        }
        case Direction::Left: {
            x() -= m_speed * attenuation;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight, true);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
            m_orientation = Orientation::Left;

            m_is_moving = true;
            break;
        }
        case Direction::Right: {
            x() += m_speed * attenuation;
            m_sprite.set_sprite_set(SpriteSet::JumpingRight);
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - JUMP_FRAMES);
            m_orientation = Orientation::Right;

            m_is_moving = true;
            break;
        }
        }
    }
    else if (!m_is_attacking)
    {
        // Go to idle
        switch (m_orientation)
        {
        case Orientation::Left: {
            m_sprite.set_sprite_set(SpriteSet::IdleRight, true);
            break;
        }
        case Orientation::Right: {
            m_sprite.set_sprite_set(SpriteSet::IdleRight);
            break;
        }
        }

        if (m_sprite.total_frames() != IDLE_FRAMES)
        {
            m_sprite.set_total_frames(MAX_FRAMES, MAX_FRAMES - IDLE_FRAMES);
            m_sprite.reset();
        }
    }
}

void Slime::render(Renderer &renderer)
{
    m_sprite.render(renderer, x(), y());
}

void Slime::take_damage(float damage)
{
    m_health -= damage;

    std::cout << "Slime took " << damage << " damage. Health: " << m_health << std::endl;

    if (m_health <= 0.F)
    {
        m_should_be_destroyed = true;
    }
}

bool Slime::should_be_destroyed()
{
    return m_should_be_destroyed;
}

bool Slime::is_attacking() const
{
    return m_is_attacking;
}

float Slime::attack_power() const
{
    return 0.5F;
}

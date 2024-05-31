#include "game.h"
#include "events.h"
#include "i_attacker.h"
#include "i_collidable.h"
#include "i_destroyable.h"
#include "i_input_handler.h"
#include "i_renderable.h"
#include "object.h"
#include "renderer.h"
#include "resources.h"

#include <fmt/format.h>

#include <algorithm>
#include <map>
#include <unordered_set>

Game::~Game() = default;

void Game::load_assets(Renderer &renderer)
{
    m_map.emplace(renderer);

    m_bg_music_id = m_sound.load_music(resource_world_bg, resource_world_bg_size);
    m_sound.play_music(m_bg_music_id);

    auto *tree = new Object{ renderer, 100, 100, resource_tree_01, resource_tree_01_size };
    tree->set_collision_box({ 33, 70, 28, 16 });
    m_things.emplace(tree->id(), tree);

    tree = new Object{ renderer, 200, 200, resource_tree_01, resource_tree_01_size };
    tree->set_collision_box({ 33, 70, 28, 16 });
    m_things.emplace(tree->id(), tree);

    auto *bush = new Object{ renderer, 300, 300, resource_bush_01, resource_bush_01_size };
    m_things.emplace(bush->id(), bush);

    auto *hero = new Hero{ renderer, m_sound };
    hero->x()  = 500;
    m_things.emplace(hero->id(), hero);

    for (int i = 0; i < 5; ++i)
    {
        auto *slime = new Slime{ renderer, m_sound };
        slime->x() += 100 + i * 64;
        slime->y() += 100 + i * 45;

        slime->set_aggravated_by(*hero);

        m_things.emplace(slime->id(), slime);
    }

    m_hero = hero;
}

void Game::render(Renderer &renderer, const RenderEvent &event)
{
    renderer.set_color({ 0, 0, 0, 255 });
    renderer.clear();

    m_map->update(m_hero->x(), m_hero->y(), m_hero->speed(), event.seconds_elapsed);
    m_map->render();

    std::vector<std::size_t> destroyed_ids;
    for (const auto &[id, thing] : m_things)
    {
        if (auto *destroyable = dynamic_cast<IDestroyable *>(thing.get()))
        {
            if (destroyable->should_be_destroyed())
            {
                destroyed_ids.push_back(id);
            }
        }
    }

    for (auto i = 0; i < destroyed_ids.size(); ++i)
    {
        m_things.erase(destroyed_ids[i]);
    }

    std::multimap<int, IRenderable *> renderables;
    for (const auto &[id, thing] : m_things)
    {
        thing->update(*this, event.seconds_elapsed);

        thing->x() = std::clamp(thing->x(), 0.F, m_map->width() - thing->width() / 2.F);
        thing->y() = std::clamp(thing->y(), 0.F, m_map->height() - thing->height() / 2.F);

        if (auto *renderable = dynamic_cast<IRenderable *>(thing.get()))
        {
            renderables.emplace(renderable->z(), renderable);
        }
    }

    for (const auto &[z, renderable] : renderables)
    {
        renderable->render(renderer, m_map->viewport());
    }

    std::unordered_set<ICollidable *> colliding;
    colliding.reserve(m_things.size());
    std::unordered_set<ICollidable *> not_colliding;
    not_colliding.reserve(m_things.size());
    std::vector<std::pair<std::int32_t, ICollidable *>> collidables;
    collidables.reserve(m_things.size());
    for (const auto &[id, thing] : m_things)
    {
        if (auto *collidable = dynamic_cast<ICollidable *>(thing.get()))
        {
            not_colliding.insert(collidable);
            collidables.emplace_back(id, collidable);
        }
    }
    for (std::size_t i = 0; i < collidables.size() - 1; ++i)
    {
        for (std::size_t j = i + 1; j < collidables.size(); ++j)
        {
            if (collidables[i].second->is_colliding(*collidables[j].second))
            {
                const auto i_id = collidables[i].first;
                const auto j_id = collidables[j].first;

                auto *ci = collidables[i].second;
                auto *cj = collidables[j].second;

                if (!cj->allow_passthrough())
                {
                    fixup_collisions(ci, cj);
                }
                if (!ci->allow_passthrough())
                {
                    fixup_collisions(cj, ci);
                }

                try_attack(i_id, j_id);
                try_attack(j_id, i_id);

                colliding.insert(collidables[i].second);
                colliding.insert(collidables[j].second);

                not_colliding.erase(collidables[i].second);
                not_colliding.erase(collidables[j].second);
            }
        }
    }

    //    for (const auto &collidable : colliding)
    //    {
    //        collidable->render_collision_box(renderer, m_map->viewport(), true);
    //    }
    //
    //    for (const auto &collidable : not_colliding)
    //    {
    //        collidable->render_collision_box(renderer, m_map->viewport(), false);
    //    }

    fps_timer += event.seconds_elapsed;
    total_seconds_elapsed += event.seconds_elapsed;
    ++frame_counter;

    if (fps_timer >= 1.0F)
    {
        fmt::print("FPS: {}\n", frame_counter);
        fps_timer     = fps_timer - 1.0F;
        frame_counter = 0;
    }
}

void Game::on_window_shown()
{}

void Game::on_window_hidden()
{}

void Game::on_window_moved(const WindowMoveEvent &event)
{}

void Game::on_window_resized(const WindowResizeEvent &event)
{}

void Game::on_window_focus_gained()
{}

void Game::on_window_focus_lost()
{}

void Game::on_window_closed()
{}

void Game::on_window_mouse_entered()
{}

void Game::on_window_mouse_left()
{}

void Game::on_mouse_moved(const MouseMoveEvent &event)
{}

void Game::on_mouse_button_pressed(const MouseButtonPressEvent &event)
{}

void Game::on_mouse_button_released(const MouseButtonReleaseEvent &event)
{}

void Game::on_mouse_wheel(const MouseWheelEvent &event)
{}

void Game::on_key_pressed(const KeyPressEvent &event)
{
    for (auto &[id, thing] : m_things)
    {
        if (auto *input_handler = dynamic_cast<IInputHandler *>(thing.get()))
        {
            input_handler->on_key_pressed(event);
        }
    }
}

void Game::on_key_released(const KeyReleaseEvent &event)
{
    for (auto &[id, thing] : m_things)
    {
        if (auto *input_handler = dynamic_cast<IInputHandler *>(thing.get()))
        {
            input_handler->on_key_released(event);
        }
    }
}

bool Game::is_mouse_button_pressed(MouseButton button) const
{
    return m_mouse_button_states[static_cast<std::size_t>(button)] != 0;
}

bool Game::is_key_pressed(KeyCode key_code) const
{
    return m_keyboard_state[static_cast<std::size_t>(key_code)] != 0;
}

void Game::try_attack(std::int32_t thing_id_1, std::int32_t thing_id_2)
{
    // Find out if thing_1 is an attacker
    const auto attacker_it = m_things.find(thing_id_1);
    if (attacker_it != m_things.end()) // thing_1 is an attacker
    {
        const auto *attacker = dynamic_cast<IAttacker *>(attacker_it->second.get());
        if (attacker != nullptr && attacker->is_attacking()) // thing_1 is attacking
        {
            // We are in an attack phase

            // Find out if our target is a destroyable
            auto dest_it = m_things.find(thing_id_2);
            if (dest_it != m_things.end()) // Our target is a destroyable

            {
                auto *dest = dynamic_cast<IDestroyable *>(dest_it->second.get());
                if (dest == nullptr)
                {
                    return;
                }

                // Find out if the attacker has already landed an attack on some target
                auto attack_landed = m_landed_attacks.find(attacker_it->first);
                if (attack_landed != m_landed_attacks.end()) // The attacker has already
                                                             // landed an attack on some
                                                             // target
                {
                    // Find out if the attacker has already landed an attack on our
                    // target
                    const auto target_hit = attack_landed->second.find(dest_it->first);
                    if (target_hit == attack_landed->second.end()) // The attacker has not
                                                                   // landed an attack on our
                                                                   // target
                    {
                        // Mark the target as hit in this attack phase
                        attack_landed->second.insert(dest_it->first);
                        // Deal damage to the target
                        dest->take_damage(attacker->attack_power());
                    }
                }
                else // The attacker has just started their attack phase
                {
                    // Mark the attacker as having landed an attack on our target
                    // Add the attacker to the landed attacks and mark the target as hit
                    m_landed_attacks.emplace(attacker_it->first,
                                             std::unordered_set<int32_t>{ dest_it->first });
                    // Deal damage to the target
                    dest->take_damage(attacker->attack_power());
                }
            }
        }
        else // thing_1 is not attacking
        {
            // thing_1's attack period has ended
            // Remove the attacker from the landed attacks
            m_landed_attacks.erase(attacker_it->first);
        }
    }
}

void Game::fixup_collisions(ICollidable *lhs, ICollidable *rhs) noexcept
{
    auto *ci_thing = dynamic_cast<IThing *>(lhs);
    auto *cj_thing = dynamic_cast<IThing *>(rhs);

    const auto [ci_center_x, ci_center_y] = lhs->get_center();
    const auto [cj_center_x, cj_center_y] = rhs->get_center();

    const auto ci_cb = lhs->get_collision_box();
    const auto cj_cb = rhs->get_collision_box();

    if (ci_thing != nullptr && cj_thing != nullptr)
    {
        // The thing that is moving is on top or under the immovable thing
        // We use a tolerance of 2 pixels
        if ((ci_cb.y + ci_cb.height - 2) < cj_cb.y || (ci_cb.y + 2) > cj_cb.y + cj_cb.height)
        {
            // If we're on top or bottom then move the thing back vertically
            if (cj_center_y < ci_center_y)
            {
                ci_thing->y() = cj_cb.y + cj_cb.height - (ci_cb.y - ci_thing->y());
            }
            else if (cj_center_y >= ci_center_y)
            {
                ci_thing->y() = cj_cb.y - ci_cb.height - (ci_cb.y - ci_thing->y());
            }
        }
        else
        {
            // If we're on the left or right then move the thing back horizontally
            if (cj_center_x < ci_center_x)
            {
                ci_thing->x() = cj_cb.x + cj_cb.width - (ci_cb.x - ci_thing->x());
            }
            else if (cj_center_x >= ci_center_x)
            {
                ci_thing->x() = cj_cb.x - ci_cb.width - (ci_cb.x - ci_thing->x());
            }
        }
    }
}

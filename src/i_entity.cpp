#include "i_entity.h"

float &IEntity::x()
{
    return m_x;
}

const float &IEntity::x() const
{
    return m_x;
}

float &IEntity::y()
{
    return m_y;
}

const float &IEntity::y() const
{
    return m_y;
}

float &IEntity::scale_x()
{
    return scale_x;
}

const float &IEntity::scale_x() const
{
    return scale_x;
}

float &IEntity::scale_y()
{
    return scale_y;
}

const float &IEntity::scale_y() const
{
    return scale_y;
}

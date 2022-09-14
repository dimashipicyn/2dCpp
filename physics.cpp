#include "physics.h"

#include <stdlib.h>
#include <SDL.h>

bool Physic::has_intersection(const AABB &aabb) {
    
    for (auto& body : dynamic_body_)
    {
        if (body.aabb.has_intersection(aabb)) {
            return true;
        }
    }
    return false;
}


void Physic::step()
{
    for (auto& d_body : dynamic_body_)
    {
        if (!d_body.is_active) {
            continue;
        }
        
        d_body.contacted_bodyes.clear();

        for (auto& s_body : static_body_)
        {
            if (!s_body.is_active) {
                continue;
            }
            
            bool is_equal_layer = (d_body.layer & s_body.layer);
            bool has_intersect = d_body.aabb.has_intersection(s_body.aabb);
            if (is_equal_layer && has_intersect) {
                d_body.contacted_bodyes.push_back(&s_body);
            }
        }

        for (auto& d_body2 : dynamic_body_)
        {
            if (!d_body2.is_active) {
                continue;
            }
            
            if (&d_body == &d_body2) {
                continue;
            }
            
            bool is_equal_layer = (d_body.layer & d_body2.layer);
            bool has_intersect = d_body.aabb.has_intersection(d_body2.aabb);
            if (is_equal_layer && has_intersect) {
                d_body.contacted_bodyes.push_back(&d_body2);
            }
        }
    }
}


bool AABB::has_intersection(const AABB &aabb) const {
    if ((pos.x + size.x) < aabb.pos.x && (aabb.pos.x + aabb.size.x) < pos.x) {
        return false;
    }
    if ((pos.y + size.y) < aabb.pos.y && (aabb.pos.y + aabb.size.y) < pos.y) {
        return false;
    }
    return true;
}

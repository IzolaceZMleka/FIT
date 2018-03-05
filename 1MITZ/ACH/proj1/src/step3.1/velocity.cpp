/*
 * Architektura procesoru (ACH 2016)
 * Projekt c. 1 (nbody)
 * Login: xpavli78
 */

#include <cmath>
#include <cfloat>
#include "velocity.h"

/**
 * @breef   Funkce vypocte rychlost kterou teleso p1 ziska vlivem gravitace p2.
 * @details Viz zadani.pdf
 */
void calculate_both_velocities(
  const t_particle &p1,
  const t_particle &p2,
  t_velocity &vel
)
{
    float r, dx, dy, dz;
    float vx, vy, vz;
    
    dx = p2.pos_x - p1.pos_x;
    dy = p2.pos_y - p1.pos_y;
    dz = p2.pos_z - p1.pos_z;

    r = sqrt(dx*dx + dy*dy + dz*dz);
    
    if(r > COLLISION_DISTANCE)
    {
        float velocity = ((G * p1.weight * p2.weight / (r*r)) / p1.weight) * DT / r;
        
        vel.x += dx * velocity;
        vel.y += dy * velocity;
        vel.z += dz * velocity;
    }

    if (r > 0.0f && r < COLLISION_DISTANCE) {
        float dw = p1.weight - p2.weight;
        float totalw = p1.weight + p2.weight;
        
        vel.x += ((2 * p2.weight * p2.vel_x + dw * p1.vel_x) / totalw) - p1.vel_x;
        vel.y += ((2 * p2.weight * p2.vel_y + dw * p1.vel_y) / totalw) - p1.vel_y;
        vel.z += ((2 * p2.weight * p2.vel_z + dw * p1.vel_z) / totalw) - p1.vel_z;
    }
    
    
}

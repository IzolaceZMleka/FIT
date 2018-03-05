/*
 * Architektura procesoru (ACH 2016)
 * Projekt c. 1 (nbody)
 * Login: xpavli78
 */

#ifndef __NBODY_H__
#define __NBODY_H__

#include <cstdlib>
#include <cstdio>
#include "velocity.h"

struct t_particles {
    float pos_x[N];
    float pos_y[N];
    float pos_z[N];
    float vel_x[N];
    float vel_y[N];
    float vel_z[N];
    float weight[N];
};

struct t_velocities
{
    float x[N];
    float y[N];
    float z[N];
};

void particles_simulate(t_particles &p);

void particles_read(FILE *fp, t_particles &p);

void particles_write(FILE *fp, t_particles &p);

#endif /* __NBODY_H__ */

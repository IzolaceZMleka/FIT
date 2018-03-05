/*
 * Architektura procesoru (ACH 2017)
 * Projekt c. 2 (cuda)
 * Login: xpavli78
 */

#ifndef __NBODY_H__
#define __NBODY_H__

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <random>
#include <exception>
#include <string>

using namespace std;


/* gravitacni konstanta */
#define G 6.67384e-11f
#define COLLISION_DISTANCE 0.01f

/* struktura castic */
typedef struct
{
    float4 *pos;
    float4 *vel;
/* DOPLNTE: vhodnou datovou strukturu */ 
} t_particles;

/* pomocna struktura rychlosti */
typedef struct
{
    float4 *coords;
/* DOPLNTE: vhodnou datovou strukturu */ 
} t_velocities;


__global__ void calculate_velocity(t_particles p_in, t_particles p_out, int N, float dt);

void particles_read(FILE *fp, t_particles &p, int N);

void particles_write(FILE *fp, t_particles &p, int N);

#endif /* __NBODY_H__ */

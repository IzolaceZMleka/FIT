/*
 * Architektura procesoru (ACH 2016)
 * Projekt c. 1 (nbody)
 * Login: xpavli78
 */

#include "nbody.h"

void particles_simulate(t_particles &p)
{
    
    int i, j, k;

    t_velocities velocities;

    for (k = 0; k < STEPS; k++)
    {
            //vynulovani mezisouctu
        for (i = 0; i < N; i++)
        {
            velocities.x[i] = 0.0f;
            velocities.y[i] = 0.0f;
            velocities.z[i] = 0.0f;
        }
            //vypocet nove rychlosti
        for (i = 0; i < N; i++)
        {
            #pragma omp simd
            for (j = 0; j < N; j++)
            {
                __assume_aligned(&p, 64);
                
                t_particle p1 = { .pos_x = p.pos_x[j], .pos_y = p.pos_y[j], .pos_z = p.pos_z[j], .vel_x = p.vel_x[j], .vel_y = p.vel_y[j], .vel_z = p.vel_z[j], .weight = p.weight[j]};
                t_particle p2 = { .pos_x = p.pos_x[i], .pos_y = p.pos_y[i], .pos_z = p.pos_z[i], .vel_x = p.vel_x[i], .vel_y = p.vel_y[i], .vel_z = p.vel_z[i], .weight = p.weight[i]};
                
                float r, dx, dy, dz;
                float vx, vy, vz;
                
                dx = p2.pos_x - p1.pos_x;
                dy = p2.pos_y - p1.pos_y;
                dz = p2.pos_z - p1.pos_z;

                r = sqrt(dx*dx + dy*dy + dz*dz);
                
                if(r > COLLISION_DISTANCE)
                {
                    float velocity = ((((G * p1.weight * p2.weight) / (r*r)) / p1.weight) * DT) / r;
                    
                    velocities.x[j] += dx * velocity;
                    velocities.y[j] += dy * velocity;
                    velocities.z[j] += dz * velocity;
                }

                if (r > 0.0f && r < COLLISION_DISTANCE) {
                    float dw = p1.weight - p2.weight;
                    float totalw = p1.weight + p2.weight;
                    
                    velocities.x[j] += ((2 * p2.weight * p2.vel_x + dw * p1.vel_x) / totalw) - p1.vel_x;
                    velocities.y[j] += ((2 * p2.weight * p2.vel_y + dw * p1.vel_y) / totalw) - p1.vel_y;
                    velocities.z[j] += ((2 * p2.weight * p2.vel_z + dw * p1.vel_z) / totalw) - p1.vel_z;
                }

            }
        }
        
        
        
            //ulozeni rychlosti a posun castic
        for (i = 0; i < N; i++)
        {
            p.vel_x[i] += velocities.x[i];
            p.vel_y[i] += velocities.y[i];
            p.vel_z[i] += velocities.z[i];
            
            p.pos_x[i] += p.vel_x[i] * DT;
            p.pos_y[i] += p.vel_y[i] * DT;
            p.pos_z[i] += p.vel_z[i] * DT;
        }
    }
}


void particles_read(FILE *fp, t_particles &p)
{
    for (int i = 0; i < N; i++)
    {
        fscanf(fp, "%f %f %f %f %f %f %f \n",
            &p.pos_x[i], &p.pos_y[i], &p.pos_z[i],
            &p.vel_x[i], &p.vel_y[i], &p.vel_z[i],
            &p.weight[i]);
    }
}

void particles_write(FILE *fp, t_particles &p)
{
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "%10.10f %10.10f %10.10f %10.10f %10.10f %10.10f %10.10f \n",
            p.pos_x[i], p.pos_y[i], p.pos_z[i],
            p.vel_x[i], p.vel_y[i], p.vel_z[i],
            p.weight[i]);
    }
}

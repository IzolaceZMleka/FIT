/*
 * Architektura procesoru (ACH 2017)
 * Projekt c. 2 (cuda)
 * Login: xpavli78
 */

#include <cmath>
#include <cfloat>
#include "nbody.h"
#include <iostream>
#include <random>
#include <exception>
#include <string>
#include <cstdio>

using namespace std;

__global__ void calculate_gravitation_velocity(t_particles p, t_velocities tmp_vel, int N, float dt)
{
	/* DOPLNTE: telo funkce pro vypocet gravitacni rychlosti */
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
		float r;
/*
		if(i == 1)
			printf("%f %f %f %f %f %f %f %f xaxa\n", p.pos[i].x,p.pos[i].y,p.pos[i].z,p.pos[i].w,p.vel[i].x,p.vel[i].y,p.vel[i].z,p.vel[i].w);
*/
		for (int j = 0; j < N; j++) {
			float dx = p.pos[j].x - p.pos[i].x;
			float dy = p.pos[j].y - p.pos[i].y;
			float dz = p.pos[j].z - p.pos[i].z;

			r = sqrtf(dx*dx + dy*dy + dz*dz);
			float r3 = r * r * r;

			float velocity = (G * dt * p.pos[j].w) / r3;

			Fx += (r > COLLISION_DISTANCE) ? dx * velocity : 0.0f;
			Fy += (r > COLLISION_DISTANCE) ? dy * velocity : 0.0f;
			Fz += (r > COLLISION_DISTANCE) ? dz * velocity : 0.0f;
		}

		tmp_vel.coords[i].x += Fx;
		tmp_vel.coords[i].y += Fy;
		tmp_vel.coords[i].z += Fz;

	}
}

__global__ void calculate_collision_velocity(t_particles p, t_velocities tmp_vel, int N, float dt)
{
	/* DOPLNTE: telo funkce pro vypocet kolizni rychlosti */
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
		float r;

		for (int j = 0; j < N; j++) {
			float dx = p.pos[i].x - p.pos[j].x;
			float dy = p.pos[i].y - p.pos[j].y;
			float dz = p.pos[i].z - p.pos[j].z;

			r = sqrtf(dx*dx + dy*dy + dz*dz);

			float dw = p.pos[i].w - p.pos[j].w;
			float totalw = p.pos[i].w + p.pos[j].w;

			Fx += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2 * p.pos[j].w * p.vel[j].x + dw * p.vel[i].x) / totalw) - p.vel[i].x : 0.0f;
			Fy += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2 * p.pos[j].w * p.vel[j].y + dw * p.vel[i].y) / totalw) - p.vel[i].y : 0.0f;
			Fz += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2 * p.pos[j].w * p.vel[j].z + dw * p.vel[i].z) / totalw) - p.vel[i].z : 0.0f;
		}

		tmp_vel.coords[i].x += Fx;
		tmp_vel.coords[i].y += Fy;
		tmp_vel.coords[i].z += Fz;

	}

}

__global__ void update_particle(t_particles p, t_velocities tmp_vel, int N, float dt)
{
	/* DOPLNTE: telo funkce pro vypocet kolizni rychlosti */
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		p.vel[i].x += tmp_vel.coords[i].x;
		p.vel[i].y += tmp_vel.coords[i].y;
		p.vel[i].z += tmp_vel.coords[i].z;

		p.pos[i].x += p.vel[i].x * dt;
		p.pos[i].y += p.vel[i].y * dt;
		p.pos[i].z += p.vel[i].z * dt;

		tmp_vel.coords[i].x = 0.0f;
		tmp_vel.coords[i].y = 0.0f;
		tmp_vel.coords[i].z = 0.0f;
	}


}

__host__ void particles_read(FILE *fp, t_particles &p, int N)
{
	/* DOPLNTE: telo funkce pro nacitani castic */
	for (int i = 0; i < N; i++)
	{
		fscanf(fp, "%f %f %f %f %f %f %f \n",
				&p.pos[i].x, &p.pos[i].y, &p.pos[i].z,
				&p.vel[i].x, &p.vel[i].y, &p.vel[i].z,
				&p.pos[i].w);
	}
}

__host__  void particles_write(FILE *fp, t_particles &p, int N)
{
	for (int i = 0; i < N; i++)
	{
		fprintf(fp, "%10.10f %10.10f %10.10f %10.10f %10.10f %10.10f %10.10f \n",
				p.pos[i].x, p.pos[i].y, p.pos[i].z,
				p.vel[i].x, p.vel[i].y, p.vel[i].z,
				p.pos[i].w);
	}
	/* DOPLNTE: telo funkce pro ulozeni castic */
}

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

__global__ void calculate_velocity(t_particles p_in, t_particles p_out, int N, float dt)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
		float r;


		for (int j = 0; j < N; j++) {
			float dx = p_in.pos[i].x - p_in.pos[j].x;
			float dy = p_in.pos[i].y - p_in.pos[j].y;
			float dz = p_in.pos[i].z - p_in.pos[j].z;

			r = sqrtf(dx*dx + dy*dy + dz*dz);

			float dw = p_in.pos[i].w - p_in.pos[j].w ;
			float totalw = p_in.pos[i].w + p_in.pos[j].w ;
			float r3 = r * r * r;
			float velocity = (G * dt * p_in.pos[j].w * -1.0f) / r3;

			Fx += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * p_in.pos[j].w * p_in.vel[j].x + dw * p_in.vel[i].x) / totalw) - p_in.vel[i].x : 0.0f;
			Fy += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * p_in.pos[j].w * p_in.vel[j].y + dw * p_in.vel[i].y) / totalw) - p_in.vel[i].y : 0.0f;
			Fz += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * p_in.pos[j].w * p_in.vel[j].z + dw * p_in.vel[i].z) / totalw) - p_in.vel[i].z : 0.0f;



			Fx += (r > COLLISION_DISTANCE) ? dx * velocity : 0.0f;
			Fy += (r > COLLISION_DISTANCE) ? dy * velocity : 0.0f;
			Fz += (r > COLLISION_DISTANCE) ? dz * velocity : 0.0f;

		}

		Fx += p_in.vel[i].x;
		Fy += p_in.vel[i].y;
		Fz += p_in.vel[i].z;



		p_out.vel[i].x =  Fx;
		p_out.vel[i].y =  Fy;
		p_out.vel[i].z =  Fz;

		p_out.pos[i].x = (Fx * dt) + p_in.pos[i].x;
		p_out.pos[i].y = (Fy * dt) + p_in.pos[i].y;
		p_out.pos[i].z = (Fz * dt) + p_in.pos[i].z;

		//printf("%f %f %f %f %f %f %f %f xaxa\n", p_out.pos[i].x,p_out.pos[i].y,p_out.pos[i].z,p_out.pos[i].w,p_out.vel[i].x,p_out.vel[i].y,p_out.vel[i].z,p_out.vel[i].w);

	}
}

/*
__global__ void calculate_velocity(const t_particles p_in, t_particles p_out, const int N, const float dt)
{

	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		float Fx = p_in.vel[i].x; float Fy = p_in.vel[i].y; float Fz = p_in.vel[i].z;
		float r;

		for (int j = 0; j < N; j++) {
			float dx = p_in.pos[i].x - p_in.pos[j].x;
			float dy = p_in.pos[i].y - p_in.pos[j].y;
			float dz = p_in.pos[i].z - p_in.pos[j].z;

			r = sqrtf(dx*dx + dy*dy + dz*dz);
			float r3 = r * r * r;

			float velocity = (G * dt * p_in.pos[j].w -1.0f) / r3;

			Fx += (r > COLLISION_DISTANCE) ? dx * velocity : 0.0f;
			Fy += (r > COLLISION_DISTANCE) ? dy * velocity : 0.0f;
			Fz += (r > COLLISION_DISTANCE) ? dz * velocity : 0.0f;

			float dw = p_in.pos[i].w - p_in.pos[j].w;
			float totalw = p_in.pos[i].w + p_in.pos[j].w;

			Fx += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2 * p_in.pos[j].w * p_in.vel[j].x + dw * p_in.vel[i].x) / totalw) - p_in.vel[i].x : 0.0f;
			Fy += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2 * p_in.pos[j].w * p_in.vel[j].y + dw * p_in.vel[i].y) / totalw) - p_in.vel[i].y : 0.0f;
			Fz += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2 * p_in.pos[j].w * p_in.vel[j].z + dw * p_in.vel[i].z) / totalw) - p_in.vel[i].z : 0.0f;
		}

		p_out.vel[i].x = Fx;
		p_out.vel[i].y = Fy;
		p_out.vel[i].z = Fz;

		p_out.pos[i].x = Fx * dt + p_in.pos[i].x;
		p_out.pos[i].y = Fy * dt + p_in.pos[i].y;
		p_out.pos[i].z = Fz * dt + p_in.pos[i].z;
	}

}
 */
/*
__global__ void calculate_velocity(t_particles p_in, t_particles p_out, int N, float dt)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {

		float Fx = p_in.vel[i].x; float Fy = p_in.vel[i].y; float Fz = p_in.vel[i].z;
		float r;

		for (int j = 0; j < N; j++) {

			float dx = p_in.pos[i].x - p_in.pos[j].x;
			float dy = p_in.pos[i].y - p_in.pos[j].y;
			float dz = p_in.pos[i].z - p_in.pos[j].z;

			float dw = p_in.pos[i].w - p_in.pos[j].w ;
			float totalw = p_in.pos[i].w + p_in.pos[j].w ;

			r = sqrtf(dx*dx + dy*dy + dz*dz);
			float r3 = r * r * r;
			float velocity = (G * dt * p_in.pos[j].w * -1.0f) / r3;

			Fx += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * p_in.pos[j].w * p_in.vel[j].x + dw * p_in.vel[i].x) / totalw) - p_in.vel[i].x : 0.0f;
			Fy += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * p_in.pos[j].w * p_in.vel[j].y + dw * p_in.vel[i].y) / totalw) - p_in.vel[i].y : 0.0f;
			Fz += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * p_in.pos[j].w * p_in.vel[j].z + dw * p_in.vel[i].z) / totalw) - p_in.vel[i].z : 0.0f;

			Fx += (r > COLLISION_DISTANCE) ? dx * velocity : 0.0f;
			Fy += (r > COLLISION_DISTANCE) ? dy * velocity : 0.0f;
			Fz += (r > COLLISION_DISTANCE) ? dz * velocity : 0.0f;
		}


		p_out.vel[i].x =  Fx;
		p_out.vel[i].y =  Fy;
		p_out.vel[i].z =  Fz;

		p_out.pos[i].x = (Fx * dt) + p_in.pos[i].x;
		p_out.pos[i].y = (Fy * dt) + p_in.pos[i].y;
		p_out.pos[i].z = (Fz * dt) + p_in.pos[i].z;

	}
	//printf("%f %f %f %f %f %f %f %f xaxa\n", p_out.pos[i].x,p_out.pos[i].y,p_out.pos[i].z,p_out.pos[i].w,p_out.vel[i].x,p_out.vel[i].y,p_out.vel[i].z,p_out.vel[i].w);

}
 */


/*
__global__ void calculate_velocity(t_particles p_in, t_particles p_out, int N, float dt)
{
	int i = blockDim.x * blockIdx.x + threadIdx.x;

	if (i < N) {
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
		float r;

		float px = p_in.pos[i].x;
		float py = p_in.pos[i].y;
		float pz = p_in.pos[i].z;

		float pvx = p_in.vel[i].x;
		float pvy = p_in.vel[i].y;
		float pvz = p_in.vel[i].z;

		float pw = p_in.pos[i].w;

		for (int j = 0; j < N; j++) {
			float dx = px - p_in.pos[j].x;
			float dy = py - p_in.pos[j].y;
			float dz = pz - p_in.pos[j].z;

			float pwi = p_in.pos[j].w;

			r = sqrtf(dx*dx + dy*dy + dz*dz);

			if(r > 0.0f && r < COLLISION_DISTANCE)
			{
				float dw = pw - pwi ;
				float totalw = pw + pwi ;

				Fx += ((2.0f * pwi * p_in.vel[j].x + dw * pvx) / totalw) - pvx;
				Fy += ((2.0f * pwi * p_in.vel[j].y + dw * pvy) / totalw) - pvy;
				Fz += ((2.0f * pwi * p_in.vel[j].z + dw * pvz) / totalw) - pvz;

			}
			else
			{
				float r3 = r * r * r;
				float velocity = (G * dt * pwi * -1.0f) / r3;

				Fx += (r > COLLISION_DISTANCE) ? dx * velocity : 0.0f;
				Fy += (r > COLLISION_DISTANCE) ? dy * velocity : 0.0f;
				Fz += (r > COLLISION_DISTANCE) ? dz * velocity : 0.0f;
			}

		}

		Fx += pvx;
		Fy += pvy;
		Fz += pvz;



		p_out.vel[i].x =  Fx;
		p_out.vel[i].y =  Fy;
		p_out.vel[i].z =  Fz;

		p_out.pos[i].x = (Fx * dt) + px;
		p_out.pos[i].y = (Fy * dt) + py;
		p_out.pos[i].z = (Fz * dt) + pz;

		//printf("%f %f %f %f %f %f %f %f xaxa\n", p_out.pos[i].x,p_out.pos[i].y,p_out.pos[i].z,p_out.pos[i].w,p_out.vel[i].x,p_out.vel[i].y,p_out.vel[i].z,p_out.vel[i].w);

	}
}
 */
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

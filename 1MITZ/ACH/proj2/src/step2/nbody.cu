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
        //sdílená paměť velikost 2x počet vláken * sizeof(float4)
	extern __shared__ float4 sharedP[];

	if (i < N) {
		float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
		float r;
                //soubezna iterace pres vsechna vlakna
		for(int k = 0; k < N/blockDim.x; k++)
		{
                        //zjisteni indexu v ramci globalni pameti
			int index = (threadIdx.x + k * blockDim.x);
                        //nahrani do sdílené paměti, index * 2, na index + 1 se nachází druhý float4
			sharedP[threadIdx.x*2] = p_in.pos[index];
			sharedP[threadIdx.x*2 + 1] = p_in.vel[index];
                        //synchronizace, aby nedošlo k přístupu k neincializovaným datům
			__syncthreads();
                        // prrubezny vypocet nad prave nahranym usekem odpovidajici poctu vlaken do globalni pameti
                        //vypocet jednotlivych vlaken, kde vsechny "druhe" castice jsou nahrazen jejich vyskytem v sdilene pameti
			for (int j = 0; j < blockDim.x; j++) {
				float dx = p_in.pos[i].x - sharedP[j*2].x;
				float dy = p_in.pos[i].y - sharedP[j*2].y;
				float dz = p_in.pos[i].z - sharedP[j*2].z;

				r = sqrtf(dx*dx + dy*dy + dz*dz);

				float dw = p_in.pos[i].w - sharedP[j*2].w ;
				float totalw = p_in.pos[i].w + sharedP[j*2].w ;
				float r3 = r * r * r;
				float velocity = (G * dt * sharedP[j*2].w * -1.0f) / r3;

				Fx += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * sharedP[j*2].w * sharedP[j*2 ].x + dw * p_in.vel[i].x) / totalw) - p_in.vel[i].x : 0.0f;
				Fy += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * sharedP[j*2].w * sharedP[j*2 ].y + dw * p_in.vel[i].y) / totalw) - p_in.vel[i].y : 0.0f;
				Fz += (r > 0.0f && r < COLLISION_DISTANCE) ? ((2.0f * sharedP[j*2].w * sharedP[j*2 ].z + dw * p_in.vel[i].z) / totalw) - p_in.vel[i].z : 0.0f;

				Fx += (r > COLLISION_DISTANCE) ? dx * velocity : 0.0f;
				Fy += (r > COLLISION_DISTANCE) ? dy * velocity : 0.0f;
				Fz += (r > COLLISION_DISTANCE) ? dz * velocity : 0.0f;

			}
                        //synchronizace po ukonceni vypoctu, aby nedoslo k drivejsimu nahrani do sdilene pameti
			__syncthreads();

		}
                //aktualizace pozice a rychlosti castice 
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

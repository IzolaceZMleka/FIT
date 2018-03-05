/*
 * Architektura procesoru (ACH 2017)
 * Projekt c. 2 (cuda)
 * Login: xpavli78
 */

#include <sys/time.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "nbody.h"

// Makro pre kontrolu chyb CUDA kniznice
#define cudaCheckError() __cudaCheckError( __FILE__, __LINE__ )

// Kontrola chyb CUDA
inline void __cudaCheckError( const char *file, const int line )
{
  using namespace std;
  cudaError err = cudaGetLastError();
  if ( cudaSuccess != err )
  {
    cerr<<"cudaCheckError() failed at "<<file<<":"<<line<<" : "<<cudaGetErrorString( err )<<endl;
    exit( -1 );
  }
}

int main(int argc, char **argv)
{
    FILE *fp;
    struct timeval t1, t2;
    int N;
    float dt;
    int steps;
    int thr_blc;

    // parametry
    if (argc != 7)
    {
        printf("Usage: nbody <N> <dt> <steps> <thr/blc> <input> <output>\n");
        exit(1);
    }
    N = atoi(argv[1]);
    dt = atof(argv[2]);
    steps = atoi(argv[3]);
    thr_blc = atoi(argv[4]);

    printf("N: %d\n", N);
    printf("dt: %f\n", dt);
    printf("steps: %d\n", steps);
    printf("threads/block: %d\n", thr_blc);

    // alokace pameti na CPU
    t_particles particles_cpu;
    size_t partArr = N*sizeof(float4);

    cudaHostAlloc(&particles_cpu.pos, partArr, cudaHostAllocDefault);
    cudaHostAlloc(&particles_cpu.vel, partArr, cudaHostAllocDefault);
    cudaCheckError();

    // nacteni castic ze souboru
    fp = fopen(argv[5], "r");
    if (fp == NULL)
    {
        printf("Can't open file %s!\n", argv[5]);
        exit(1);
    }
    particles_read(fp, particles_cpu, N);
    fclose(fp);

    t_particles particles_gpu1;
    t_particles particles_gpu2;

    /* DOPLNTE: alokaci pameti na GPU */
    cudaMalloc(&particles_gpu1.pos, partArr);
    cudaMalloc(&particles_gpu1.vel, partArr);
    cudaMalloc(&particles_gpu2.pos, partArr);
    cudaMalloc(&particles_gpu2.vel, partArr);
    cudaCheckError();

    /* DOPLNTE: kopirovani pameti na GPU */

    cudaMemcpy(particles_gpu1.pos, particles_cpu.pos, partArr, cudaMemcpyHostToDevice);
    cudaMemcpy(particles_gpu1.vel, particles_cpu.vel, partArr, cudaMemcpyHostToDevice);
    cudaMemcpy(particles_gpu2.pos, particles_cpu.pos, partArr, cudaMemcpyHostToDevice);
    cudaMemcpy(particles_gpu2.vel, particles_cpu.vel, partArr, cudaMemcpyHostToDevice);
    cudaCheckError();

    /* DOPLNTE: vypocet velikosti vypocetniho gridu */
    size_t grid = (N + thr_blc - 1) / thr_blc;;


    // vypocet
    gettimeofday(&t1, 0);

    for (int s = 0; s < steps; ++s)
    {
        calculate_velocity<<<grid, thr_blc>>>(particles_gpu1, particles_gpu2, N, dt);
        swap(particles_gpu1, particles_gpu2);
    }
    cudaDeviceSynchronize();
    gettimeofday(&t2, 0);

    // cas
    double t = (1000000.0 * (t2.tv_sec - t1.tv_sec) + t2.tv_usec - t1.tv_usec) / 1000000.0;
    printf("Time: %f s\n", t);

    /* DOPLNTE: koopirovani castic spatky na CPU */

    cudaMemcpy(particles_cpu.pos,  particles_gpu1.pos,  partArr, cudaMemcpyDeviceToHost);
    cudaMemcpy(particles_cpu.vel,  particles_gpu1.vel,  partArr, cudaMemcpyDeviceToHost);

    cudaCheckError();

    // ulozeni castic do souboru
    fp = fopen(argv[6], "w");
    if (fp == NULL)
    {
        printf("Can't open file %s!\n", argv[6]);
        exit(1);
    }
    particles_write(fp, particles_cpu, N);
    fclose(fp);

    /* dealokaci pameti na CPU */
	if(particles_cpu.pos)  cudaFree(particles_cpu.pos);
	if(particles_cpu.vel)  cudaFree(particles_cpu.vel);
	if(particles_gpu1.pos)  cudaFree(particles_gpu1.pos);
	if(particles_gpu1.vel)  cudaFree(particles_gpu1.vel);
	if(particles_gpu2.pos)  cudaFree(particles_gpu2.pos);
	if(particles_gpu2.vel)  cudaFree(particles_gpu2.vel);

    return 0;
}


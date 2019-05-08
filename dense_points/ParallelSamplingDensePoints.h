//NUEVO REPO
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <vector>
#include <list>
#include <math.h>
#include <iostream>
#include "sample.h"

#define N_THREADS 2
#define WIDTH 500
#define HEIGHT 500
#define L 4000

using namespace std;

mutex mutex_;

int numberPoints;
double r = 5;
double size_ = r/sqrt(2);
int cols = WIDTH/size_;
int fils = HEIGHT/size_;


vector<Sample*> cloud(L); //dense points cloud
vector<int> pointIndex(L);
vector<vector<Sample*>> actives;//
vector<vector<Sample*>> idles;  //
vector<list<Sample*>> grid(cols*fils);   //inicializar el grid !!! esta sera la respuesta

void parallel_generateDP(int ini, int fin) {
    for (int i = ini; i < fin; ++i) {
        Sample* sample = new Sample();
        sample->pos[0] = rand()%(WIDTH-1);  //numeros aleatorios entre 0 y 500
        sample->pos[1] = rand()%(HEIGHT-1);
        sample->status = "IDLE";
      //  printf("x : %d y : %d\n", sample->pos[0], sample->pos[1]);
        mutex_.lock();
        cloud[i] = sample;
        //agregar al grid cell correspondiente
        int ii = sample->pos[0]/size_;
        int j = sample->pos[1]/size_;
        int id_grid_cell = ii + j * cols;
       // printf("id : %d cell: %d \n",data1->id_t, id_grid_cell);
        grid[id_grid_cell].push_back(sample);
        pointIndex[i] = i;
        mutex_.unlock();
    }
};

class ParallelSamplingDensePoints {
    
    thread hilos[N_THREADS];
    
public:
    void generateDensePoints() {
        for (int i = 0; i < N_THREADS; ++i) {
            int ini = i * (L/2);
            int fin = (i+1) * (L/2);
            hilos[i] = thread(parallel_generateDP, ini, fin);
        }
        
        for(int i = 0; i < N_THREADS; ++i) {
            hilos[i].join();
        }
        
    //    printf("puntos en la nube : %lu\n",cloud.size());
    
        //int count = 0;
        std::cout<<L<<'\n';
        for(auto i : grid){
            for(auto j : i){
                std::cout<<j->pos[0]<<" "<<j->pos[1]<<std::endl;
                //++count;
            }
        }
    }
};

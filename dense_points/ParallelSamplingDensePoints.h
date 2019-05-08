//NUEVO REPO
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
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

pthread_mutex_t mutex1;

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




struct parallel_data {
    int ini;
    int fin;
    int id_t;
};

void* parallel_generateDP(void* arg) {
    
    parallel_data* data1 = (parallel_data*) arg;
    
    for (int i = data1->ini; i < data1->fin; ++i) {
        
        Sample* sample = new Sample();
        sample->pos[0] = rand()%(WIDTH-1);  //numeros aleatorios entre 0 y 500
        sample->pos[1] = rand()%(HEIGHT-1);
        sample->status = "IDLE";
      //  printf("x : %d y : %d\n", sample->pos[0], sample->pos[1]);
        
        pthread_mutex_lock(&mutex1);
        cloud[i] = sample;
        
        //agregar al grid cell correspondiente
        int ii = sample->pos[0]/size_;
        int j = sample->pos[1]/size_;
        int id_grid_cell = ii + j * cols;
       // printf("id : %d cell: %d \n",data1->id_t, id_grid_cell);
        grid[id_grid_cell].push_back(sample);
        
        pointIndex[i] = i;
        
        pthread_mutex_unlock(&mutex1);
    }
    
    return NULL;
};

class ParallelSamplingDensePoints {
    
    pthread_t hilos[N_THREADS];
    
public:
    void generateDensePoints() {
    
        for (int i = 0; i < N_THREADS; ++i) {
            parallel_data* args = new parallel_data();
            args->ini = i * (L/2);
            args->fin = (i+1) * (L/2);
            args->id_t = i;
            pthread_create(&hilos[i], NULL, parallel_generateDP, (void*) args);
        }
        
        for(int i = 0; i < N_THREADS; ++i) {
            pthread_join(hilos[i], NULL);
        }
        
    //    printf("puntos en la nube : %lu\n",cloud.size());
    
    
    std::cout<<L<<'\n';
    for(auto &i : grid){
        for(auto &j : i){
            std::cout<<j->pos[0]<<" "<<j->pos[1]<<std::endl;
        }
    }
    }

};

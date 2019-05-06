#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include "sample.h"

#define N_THREADS 2
#define WIDTH 500
#define HEIGHT 500

#include <vector>

using namespace std;

int numberPoints;
double r = 5;
double size = r/sqrt(2);
int cols = WIDTH/size;
int fils = HEIGHT/size;


vector<Sample*> cloud; //dense points cloud
vector<int> pointIndex;
vector<vector<Sample*>> actives;//
vector<vector<Sample*>> idles;  //
vector<vector<Sample*>> grid(cols*fils);   //inicializar el grid !!! esta sera la respuesta


struct parallel_data {
    int ini;
    int fin;
    int id_t;
};
void* parallel_generateDP(void* arg) {
    
    parallel_data* data1 = (parallel_data*) arg;
    
    for (int i = data1->ini; i < data1->fin; ++i) {
        Sample* sample = new Sample();
        sample->pos[0] = rand()%WIDTH;  //numeros aleatorios entre 0 y 500
        sample->pos[1] = rand()%HEIGHT;
        sample->status = "IDLE";
        printf("x : %d y : %d\n", sample->pos[0], sample->pos[1]);
        cloud.push_back(sample);
        //agregar al grid cell correspondiente
        int ii = sample->pos[0]/size;
        int j = sample->pos[1]/size;
        int id_grid_cell = ii + j * cols;
        printf("id : %d cell: %d \n",data1->id_t, id_grid_cell);
        grid[id_grid_cell].push_back(sample);
        
    }
    
    return NULL;
};

class ParallelSamplingDensePoints {
    
    pthread_t hilos[N_THREADS];
    
    
    
public:
    void generateDensePoints() {
        
        for (int i = 0; i < N_THREADS; ++i) {
            parallel_data* args = new parallel_data();
            args->ini = i * 250;;
            args->fin = (i+1) * 250;
            args->id_t = i;
            pthread_create(&hilos[i], NULL, parallel_generateDP, (void*) args);
        }
        
        for(int i = 0; i < N_THREADS; ++i) {
            pthread_join(hilos[i], NULL);
        }
        
    }
    
    
    void checkStatus(Sample* sample){
        
    }
};

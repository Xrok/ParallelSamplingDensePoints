//NUEVO REPO
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


class ParallelSamplingDensePoints {
    
    pthread_t hilos[N_THREADS];
    
    
    
public:
    
};

//NUEVO REPO
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <thread>
#include <limits>
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
const double r = 5;
const double size_ = r / sqrt(2);
const int cols = WIDTH / size_;
const int fils = HEIGHT / size_;

vector<Sample *> cloud(L); //dense points cloud
vector<int> pointIndex(L);
//vector<vector<Sample*>> actives;//
//vector<vector<Sample*>> idles;  //
vector<list<Sample *>> grid(cols *fils); //inicializar el grid !

void parallel_generateDP(int ini, int fin)
{
    for (int i = ini; i < fin; ++i)
    {
        Sample *sample = new Sample();
        sample->pos[0] = rand() % (WIDTH - 1); //numeros aleatorios entre 0 y 500
        sample->pos[1] = rand() % (HEIGHT - 1);
        sample->status = "IDLE";
        //  printf("x : %d y : %d\n", sample->pos[0], sample->pos[1]);
        mutex_.lock();
        cloud[i] = sample;
        //agregar al grid cell correspondiente
        int ii = sample->pos[0] / size_;
        int j = sample->pos[1] / size_;
        int id_grid_cell = ii + j * cols;
        // printf("id : %d cell: %d \n",data1->id_t, id_grid_cell);
        grid[id_grid_cell].push_back(sample);
        pointIndex[i] = i;
        mutex_.unlock();
    }
};

void parallel_run(int i, Sample **S)
{
    int inicio = (i - 1 / N_THREADS) * L;
    int fin = (i / N_THREADS) * L;
    int curr_index = inicio;
    do
    {
        while (cloud[pointIndex[curr_index]]->status != "IDLE")
        {
            curr_index++;
            if (curr_index == fin)
                break;
        }
        Sample *sample = cloud[pointIndex[curr_index]];
        sample->status = "ACTIVE";
        sample->prioridad = (rand() * N_THREADS + i) / numeric_limits<int>::max();

        //LLAMAR DETECT COLLISION

        //CHECK STATUS

        if (sample->status == "ACCEPTED")
        {
            mutex_.lock();
            S[sample->id_grid_cell] = sample;
            mutex_.unlock();

            for (Sample *i : sample->I)
            {
                mutex_.lock();
                i->status = "REJECTED";
                mutex_.unlock();
            }
        }
    } while (true); //FUNCION QUE NOS DIGA CUANTOS IDDLE points QUEDA
}

class ParallelSamplingDensePoints
{

    thread hilos[N_THREADS];
    Sample **S = new Sample *[cols * fils];

public:
    void generateDensePoints()
    {
        for (int i = 0; i < N_THREADS; ++i)
        {
            int ini = i * (L / 2);
            int fin = (i + 1) * (L / 2);
            hilos[i] = thread(parallel_generateDP, ini, fin);
        }

        for (int i = 0; i < N_THREADS; ++i)
        {
            hilos[i].join();
        }

        //printf("puntos en la nube : %lu\n",cloud.size());
        random_shuffle(pointIndex.begin(), pointIndex.end());
        //int count = 0;
        //        std::cout<<L<<'\n';
        //        for(auto i : grid){
        //            for(auto j : i){
        //                std::cout<<j->pos[0]<<" "<<j->pos[1]<<std::endl;
        //                //++count;
        //            }
        //        }
    }

    void detectCollision(Sample *p, int r)
    {
        int col = ((p->pos[0]) / size_);
        int row = ((p->pos[1]) / size_);

        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                int position = (col + i) + (row + j) * cols;
                if (position >= 0 && position < cols * fils)
                {
                    std::list<Sample *> neighbour = grid[position];
                    for (auto &sample : neighbour)
                    {
                        if (sample->status == "IDLE")
                        {
                            p->I.push_back(sample);
                        }
                        else if (sample->status == "ACTIVE")
                        {
                            p->A.push_back(sample);
                        }
                    }
                }
            }
        }
        for (auto &samples : grid[col + row * fils])
        {
            if (samples->pos[0] != p->pos[0] && samples->pos[1] != p->pos[1])
            {
                if (samples->status == "IDLE")
                {
                    p->I.push_back(samples);
                }
                else if (samples->status == "ACTIVE")
                {
                    p->A.push_back(samples);
                }
            }
        }
    }

    void checkStatus(Sample *pi)//pi as argument
    {
        mutex_.lock();
        if(pi->status != "ACTIVE"){//atomic
            mutex_.unlock();
            return ;
        }
        mutex_.unlock();
        for (auto &q :pi->A) {
            if(q->prioridad > pi->prioridad){
                checkStatus(q);
                mutex_.lock();
                if (q->status=="ACCEPTED"){//atomic
                    pi->status= "REJECTED";
                    mutex_.unlock();
                    return;
                }
                mutex_.unlock();
            }
        }
        mutex_.lock();
        pi->status="ACCEPTED";//atomic
        mutex_.unlock();
        return;
    }


    void run()
    {
        for (int i = 0; i < N_THREADS; ++i)
        {
            hilos[i] = thread(parallel_run, i + 1, S);
        }
        for (int i = 0; i < N_THREADS; ++i)
        {
            hilos[i].join();
        }
    }
};

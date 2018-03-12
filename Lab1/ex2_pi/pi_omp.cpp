#include "pi.h"

#define SIZE 10000l
#define CHUNKSIZE 100
//#define OMP_NUM_THREADS 100
#define MAX_THREADS 100
double *rect, *midPt, *area;
int num_threads =1;
/* TODO */
/* Modify this function to implement a parallel version with openmp */
int compute(long int num_steps)
{
    int count [MAX_THREADS] = {0};
    double pi;
    int i =0;
    double width = 2. / num_steps;
    pi = 0.;
    int chunk = CHUNKSIZE;
    omp_set_num_threads(num_threads);
    #pragma omp parallel shared(rect, midPt, area, width) private(i)
    {
        #pragma omp for schedule(dynamic, chunk) nowait
        for ( i = 0; i < num_steps; ++i) {
            // int tid = omp_get_thread_num();
            // count[tid] ++;
            rect[i] = (double)i;
            midPt[i] = (i + 0.5) * width - 1.0;
            area[i] = sqrt(1.0 - midPt[i] * midPt[i]) * width;
        } 

    }

    // for (int i = 0; i < MAX_THREADS; ++i)
    // {
    //     if(count[i]>0)
    //         printf("Thread %d has %d loops\n",i, count[i] );
    // }

    for ( i = 0; i < num_steps; ++i)
    {
        pi += area[i] * 2.0;
    }


    std::cout << "PI:" << pi << std::endl;

    return (0);
}

int prepare(long int Count)
{
    int i, j, n = Count;
    rect = new double[Count];
    midPt = new double[Count];
    area = new double[Count];

    return (0);
}

int cleanup(long int N)
{
    delete rect;
    delete midPt;
    delete area;
    return (0);
}

int main(int argc, char *argv[])
{
    long int Count = SIZE;
    int Error;

    if(argc >= 2){
        Count = std::atoi(argv[1]);
        if (Count <= 0)
        {
                    std::cerr << "Invalid argument" << std::endl;
                    std::cerr << "Usage: " << argv[0] << "N" << std::endl;
                    std::cerr << "       N = size" << std::endl;
                    return 1;
        }
        if(argc >=3) {
            num_threads = std::atoi(argv[2]);
            if(num_threads <= 0 ){
                std::cerr << "Invalid argument" << std::endl;
                    std::cerr << "Usage: " << argv[0] << "<N> <M>" << std::endl;
                    std::cerr << "       N = size" << std::endl;
                    std::cerr << "       M = number of threads" << std::endl;
                    return 1;
            }
        }
    }

    std::cout << "counts:" << Count << std::endl;
    std::cout << "Thread:" << num_threads << std::endl;
    std::cout << "preparation starting" << std::endl;
    if (Error = prepare(Count) != 0)
        return Error;
    std::cout << "preparation done" << std::endl;
    unsigned long long start_ticks = my_getticks();
    Error = compute(Count);

    unsigned long long end_ticks = my_getticks();
    unsigned long long ticks = end_ticks - start_ticks;

    if (Error == 0)
        std::cout << "succeeded in ";
    else
        std::cout << "failed in ";
    std::cout << my_ticks_to_seconds(ticks) << " seconds." << std::endl;
    std::cout << "starting cleanup" << std::endl;
    return cleanup(Count);
}
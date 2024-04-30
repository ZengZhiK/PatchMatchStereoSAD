#include<iostream>
#include<omp.h>
#include <time.h>
#include <windows.h>
#include <math.h>
// #include <unistd.h> linux
using namespace std;

/*
    测试样例：线性方程 y=w*x+b
*/
#define LINEAR_LEN 1024*1024

int main() {

    // 1. 创建内存空间
    float *y = new float[LINEAR_LEN];
    float *x = new float[LINEAR_LEN];
    float *w = new float[LINEAR_LEN];
    float *b = new float[LINEAR_LEN];

    // 2.数据初始化
    for (int idx = 0; idx < LINEAR_LEN; idx++) {
        y[idx] = 0.0f;
        x[idx] = idx * 1.0f;
        x[idx] = idx * 1.0f;
        x[idx] = idx * 1.0f;
    }

    // 3.标量计算

    double start = omp_get_wtime();
    Sleep(1000);
    double end = omp_get_wtime();
    cout << "test time : " << double(end - start) << "s" << endl;

    double tm_start = omp_get_wtime();
    for (int idx = 0; idx < LINEAR_LEN; idx++) {
        y[idx] = sin(w[idx] * x[idx] + b[idx]);
    }
    double tm_end = omp_get_wtime();
    cout << "thread 0 cost time : " << double(tm_end - tm_start) << endl;

    //4.多线程omp
    int phy_procs = omp_get_max_threads();
    cout << "threads : " << phy_procs << endl;

    tm_start = omp_get_wtime();
    //#pragma omp parallel for num_threads(phy_procs)
#pragma omp parallel for
    for (int idx = 0; idx < LINEAR_LEN; idx++) {
        //printf("thread %d  %d \n",omp_get_thread_num(), idx);
        y[idx] = sin(w[idx] * x[idx] + b[idx]);
    }
    tm_end = omp_get_wtime();
    cout << "mul-thread cost time : " << double(tm_end - tm_start) << endl;



    // 4.释放内存
    delete[]y;
    delete[]x;
    delete[]w;
    delete[]b;

    return 0;
}
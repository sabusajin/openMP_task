#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif
void merge_sort(int *arr, int left, int right);
void merge(int *arr, int left, int right);


int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }

  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int * arr = new int [atoi(argv[1])];

  generateMergeSortData (arr, atoi(argv[1]));

  //write code here
  long int n = atol (argv[1]);
  int nbthreads = atoi (argv[2]);
  generateMergeSortData (arr, atoi(argv[1]));
  omp_set_num_threads(nbthreads);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  merge_sort(arr, 0, n-1);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, atoi(argv[1]));

  delete[] arr;

  return 0;
}
void merge_sort(int *arr, int left, int right)
{
  if (left < right){
    int middle = (left+(right-left)/2);
    #pragma omp parallel
    {
      #pragma omp single
      {
        #pragma omp task
        merge_sort(arr, left, middle);

        #pragma omp task
        merge_sort(arr, middle+1, right);
      }
    }
  }

   merge(arr, left, right);
}
void merge(int *arr, int left, int right)
{
    int middle = (left+(right-left)/2);
    int * temp = new int[right-left+1];
    int n1 = left;
    int n2 = middle+1;
    int i = 0;

    while(n1 <= middle && n2 <= right) {
        if (arr[n1] < arr[n2]) {
            temp[i] = arr[n1];
            i++;
            n1++;
        }
        else {
            temp[i] = arr[n2];
            i++;
            n2++;
        }
    }

    //Add elements from left half
    while(n1 <= middle) {
        temp[i] = arr[n1];
        i++;
        n1++;
    }

    //add elements from right half
    while(n2 <= right) {
        temp[i] = arr[n2];
        i++;
        n2++;
    }

    //update main array
    for(int i = left; i <= right; i++) {
        arr[i] = temp[i - left];
    }

    delete[] temp;
}

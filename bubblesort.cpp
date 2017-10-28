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
void bubble_sort(int *arr, long int n);


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
  bubble_sort(arr, n);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  checkMergeSortResult (arr, atoi(argv[1]));

  delete[] arr;

  return 0;
}

void bubble_sort(int *arr, long int n) {
  for (int i = 0; i<n;i++) {
    int k = i%2;
    #pragma omp parallel for default(none),shared(arr,k, n)
    for (int j = k; j<n-1; j+=2) {
      if (arr[j] > arr[j+1]){
        std::swap(arr[j], arr[j+1]);
      }
    }
  }
}

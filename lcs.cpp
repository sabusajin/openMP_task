#include <omp.h>
#include <iostream>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);


#ifdef __cplusplus
}
#endif

using namespace std;

void lcs_task(int **lcs, std::string str1, std::string str2, int i, int j, int k);

int compute_lcs (int **lcs, std::string str1, std::string str2);




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

  if (argc < 2) {
    std::cerr<<"Usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
    return -1;
  }
  std::ifstream ifs("inp1.txt");
  std::string str1;
  str1.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
  std::ifstream ifs2("inp2.txt");
  std::string str2;
  str2.assign( (std::istreambuf_iterator<char>(ifs2) ),
                              (std::istreambuf_iterator<char>()    ) );

  str1.erase(std::remove(str1.begin(), str1.end(), '\n'), str1.end());
  str2.erase(std::remove(str2.begin(), str2.end(), '\n'), str2.end());
  int nbthreads = atoi (argv[1]);
  omp_set_num_threads(nbthreads);
  int row = str1.length();
  int column = str2.length();

  int length = 0;

  int **lcs;

  lcs = new int*[row+1];
  for (int i=0; i<=row; i++)
    lcs[i] = new int[column+1];

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  length = compute_lcs (lcs, str1, str2);
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cerr<<elapsed_seconds.count()<<std::endl;

  std::cout<<length<<std::endl;

  delete[] lcs;

  return 0;
}

int compute_lcs (int **lcs, std::string str1, std::string str2) {
  #pragma omp parallel
  {
    #pragma omp single
    {
      for (int i = 0, j = 0; i <= str1.length() && j <= str2.length(); j++){

	      int diagonal = std::min (j, (int) str1.length() - i);

	      for(int k=0; k<=diagonal; k++){
          #pragma omp task
		      lcs_task(lcs,str1,str2,i,j,k);
	      }
        #pragma omp taskwait
	      if(j>=str2.length()) {
          j = str2.length()-1;
          i++;
        }
    }
    }
  }
  return lcs[str1.length()][str2.length()];
}

void lcs_task(int **lcs, std::string str1, std::string str2, int i, int j, int k){
    int t_i = i + k;
    int t_j = j - k;
			if (t_i == 0 || t_j == 0)
				lcs[t_i][t_j] = 0;

			else if (str1[t_i - 1] == str2[t_j - 1])
				lcs[t_i][t_j] = lcs[t_i - 1][t_j - 1] + 1;

			else
				lcs[t_i][t_j] = max(lcs[t_i - 1][t_j], lcs[t_i][t_j - 1]);

}

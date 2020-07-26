#include <iostream>
#include <fstream>
#include <string>

#define pin std::pair<int,int>

int main(int argc, char** argv){


  std::string ifname = argv[1];
  std::string ofname = argv[2];
  
  std::ifstream help_read(ifname);

  int no_lines = -1; //Also number of nonzeros in the tensor

  int i_max = 0; //Length of mode1
  int j_max = 0; //Length of mode2
  int k_max = 0; //Length of mode3
  

  int i_val, j_val, k_val, val;

  while(!help_read.eof()){

    help_read >> i_val >> j_val >> k_val >> val;

    no_lines += 1;

    if(i_val > i_max)
      i_max = i_val;

    if(j_val > j_max)
      j_max = j_val;

    if(k_val > k_max)
      k_max = k_val;
    

    //if(no_lines < 11)
    //std::cout << "line " << no_lines << ": " << i_val << " " <<  j_val << " " << k_val << std::endl;
  }

  help_read.close();

  int longest_mode = 0;
  
  std::cout << "NNz: " << no_lines << "| mode-1 len: " << i_max << "| mode-2 len: " << j_max << "| mode-3 len: " << k_max << std::endl; 

  int* longest;
  int no_vertices;
  
  // A read for HG2, vertices are fibers of longest mode  
  if((i_max == j_max) && (i_max == k_max)){
    longest = &i_max;
    no_vertices = k_max * j_max;
    std::cout << "Longest mode: 1 -- " << i_max  << std::endl; 
  }

  if((i_max > k_max) && (i_max > j_max)){
    longest = &i_max;
    no_vertices = k_max * j_max;
    std::cout << "Longest mode: 1 -- " << i_max  << std::endl; 
  }
  else if(j_max > k_max){
    longest = &j_max;
    no_vertices = i_max * k_max;
    std::cout << "Longest mode: 2 -- " << j_max  << std::endl; 
  }
  else{
    longest = &k_max;
    no_vertices = i_max * j_max;
    std::cout << "Longest mode: 3 -- " << k_max  << std::endl; 
  }

  std::cout << "No vertices: " << no_vertices << std::endl;


  //HG 1 -> Dimensions of longest are vertices
  //        Dimensions of other modes are nets therefore
  //        #vertices = longest mode's # of vertices
  //        #nets = i + k (if j is longest)
  //        #pins = #NNz
  

  std::ifstream read(ifname);
  
  while(!read.eof()){
    read >> i_val >> j_val >> k_val >> val;
    std::cout << "line " << no_lines << ": " << i_val << " " <<  j_val << " " << k_val << std::endl;
  }

  

  
}

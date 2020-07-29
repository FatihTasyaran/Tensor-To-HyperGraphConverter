#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


int main(int argc, char** argv){


  std::string ifname = argv[1];
  std::string ofname = argv[2];
  
  std::ifstream help_read(ifname);

  int no_lines = -1; //Also number of nonzeros in the tensor

  int i_max = 0; //Length of mode1
  int j_max = 0; //Length of mode2
  int k_max = 0; //Length of mode3
  

  int i_val, j_val, k_val;
  double val;

  while(!help_read.eof()){

    help_read >> i_val >> j_val >> k_val >> val;

    no_lines += 1;

    if(i_val > i_max)
      i_max = i_val;

    if(j_val > j_max)
      j_max = j_val;

    if(k_val > k_max)
      k_max = k_val;
    

    if(no_lines < 11)
    std::cout << "line " << no_lines << ": " << i_val << " " <<  j_val << " " << k_val << std::endl;
  }

  help_read.close();

  int longest_mode = 0;
  int no_nets, no_vertices;
  
  std::cout << "NNz: " << no_lines << "| mode-1 len: " << i_max << "| mode-2 len: " << j_max << "| mode-3 len: " << k_max << std::endl; 


  // i=0, k=1, j=2

  int net_offset = 0;
  
  if((i_max == j_max) && (i_max == k_max)){
    longest_mode = 0;
    no_vertices = i_max;
    no_nets = j_max + k_max;
    net_offset = j_max;
    std::cout << "Longest mode: 1 -- " << i_max  << std::endl; 
  }

  if((i_max > k_max) && (i_max > j_max)){
    longest_mode = 0;
    no_vertices = i_max;
    no_nets = j_max + k_max;
    net_offset = j_max;
    std::cout << "Longest mode: 1 -- " << i_max  << std::endl; 
  }
  else if(j_max > k_max){
    longest_mode = 1;
    no_vertices = j_max;
    no_nets = i_max + k_max;
    net_offset = i_max;
    std::cout << "Longest mode: 2 -- " << j_max  << std::endl; 
  }
  else{
    longest_mode = 2;
    no_vertices = k_max;
    no_nets = i_max + j_max;
    net_offset = i_max;
    std::cout << "Longest mode: 3 -- " << k_max  << std::endl; 
  }

  std::cout << "No vertices: " << no_vertices << std::endl;


  //HG 1 -> Dimensions of longest are vertices
  //        Dimensions of other modes are nets therefore
  //        #vertices = longest mode's # of vertices
  //        #nets = i + k (if j is longest)
  //        #pins = #NNz


  

  int nnz_dims[3];
  std::vector<int> *nets = new std::vector<int>[no_nets+1];

  std::cout << "Allocated mem for nets" << std::endl;
  std::cout << "No nets: " << no_nets << std::endl;
  std::cout << "Second party nets start at: " << net_offset << std::endl;
  
  std::ifstream read(ifname);

  int offset_cursor = 0;
  int curr_nnz = 0;
  
  while(!read.eof()){
    read >> nnz_dims[0] >> nnz_dims[1] >> nnz_dims[2] >> val;
    //std::cout << "0: " << nnz_dims[0] << " 1: " << nnz_dims[1] << " 2: " << nnz_dims[2] << std::endl;
    offset_cursor = 0;
    //std::cout << "Curr nnz: " << curr_nnz << '\n'; 
    for(int dim = 0; dim < 3; dim++){
      //std::cout << "dim: " << dim << " cursor: " << offset_cursor << std::endl;
      if(dim != longest_mode){
	if(offset_cursor == 0){
	  //std::cout << "Pushing " << nnz_dims[longest_mode] << " to net " << nnz_dims[dim] << std::endl;
	  if (std::find(nets[nnz_dims[dim]].begin(), nets[nnz_dims[dim]].end(), nnz_dims[longest_mode]) == nets[nnz_dims[dim]].end())
	    nets[nnz_dims[dim]].push_back(nnz_dims[longest_mode]);
	}
	else{
	  //std::cout << "Pushing " << nnz_dims[longest_mode] << " to net " << nnz_dims[dim] + net_offset << std::endl;
	  if (std::find(nets[nnz_dims[dim]+net_offset].begin(), nets[nnz_dims[dim]+net_offset].end(), nnz_dims[longest_mode]) == nets[nnz_dims[dim]+net_offset].end())
	    nets[nnz_dims[dim]+net_offset].push_back(nnz_dims[longest_mode]);
	}
	offset_cursor = 1;
      }
    }
    curr_nnz++;
  }

  //#define PRINT
#ifdef PRINT
  for(int i = 0; i < no_nets; i++){
    std::cout << "Net " << i << ": ";
    for(int j = 0; j < nets[i].size(); j++){
      std::cout << nets[i][j] << " "; 
    }
    std::cout << "\n";
}
#endif
  
  std::ofstream write(ofname);
  
  write << "1 " << no_vertices << " " << no_nets << " " << no_lines << std::endl;
  
  for(int i = 1; i < no_nets+1; i++){
    for(int j = 0; j < nets[i].size()-1; j++){
      write << nets[i][j] << " ";
    }
    write << nets[i][nets[i].size()-1] << "\n";
  }

  std::cout << "Writing config" << std::endl;
  std::string confile = ofname + ".config";
  std::ofstream config(confile);
  config << longest_mode << " " << net_offset << std::endl;
  std::cout << "Wrote config to file: " << confile << std::endl;
  
  
  
}

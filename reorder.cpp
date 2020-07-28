#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

//static int next_vertice = 0;
//static int next_net = 0;

int main(int argc, char** argv){

  std::string hgname = argv[1];
  std::string configname = argv[2];
  std::string partvecname = argv[3];
  std::string ofname = argv[4];
  
  /*-----READ CONFIG------*/
  std::ifstream config (configname);
  int longest_mode, net_offset;
  config >> longest_mode >> net_offset;
  std::cout << "In config: " << "|Longest mode: " << longest_mode << " |Net offset: " << net_offset << std::endl;
  /*-----READ CONFIG------*/
  
  std::ifstream readhg(hgname);
  
  int val, no_vertices, no_nets, nnz;
  std::string line;


  /*-----READ HYPERGRAPH-----*/
  std::getline(readhg, line);
  std::istringstream fiss(line);
  fiss >> val >> no_vertices >> no_nets >> nnz;

  std::cout << "In Hg: " << "| No vertices: " << no_vertices << " | No nets: " << no_nets << " | NNz: " << nnz << " |" <<std::endl;
  
  std::vector<int>* first_net_set = new std::vector<int>[net_offset];
  std::vector<int>* second_net_set = new std::vector<int>[no_nets - net_offset];

  std::cout << "Allocated mem for nets" << std::endl;

  int current_net = 0;
  
  while(std::getline(readhg, line)){
    //std::cout << "Current net: " << current_net << std::endl;  
    std::istringstream iss(line);
    int vertex;
    while(!iss.eof()){
      iss >> vertex;
      if(current_net < net_offset)
	first_net_set[current_net].push_back(vertex);
      else
	second_net_set[current_net-net_offset].push_back(vertex);
    }
    current_net++;
  }
  /*-----READ HYPERGRAPH-----*/

  //#define PRINTHG
#ifdef PRINTHG
  for(int i = 0; i < 10; i++){
    std::cout << "Net " << i << ": ";
    for(int j = 0; j < first_net_set[i].size(); j++){
      std::cout << first_net_set[i][j] << " ";
    }
    std::cout << "\n";
  }
#endif


/*-----READ PARTVECTOR-----*/
  //Mind the 1-indexed vertices
  std::ifstream readpv(partvecname);
  int* partvec = new int[no_vertices+1];
  int no_parts = 0;
  int part;
  int current_part = 1;
  
  while(!readpv.eof()){
    readpv >> part;
    partvec[current_part] = part;
    if(part > no_parts)
      no_parts = part;
    current_part++;
  }
  no_parts += 1; 
/*-----READ PARTVECTOR-----*/


#define PRINTPART
#ifdef PRINTPART
  for(int i = 1; i < 11; i++){
    std::cout << partvec[i] << std::endl;
  }
#endif
  
  bool* first_set_is_inner = new bool[net_offset];
  bool* second_set_is_inner = new bool[no_nets - net_offset];

  int* first_set_parts = new int[net_offset];
  int* second_set_parts = new int[no_nets - net_offset];

  int* first_set_new_name = new int[net_offset];
  int* second_set_new_name = new int[no_nets - net_offset];

  int next_first_name = 0;
  int next_second_name = 1;

  for(int i = 0; i < net_offset; i++){
    std::cout << "Net " << i << " parts: " << std::endl;
    for(int j = 0; j < first_net_set[i].size();j++){
      std::cout << partvec[first_net_set[i][j]] << " ";
    }
    std::cout << "\n" << std::endl;
  }

  for(int i = 0; i < no_nets - net_offset; i++){
    std::cout << "SNet " << i << " parts: " << std::endl;
    for(int j = 0; j < second_net_set[i].size();j++){
      std::cout << partvec[second_net_set[i][j]] << " ";
    }
    std::cout << "\n" << std::endl;
  }
  
}

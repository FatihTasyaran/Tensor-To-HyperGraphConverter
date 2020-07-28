#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
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

  //int* first_set_parts = new int[net_offset];
  //int* second_set_parts = new int[no_nets - net_offset];

  int* first_set_new_name = new int[net_offset];
  int* second_set_new_name = new int[no_nets - net_offset];

  int next_first_name = 0;
  int next_second_name = 1;


  /*-----FOUND INNER NETS-----*/
  std::vector<int> vec;

  for(int i = 0; i < net_offset; i++){
    vec = first_net_set[i];
    if(std::adjacent_find(vec.begin(), vec.end(), std::not_equal_to<>() ) == vec.end()){ //Is all pins falls in same partition, that is an inner net
      first_set_is_inner[i] = 1;
      std::cout << "Found inner net: " << i << std::endl;
      for(int r = 0; r < first_net_set[i].size(); r++){
	std::cout << first_net_set[i][r] << " ";
      }
      std::cout << std::endl;
    }
    else{
      first_set_is_inner[i] = 0;
    }
  }

  for(int i = 0; i < no_nets - net_offset - 1; i++){
    vec = second_net_set[i];
    if(std::adjacent_find(vec.begin(), vec.end(), std::not_equal_to<>() ) == vec.end()){ //Is all pins falls in same partition, that is an inner net
      second_set_is_inner[i] = 1;
      std::cout << "Found inner net: " << i << std::endl;
      for(int r = 0; r < second_net_set[i].size(); r++){
	std::cout << "Found inner net: " << i << std::endl;
      }
      std::cout << std::endl;
    }
    else{
      second_set_is_inner[i] = 0;
    }
  }
  /*-----FOUND INNER NETS-----*/


  std::vector<int>* first_set_parts = new std::vector<int>[no_parts];
  std::vector<int>* second_set_parts = new std::vector<int>[no_parts];
  
  int* part_count = new int[no_parts];
  memset(part_count, 0, sizeof(int)*no_parts);

  for(int i = 0; i < net_offset; i++){
    std::cout << "##############" << std::endl;
    for(int j = 0; j < first_net_set[i].size(); j++){
      part_count[partvec[first_net_set[i][j]]+1] += 1;
    }
    int max_part = 0;
    int max_part_count = 0;
    for(int i = 0; i < no_parts; i++){
      std::cout << "Part count " << i << " :" << part_count[i] << std::endl;;
      if(part_count[i] > max_part_count){
	max_part = i;
	max_part_count = part_count[i];
      }
    }
    std::cout << "Assigned net " << i << " to part " << max_part << std::endl;
    std::cout << "##############" << std::endl;
    first_set_parts[max_part].push_back(i);
    memset(part_count, 0, sizeof(int)*no_parts);
  }

  for(int i = 0; i < no_nets - net_offset; i++){
    for(int j = 0; j < second_net_set[i].size(); j++){
      part_count[partvec[second_net_set[i][j]]+1] += 1;
    }
    int max_part = 0;
    int max_part_count = 0;
    for(int i = 0; i < no_parts; i++){
      if(part_count[i] > max_part_count){
	max_part = i;
	max_part_count = part_count[i];
      }
    }
    second_set_parts[max_part].push_back(i);
    memset(part_count, 0, sizeof(int)*no_parts);
  }

  for(int i = 0; i < no_parts; i++){
    std::cout << "Nets assigned to part " << i << ": " << first_set_parts[i].size() << std::endl;
  }

  for(int i = 0; i < no_parts; i++){
    std::cout << "SNets assigned to part " << i << ": " << second_set_parts[i].size() << std::endl;
  }
  
}



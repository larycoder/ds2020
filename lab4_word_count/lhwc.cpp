#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <stdio.h>
#include <string.h>

struct hash_map{
  std::string key;
  int value;
  bool operator < (const hash_map &obj) const { // overload smaller operator
    return key < obj.key;
  }
  hash_map(std::string _k, int _v) : key(_k), value(_v){} // constructor
};

class lhwc{
  private:
    std::ifstream file;

  public:
    lhwc(const char* path_file){ // file constructor
      file.open(path_file);
    }
    void close_file(void){ // close current file
      if(file.is_open()){
        file.close();
      }
    }
    std::string input_split(void); // return input split for each mapper
    std::vector<hash_map> mapper(std::string midata); // mapper function
    void sorting(std::vector<hash_map> &idata); // sort intermediate data before feed to reduce
    hash_map reducer(std::vector<hash_map> ridata); // mapper function
};

// implementation of class lhwc
std::string lhwc::input_split(void){
  std::string line;
  std::getline(file, line); // read line of file
  if(file.eof()) line = "\n"; // return empty string if reach EOF
  return line;
}
std::vector<hash_map> lhwc::mapper(std::string midata){
  std::vector<hash_map> raw_data;
  std::istringstream ss(midata);
  for(std::string token; ss >> token; ){
    hash_map pair(token, 1);
    raw_data.push_back(pair);
  }
  return raw_data;
}
hash_map lhwc::reducer(std::vector<hash_map> ridata){
  int counter = 0;
  for(int i = 0; i < ridata.size(); i++){
    counter += ridata[i].value;
  }
  return hash_map(ridata[0].key, counter);
}
void lhwc::sorting(std::vector<hash_map> &idata){
  std::sort(idata.begin(), idata.end());
}

// concurrency implement for map reduce (driver function)
// -----------------------------------------------------
// In this implementation:
// because I have only 1 computer, I use fork to
// generate concurrency in map reduce model
//------------------------------------------------------
int main(){
  std::cout << "Type path to file: ";
  std::string input;
  std::cin >> input;
  char input_c[input.size() + 1];
  strcpy(input_c, input.c_str());

  std::vector<hash_map> data;
  lhwc word_count(input_c);
  
  std::string line;
  while((line = word_count.input_split()) != "\n"){
    // parallel task can perform in here
    //-----------------------------------------------------------
    std::vector<hash_map> line_raw_data = word_count.mapper(line);
    for(int i = 0; i < line_raw_data.size(); i++){
      data.push_back(line_raw_data[i]);
    }
    //-----------------------------------------------------------
  }

  word_count.sorting(data);

  // parallel task can perform in here
  //-----------------------------------------------------------
  std::vector<hash_map> ridata;
  for(int i = 0; i < data.size(); i++){
    if(ridata.size() == 0 || data[i].key == ridata[0].key){
      ridata.push_back(data[i]);
    }
    else{
      hash_map result = word_count.reducer(ridata);
      ridata.clear();
      ridata.push_back(data[i]);
      std::cout << result.key << "\t" << result.value << std::endl;
    }
  }
  if(ridata.size() != 0){
    hash_map result = word_count.reducer(ridata);
    std::cout << result.key << "\t" << result.value << std::endl;
  }
  //-----------------------------------------------------------


  word_count.close_file();
  return 0;
}
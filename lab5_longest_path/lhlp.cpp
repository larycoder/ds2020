#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct hash_map{
  std::string key;
  int value;
  bool operator < (const hash_map &obj) const { // overload smaller operator
    return key < obj.key;
  }
  hash_map(std::string _k, int _v) : key(_k), value(_v){} // constructor
};

class lhlp{
  private:
    std::ifstream file;

  public:
    lhlp(const char* path_file){ // file constructor
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

// implementation of class lhlp
std::string lhlp::input_split(void){
  std::string line;
  std::getline(file, line); // read line of file
  if(file.eof()) line = "\n"; // return empty string if reach EOF
  return line;
}
std::vector<hash_map> lhlp::mapper(std::string midata){
  std::vector<hash_map> raw_data;
  int num_path = 0;
  for(int i = 0; i < midata.size(); i++){
    if(midata.c_str()[i] == '/'){
      num_path++;
    }
  }
  hash_map pair(midata, num_path);
  raw_data.push_back(pair);
  return raw_data;
}
hash_map lhlp::reducer(std::vector<hash_map> ridata){
  int max = -1;
  std::string key;
  for(int i = 0; i < ridata.size(); i++){
    if(ridata[i].value > max){
      max = ridata[i].value;
      key = ridata[i].key;
    }
  }
  return hash_map(key, max);
}
void lhlp::sorting(std::vector<hash_map> &idata){
  std::sort(idata.begin(), idata.end());
}

// concurrency implement for map reduce (driver function)
// -----------------------------------------------------
// In this implementation:
// because I have only 1 computer, I use fork to
// generate concurrency in map reduce model and pipe
// used as the IPC between slave and master
//------------------------------------------------------
int main(){
  std::cout << "Type path to file: ";
  std::string input;
  std::cin >> input;
  char input_c[input.size() + 1];
  strcpy(input_c, input.c_str());

  // prepare pipe
  int pfd[2];
  pipe(pfd);

  pid_t wpid;
  int status;

  std::vector<hash_map> data;
  lhlp word_count(input_c);

  std::string line;
  std::vector<hash_map> line_raw_data;
  int num_map = 0;

  while((line = word_count.input_split()) != "\n"){ // mapper creator
    num_map++;
    int pid = fork();
    if(pid == 0){ // child process act as mapper
      line_raw_data = word_count.mapper(line);
      for(int i = 0; i < line_raw_data.size(); i++){
        int value = line_raw_data[i].value;
        int key_length = line_raw_data[i].key.size() + 1;
        char buff[ 4 + 4 + key_length];
        memcpy(&buff[0], &value, 4);
        memcpy(&buff[4], &key_length, 4);
        memcpy(&buff[8], line_raw_data[i].key.c_str(), key_length);
        write(pfd[1], buff, sizeof(buff));
      }
      pid = -1;
      write(pfd[1], &pid, 4);
      std::cout << "mapper end, process line: " << line << std::endl;
      // close fd
      word_count.close_file();
      close(pfd[0]);
      close(pfd[1]);
      return 0;
    }
  }

  while(num_map > 0){ // parent process wait to receive result from all mapper
    int pdata;
    read(pfd[0], &pdata, 4);
    if(pdata >= 0){ // store immediate data
      int value = pdata; // get value of hash map
      // get key length of hash map
      int key_length;
      read(pfd[0], &key_length, 4);
      // get key of hash map
      char key[key_length];
      read(pfd[0], key, key_length);
      std::string skey(key, key_length);
      // push raw data to buffer in master process
      hash_map raw_data(skey, value);
      data.push_back(raw_data);
    }
    else{ // get end signal from a mapper
      num_map--;
    }
  }

  while ((wpid = wait(&status)) > 0); // wait untill all slave process end

  word_count.sorting(data);

  std::vector<hash_map> ridata = data;
  
  if(fork() == 0){
    hash_map result = word_count.reducer(ridata);
    std::cout << "reducer end, result:\t" << result.key << "\t" << result.value << std::endl;
    word_count.close_file();
    close(pfd[0]);
    close(pfd[1]);
    return 0;
  }

  while ((wpid = wait(&status)) > 0); // wait untill all slave process end

  word_count.close_file();
  close(pfd[0]);
  close(pfd[1]);
  return 0;
}
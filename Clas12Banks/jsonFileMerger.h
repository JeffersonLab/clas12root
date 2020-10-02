#ifndef JSONFILEMERGER_H
#define JSONFILEMERGER_H

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"

#include <string>
#include <iostream>
#include <vector>

namespace clas12 {
  using std::string;
  using std::cout;
  using std::endl;
  using namespace rapidjson;
  
  class jsonFileMerger{

  public:
    jsonFileMerger()=default;
    jsonFileMerger(std::string outFilePath){_outFilePath=outFilePath;};
    virtual ~jsonFileMerger()=default;

    void addFile(string file){_files.push_back(file);}
    void mergeAllFiles();

  private: 
    string _outFilePath;
    std::vector<string> _files;
    void mergeJsonFiles(Value& target, Value& source, Value::AllocatorType& allocator);   
    void writeJsonFile(Document &jsonDOM);
    void print(Document &jsonDOM);
  };
}
#endif /* JSONFILEMERGER_H */

#include "jsonFileMerger.h"

namespace clas12 {

  /*
   * Function to merge all files that have been passed to the merger.
   */
  void jsonFileMerger::mergeAllFiles(){
    if(_files.size()<2){
      cout<<"Please provide at least two files"<<endl;
      return;
    }
    Document jsonDOMAll;  
    //Buffer needed to keep documents in memory.
    Document jsonDOMBuff[_files.size()];
    for(int i=0; i<_files.size();i++){
      char readBuffer[65536];
      FILE * jsonFile = fopen(_files[i].c_str(),"r");
      FileReadStream * jsonStream = new FileReadStream(jsonFile,readBuffer,sizeof(readBuffer));
      jsonDOMBuff[i].ParseStream(*jsonStream);
      if(i==0){
	jsonDOMAll.CopyFrom(jsonDOMBuff[i],jsonDOMAll.GetAllocator());
	
      } else{
	mergeJsonFiles(jsonDOMAll,jsonDOMBuff[i],jsonDOMAll.GetAllocator());
      }
      fclose(jsonFile);
    }
    writeJsonFile(jsonDOMAll);
  }

  /*
   * Function to merge two json files.
   * Target contains information from a file to append to source.
   */
  void jsonFileMerger::mergeJsonFiles(Value& target, Value& source, Value::AllocatorType& allocator) {
    assert(target.IsObject());
    assert(source.IsObject());
    for (Value::MemberIterator itr = source.MemberBegin(); itr != source.MemberEnd(); ++itr){
      target.AddMember(itr->name, itr->value, allocator);
    }
  }

  /*
   * Function to write jsonDOM to a file.
   */
  void jsonFileMerger::writeJsonFile(Document &jsonDOM){
    FILE* fp = fopen(_outFilePath.c_str(), "wb"); 
 
    char writeBuffer[65536];
    FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
 
    Writer<FileWriteStream> writer(os);
    jsonDOM.Accept(writer);
 
    fclose(fp);
  }

  /*
   * Function to print out jsonDOM, mostly used for debugging.
   */
  void jsonFileMerger::print(Document &jsonDOM){        
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    jsonDOM.Accept(writer);
    auto str = sb.GetString();
    printf("%s\n", str);
  }

}

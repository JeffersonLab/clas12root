/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "dictionary.h"
#include "utils.h"
#include <cstdlib>

namespace hipo {

  void  schema::parse(const std::string& schString){
    std::vector<std::string>  entries;
    std::vector<std::string>  entry;
    hipo::utils::tokenize(schString, entries, ",");
    int offset = 0;
    for(int i = 0; i < entries.size(); i++){
      entry.clear();
      hipo::utils::tokenize(entries[i],entry, "/");
      schemaEntry_t e;
      e.name = hipo::utils::trim(entry[0]);
      e.type = hipo::utils::trim(entry[1]);
      e.typeId = getTypeByString(e.type);
      e.typeSize = getTypeSize(e.typeId);
      e.offset   = offset;
      offset += e.typeSize;
      schemaEntries.push_back(e);
      schemaEntriesMap[e.name] = i;
   }
  }

  int   schema::getTypeByString(std::string &typeName){
      if(typeName=="B"){
        return 1;
      } else if(typeName=="S") {
        return 2;
      } else if(typeName=="I") {
        return 3;
      } else if(typeName=="F") {
        return 4;
      } else if(typeName=="D") {
        return 5;
      } else if(typeName=="L") {
        return 8;
      }
      return -1;
  }

  int  schema::getTypeSize(int id){
    switch(id){
      case 1: return 1;
      case 2: return 2;
      case 3: return 4;
      case 4: return 4;
      case 5: return 8;
      case 8: return 8;
      default: return 0;
    }
    return 0;
  }

  void  schema::show(){
    printf("schema : %14s , group = %6d, item = %3d\n",
       schemaName.c_str(),groupid,itemid);
    for(auto & schemaEntrie : schemaEntries){
      printf("%16s : (%3s) %5d %5d , offset = %3d --> [%s]\n",
         schemaEntrie.name.c_str(),schemaEntrie.type.c_str(),
         schemaEntrie.typeId,schemaEntrie.typeSize, schemaEntrie.offset,
         schemaEntrie.name.c_str()
         );
    }
  }


  int   schema::getSizeForRows(int rows){
    int nentries = schemaEntries.size();
    int offset   = getOffset(nentries-1,rows-1,rows) + schemaEntries[nentries-1].typeSize;
    return offset;
  }

  std::string  schema::getSchemaString(){
    char parts[256];
    std::string result;
    sprintf(parts,"{%s/%d/%d}{",schemaName.c_str(),groupid,itemid);
    result.append(parts);
    for(int loop = 0; loop < schemaEntries.size(); loop++){
      sprintf(parts,"%s/%s",schemaEntries[loop].name.c_str(), schemaEntries[loop].type.c_str());
      if(loop!=0) result.append(",");
      result.append(parts);
    }
    result.append("}");
    return result;
  }

  std::string  schema::getSchemaStringJson(){
     char parts[256];
    std::string result;
    sprintf(parts,"{ \"name\": \"%s\", \"group\": %d, \"item\": %d, \"info\": \" \",",
               schemaName.c_str(),groupid,itemid);
    result.append(parts);
    result.append("\"entries\": [ ");
    for(int loop = 0; loop < schemaEntries.size(); loop++){
      sprintf(parts,"{\"name\":\"%s\", \"type\":\"%s\", \"info\":\" \"}",
       schemaEntries[loop].name.c_str(), schemaEntries[loop].type.c_str());
      if(loop!=0) result.append(",");
      result.append(parts);
    }
    result.append("] }");
    return result;
  }


  //=============================================
  // Implementation of dictionary class
  //=============================================
  std::vector<std::string> dictionary::getSchemaList(){
    std::map<std::string, schema>::iterator it;
    std::vector<std::string> vec;
    for ( it = factory.begin(); it != factory.end(); it++ ){
        vec.push_back(it->first);
    }
    return vec;
  }

  bool    dictionary::parse(const char *schemaString){
    std::vector<std::string> tokens;
    std::string schemahead = hipo::utils::substring(schemaString,"{","}",0);
    hipo::utils::tokenize(schemahead, tokens, "/");
    int group = std::atoi(tokens[1].c_str());
    int  item = std::atoi(tokens[2].c_str());
    hipo::schema  schema (tokens[0].c_str(),group,item);
    std::string schemabody = hipo::utils::substring(schemaString,"{","}",1);
    schema.parse(schemabody.c_str());
    addSchema(schema);
    return true;
  }

  void    dictionary::show(){
    std::vector<std::string> list = getSchemaList();
    for(auto & i : list){
      schema sc = getSchema(i.c_str());
      printf("%24s : %5d %5d %5d\n", sc.getName().c_str(),
        sc.getGroup(), sc.getItem(),sc.getEntries());
    }
  }
}

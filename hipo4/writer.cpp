//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */

#include "writer.h"
#include <cstdlib>

namespace hipo {

/**
* Open a File for writing, it includes the dictionary
* in the file.
*/
 void writer::open(const char *filename){
    outputStream.open(filename);

    std::vector<std::string> schemaList = writerDictionary.getSchemaList();

    recordbuilder  builder;
    event          schemaEvent;

    for(int i = 0; i < schemaList.size(); i++){
        std::string schemaString = writerDictionary.getSchema(schemaList[i].c_str()).getSchemaString();
        std::string schemaStringJson = writerDictionary.getSchema(schemaList[i].c_str()).getSchemaStringJson();
        //---> Can open after debug level is introduced in the class
        //printf("STR  : %s\n",schemaString.c_str());
        //printf("JSON : %s\n",schemaStringJson.c_str());
        schemaEvent.reset();
        structure schemaNode(120,2,schemaString);
        structure schemaNodeJson(120,1,schemaStringJson);
        schemaEvent.addStructure(schemaNodeJson);
        schemaEvent.addStructure(schemaNode);
	      schemaEvent.show();
        builder.addEvent(schemaEvent);
    }

    //-------------------------------------------------------------------------
    // This section is added on April-28-2022, it will store user provided
    // configurations along with the schema in the record that goes into
    // HIPO header. G^2
    //-------------------------------------------------------------------------
    std::map<std::string,std::string>::iterator it;
    event configEvent;
    
    for( it = userConfig.begin(); it != userConfig.end(); it++){
      printf("::: adding user configuration (key): %s\n",it->first.c_str());

      std::string wKey = std::string(it->first.c_str());
      std::string wConfig = std::string(it->second.c_str());
      
      structure configKey(32555,1,wKey);
      structure configString(32555,2,wConfig);
      
      configEvent.reset();
      configEvent.addStructure(configKey);
      configEvent.addStructure(configString);
      builder.addEvent(configEvent);
    }
    
    //printf(" RECORD SIZE BEFORE BUILD = %d\n",builder.getRecordSize());
    builder.build();
    //printf(" RECORD SIZE AFTER  BUILD = %d, NENTRIES = %d\n",
    //  builder.getRecordSize(),builder.getEntries());

    int dictionarySize = builder.getRecordSize();

    hipoFileHeader_t header;

    //header.uniqueid         = 0x43455248;
    header.uniqueid         = 0x4F504948;// 4849504F;
    header.filenumber       = 1;
    header.headerLength     = 14;
    header.recordCount      = 0;
    header.indexArrayLength = 0;
    header.bitInfoVersion   = (0x000000FF&6);
    header.userHeaderLength = dictionarySize;// will change with the dictionary
    header.magicNumber      = 0xc0da0100;
    header.userRegister     = 0;
    header.trailerPosition  = 0;
    header.userIntegerOne   = 0;
    header.userIntegerTwo   = 0;

    outputStream.write( reinterpret_cast<char *> (&header),sizeof(header));
    long  position = outputStream.tellp();

    printf("writing     header:: position = %ld\n",position);
    outputStream.write( reinterpret_cast<char *> (&builder.getRecordBuffer()[0]),dictionarySize);
    position = outputStream.tellp();
    printf("writing dictionary:: position = %ld\n",position);
 }

void writer::addDictionary(hipo::dictionary &dict){
    std::vector<std::string> schemaList = dict.getSchemaList();
    for(int i = 0; i < schemaList.size(); i++){
        writerDictionary.addSchema(dict.getSchema(schemaList[i].c_str()));
      }
}

 void writer::addEvent(hipo::event &hevent){
  if(hevent.getTag()==0){
    bool status = recordBuilder.addEvent(hevent);
    if(status==false){
      writeRecord(recordBuilder);
      recordBuilder.addEvent(hevent);
    } 
  } else {
    int tag = hevent.getTag();
    extendedBuilder[tag].setUserWordOne(tag);
    bool status = extendedBuilder[tag].addEvent(hevent);
    if(status==false){
      writeRecord(extendedBuilder[tag]);
      extendedBuilder[tag].addEvent(hevent);
    } 
  }
 }

void writer::addEvent(std::vector<char> &vec, int size ){
  int transferSize = size;
  if(size<0){ transferSize = vec.size(); }
  bool status = recordBuilder.addEvent(vec,0,transferSize);
  if(status==false){
    writeRecord(recordBuilder);
    recordBuilder.addEvent(vec,0,transferSize);
  }
}

 void writer::writeRecord(recordbuilder &builder){
   builder.build();
   recordInfo_t  recordInfo;
   recordInfo.recordPosition = outputStream.tellp();
   recordInfo.recordEntries  = builder.getEntries();
   recordInfo.recordLength   = builder.getRecordSize();
   recordInfo.userWordOne    = builder.getUserWordOne();
   recordInfo.userWordTwo    = builder.getUserWordTwo();
   if(recordInfo.recordEntries>0){
      outputStream.write( reinterpret_cast<char *> (&builder.getRecordBuffer()[0]),recordInfo.recordLength);
      writerRecordInfo.push_back(recordInfo);
      if(verbose>0) printf("%6ld : writing::record : size = %8d, entries = %8d, position = %12ld word = %12ld %12ld\n",
                  writerRecordInfo.size(), recordInfo.recordLength,recordInfo.recordEntries,
                  recordInfo.recordPosition,recordInfo.userWordOne,recordInfo.userWordTwo);
   }  else {
     if(verbose>0) printf(" write::record : empty record will not be written.....\n");
   }
   builder.reset();
 }

void writer::showSummary(){
  for(int loop = 0; loop < writerRecordInfo.size(); loop++){
    recordInfo_t  recordInfo = writerRecordInfo[loop];
    printf(" %6d : record INFO : size = %8d, entries = %8d, position = %12ld word = %12ld %12ld\n", loop,
             recordInfo.recordLength,recordInfo.recordEntries,recordInfo.recordPosition,
           recordInfo.userWordOne,recordInfo.userWordTwo);
  }
}


void writer::writeIndexTable(){
  hipo::schema indexSchema("file::index",32111,1);
  indexSchema.parse("position/L,length/I,entries/I,userWordOne/L,userWordTwo/L");
  int  nEntries = writerRecordInfo.size();
  long indexPosition = outputStream.tellp();
  printf("\n\n-----> writing file index : entries = %d, position = %ld\n",
         nEntries,indexPosition);
  hipo::bank indexBank(indexSchema,nEntries);
  for(int i = 0; i < nEntries; i++){
    recordInfo_t  recordInfo = writerRecordInfo[i];
    indexBank.putLong("position",i,recordInfo.recordPosition);
    indexBank.putInt("length",i,recordInfo.recordLength);
    indexBank.putInt("entries",i,recordInfo.recordEntries);
    indexBank.putLong("userWordOne",i,recordInfo.userWordOne);
    indexBank.putLong("userWordTwo",i,recordInfo.userWordTwo);
  }

  int eventSize = 32*nEntries + 1024;

  hipo::event indexEvent(eventSize);
  indexEvent.addStructure(indexBank);
  recordBuilder.reset();
  recordBuilder.addEvent(indexEvent);
  writeRecord(recordBuilder);
  outputStream.seekp(40);
  outputStream.write(reinterpret_cast<char *> (&indexPosition), 8);
}

void writer::close(){
  writeRecord(recordBuilder);

  std::map<int,hipo::recordbuilder>::iterator it;
  for(it = extendedBuilder.begin(); it != extendedBuilder.end(); it++){
    writeRecord(it->second);
  } 

  writeIndexTable();
  outputStream.close();
  writerRecordInfo.clear();
}

/***
* Function to change the record builder user word one
*/
void writer::setUserIntegerOne(long userIntOne){
  recordBuilder.setUserWordOne(userIntOne);
}

/***
*Function to change the record builder user word two
*/
void writer::setUserIntegerTwo(long userIntTwo){
  recordBuilder.setUserWordTwo(userIntTwo);
}

/***
*Function to write buffer.
*/
void writer::flush(){
  writeRecord(recordBuilder);
}

}

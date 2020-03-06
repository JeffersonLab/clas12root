/*
 * This sowftware was developed at Jefferson National Laboratory.
 * (c) 2017.
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
        printf("STR  : %s\n",schemaString.c_str());
        printf("JSON : %s\n",schemaStringJson.c_str());
        schemaEvent.reset();
        structure schemaNode(120,2,schemaString);
        structure schemaNodeJson(120,1,schemaStringJson);
        schemaEvent.addStructure(schemaNode);
        schemaEvent.addStructure(schemaNodeJson);
        schemaEvent.show();
        builder.addEvent(schemaEvent);

    }


    printf(" RECORD SIZE BEFORE BUILD = %d\n",builder.getRecordSize());
    builder.build();
    printf(" RECORD SIZE AFTER  BUILD = %d, NENTRIES = %d\n",
      builder.getRecordSize(),builder.getEntries());

    int dictionarySize = builder.getRecordSize();

    hipoFileHeader_t header;

    header.uniqueid         = 0x43455248;
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
   bool status = recordBuilder.addEvent(hevent);
   if(status==false){
     writeRecord(recordBuilder);
     recordBuilder.addEvent(hevent);
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
      printf("%6ld : writing::record : size = %8d, entries = %8d, position = %12ld word = %12ld %12ld\n",
                  writerRecordInfo.size(), recordInfo.recordLength,recordInfo.recordEntries,
                  recordInfo.recordPosition,recordInfo.userWordOne,recordInfo.userWordTwo);
   }  else {
     printf(" write::record : empty record will not be written.....");
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
  writeIndexTable();
  outputStream.close();
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

#include <iostream>
#include "reader.h"
#include "event.h"

hipo::reader      hipo_FORT_Reader;
hipo::event       hipo_FORT_Event;
hipo::dictionary  hipo_FORT_Dictionary;

std::map<std::string, hipo::bank *> eventStore;

extern "C" {

  void hipo_file_open_( const char *filename, int length){
    char *buffer = (char *) malloc(length+1);
    memcpy(buffer,filename,length);
    buffer[length] = '\0';
    printf("[FORTRAN] opening file : %s\n", buffer);
    hipo_FORT_Reader.open(buffer);
    hipo_FORT_Reader.readDictionary(hipo_FORT_Dictionary);
    free(buffer);
  }

  void hipo_file_open(const char *filename){
    hipo_FORT_Reader.open(filename);
    hipo_FORT_Reader.readDictionary(hipo_FORT_Dictionary);
  }
  
  int hipo_file_next_(int* fstatus){
    bool status = hipo_FORT_Reader.next();
    if(status==false){
      *fstatus = 12;
      return 12;
    }
    hipo_FORT_Reader.read(hipo_FORT_Event);
    std::map<std::string, hipo::bank *>::iterator it;
    for ( it = eventStore.begin(); it != eventStore.end(); it++ )
      {
         it->second->reset();   // string's value
      }
      *fstatus = 0;
      return 0;
  }

  void hipo_read_bank_(const char *bankname, int *bankRows, int banknameLength){
    char *buffer = (char * ) malloc(banknameLength+1);
    memcpy(buffer,bankname,banknameLength);
    buffer[banknameLength] = '\0';
    if(eventStore.count(std::string(buffer))==0){
      if(hipo_FORT_Dictionary.hasSchema(buffer)==true){
         hipo::bank *bank_ptr = new hipo::bank(hipo_FORT_Dictionary.getSchema(buffer));
         eventStore[buffer] = bank_ptr;
         printf("---> map : initializing bank \"%24s\" (%6d, %5d) to the store\n",
            buffer,hipo_FORT_Dictionary.getSchema(buffer).getGroup(),
            hipo_FORT_Dictionary.getSchema(buffer).getItem() );
      } else {
         *bankRows = 0;
         free(buffer);
         return;
      }
    }

    hipo_FORT_Event.getStructure(*eventStore[buffer]);
    *bankRows = eventStore[buffer]->getRows();
    free(buffer);
  }
/*
  void hipo_read_bank_byid_(int *group, int *item){
    int igroup = *group;
    int iitem = *item;

    if(eventStore.count(igroup)==0){
        printf("********* bank does not exist %d\n",igroup);
        std::vector<std::string> schemaList = hipo_FORT_Dictionary.getSchemaList();
        for(int i = 0; i < schemaList.size(); i++){
            int schGroup = hipo_FORT_Dictionary.getSchema(schemaList[i].c_str()).getGroup();
            if(schGroup==igroup){
              printf(" found group = %d\n",schGroup);
               printf("---> map : initializing bank \"%24s\" (%6d) to the store\n",
                 schemaList[i].c_str(),schGroup);
                 hipo::bank *bank_ptr = new hipo::bank(hipo_FORT_Dictionary.getSchema(schemaList[i].c_str()));
                 eventStore[schGroup] = bank_ptr;
            }
        }
    }
  }*/

  void get_bank_rows_(int *group, int *bankRows){
      /*int igroup = *group;
      if(eventStore.count(igroup)==0) *bankRows = 0;
      *bankRows = eventStore[igroup]->getRows();*/
  }

  void hipo_read_float_(const char *group, const char *item, int *nread, float *buffer, int *maxRows,
      int length_group, int length_item){

      char *buffer_group = (char * ) malloc(length_group+1);
      memcpy(buffer_group,group,length_group);
      buffer_group[length_group] = '\0';

      char *buffer_item = (char * ) malloc(length_item+1);
      memcpy(buffer_item,item,length_item);
      buffer_item[length_item] = '\0';

      //printf("---->>>>> reading float (%s) (%s)\n",buffer_group,buffer_item);
      /*int id_g = *group;
      int id_i = ;
      int max  = *maxRows;
      //printf("READIN FLOAT %d %d \n",*group,*item);
      //std::vector<float> vec = hipo_FORT_Reader.getEvent()->getFloat(id_g,id_i);
      //printf("RESULT SIZE = %d \n",vec.size());
      for(int i = 0; i < vec.size(); i++){
	       if(i<max) buffer[i] = vec[i];
      }
      *nread = vec.size();*/
      if(eventStore.count(buffer_group)==0){
         *nread = 0;
         free(buffer_group);
         free(buffer_item);
         return;
      }

      hipo::bank *bank = eventStore[buffer_group];
      int  nrows = bank->getRows();
      if(nrows>(*maxRows)) nrows = *(maxRows);
      //printf("---->>>>> reading float (%s) (%s) (%d)\n",buffer_group,buffer_item,nrows);
      for(int i = 0; i < nrows; i++){
         buffer[i] = bank->getFloat(buffer_item, i);
      }
      *nread = nrows;

      free(buffer_group);
      free(buffer_item);
  }

  void hipo_read_int_(const char *group, const char *item, int *nread, int *buffer, int *maxRows,
      int length_group, int length_item){

      char *buffer_group = (char * ) malloc(length_group+1);
      memcpy(buffer_group,group,length_group);
      buffer_group[length_group] = '\0';

      char *buffer_item = (char * ) malloc(length_item+1);
      memcpy(buffer_item,item,length_item);
      buffer_item[length_item] = '\0';

      if(eventStore.count(buffer_group)==0){
         *nread = 0;
         free(buffer_group);
         free(buffer_item);
         return;
      }

      hipo::bank *bank = eventStore[buffer_group];
      int  nrows = bank->getRows();
      if(nrows>(*maxRows)) nrows = *(maxRows);
      //printf("---->>>>> reading float (%s) (%s) (%d)\n",buffer_group,buffer_item,nrows);
      for(int i = 0; i < nrows; i++){
         buffer[i] = bank->getInt(buffer_item, i);
      }
      *nread = nrows;
      free(buffer_group);
      free(buffer_item);
    }

    void hipo_read_long_(const char *group, const char *item, int *nread, int64_t *buffer, int *maxRows,
        int length_group, int length_item){

        char *buffer_group = (char * ) malloc(length_group+1);
        memcpy(buffer_group,group,length_group);
        buffer_group[length_group] = '\0';

        char *buffer_item = (char * ) malloc(length_item+1);
        memcpy(buffer_item,item,length_item);
        buffer_item[length_item] = '\0';

        if(eventStore.count(buffer_group)==0){
           *nread = 0;
           free(buffer_group);
           free(buffer_item);
           return;
        }
        
        hipo::bank *bank = eventStore[buffer_group];
        int  nrows = bank->getRows();
        if(nrows>(*maxRows)) nrows = *(maxRows);
        //printf("---->>>>> reading float (%s) (%s) (%d)\n",buffer_group,buffer_item,nrows);
        for(int i = 0; i < nrows; i++){
           buffer[i] = bank->getLong(buffer_item, i);
        }
        *nread = nrows;
        free(buffer_group);
        free(buffer_item);
      }
    /*void hipo_read_int_(int *group, int *item, int *nread, int *buffer, int *maxRows){

    }*/

}

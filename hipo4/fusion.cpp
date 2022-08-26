/*----------------------------------------------------------------------
 * This is the implementation of fusion class which is used in
 * interfacing HIPO library from python.
 * author: G.Gavalian (June, 2022)
 * ---------------------------------------------------------------------
 */

#include "fusion.h"


namespace hipo {

  void inputSource::open(const char *filename){
   reader.open(filename);
   reader.readDictionary(factory);
  }

  void inputSource::define(const char *bank){
      banks[std::string(bank)] = hipo::bank(factory.getSchema(bank));
  }
  bool inputSource::next(){
     bool status = reader.next();
     if(status==true){
       reader.read(event);
       std::map<std::string, hipo::bank>::iterator it;
       for (it = banks.begin(); it != banks.end(); it++)
       {
         event.getStructure(it->second);
            //How do I access each element?  
       }  
     }
     return status;
  }

int inputSource::getSize(const char *bank){
  return banks[bank].getRows();
}


int     inputSource::getInt(   const char *bank, const char *entry, int row){
   return banks[bank].getInt(entry,row);
}
double  inputSource::getFloat( const char *bank, const char *entry, int row){
   return banks[bank].getFloat(entry,row);
}


   int  fusion::open(const char *filename){
     int number = sources.size() + 1;
     sources.insert(std::make_pair(number, new inputSource(filename)));
     return number;
   }

  void  fusion::define(int fid, const char *bank){
     sources[fid]->define(bank);
  }

   int  fusion::getSize(int fid, const char *bank){
     return sources[fid]->getSize(bank);
  }

bool   fusion::next(int handle){
   return sources[handle]->next();
}

int     fusion::getInt(   int handle, const char *bank, const char *entry, int row){
   return sources[handle]->getInt(bank,entry,row);
}

float  fusion::getFloat( int handle, const char *bank, const char *entry, int row){
   //double result = sources[handle]->getFloat(bank,entry,row);
   //printf("reuslt = %f\n",result);
   return sources[handle]->getFloat(bank,entry,row);
}

int     fusion::getType( int handle, const char *bank, const char *entry){
  hipo::bank b = sources[handle]->get(bank);
  return b.getSchema().getEntryType(b.getSchema().getEntryOrder(entry));
}
}

#include "scaler_reader.h"

namespace clas12{
  
  scaler_reader::scaler_reader(std::string filename){
    _reader.setTags(1);
    _reader.open(filename.data()); //keep a pointer to the reader
    _reader.readDictionary(_factory);
    _factory.show();
    _bscal.reset(new scaler{_factory.getSchema("RUN::scaler")});
    _bscal->show();
    while(_reader.next()){
      _reader.read(_event);
      _event.getStructure(*_bscal.get());
      if(_bscal->getRows()==0)continue; //only RUN::scaler

      auto cup=_bscal->getFCupGated();
      if(cup>_maxCup) _maxCup=cup;
      if(cup<_minCup) _minCup=cup;
    }
  }


}

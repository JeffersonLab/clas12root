#pragma once


namespace hipo {

  class write_ntuple {

    write_ntuple()=default;
    virtual ~write_ntuple=default;
    
  private :
    writer _writer;
    std::vector<void* > _links;
    
  };
}

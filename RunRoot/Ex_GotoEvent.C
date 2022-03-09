//Example of how to go to a particualr event in the hipi file
//using clas12reader::grabEvent
void Ex_GotoEvent(){

    clas12reader c12("/hdd/jlab/clas12data/skim3_005644.hipo",{0});
    
    auto good= c12.grabEvent(100003);
    if(good){
      std::cout<<"event : rows  "<<c12.event()->getRows()<<" runconfig : rows "<<c12.runconfig()->getRows()<<" number of particles "<< c12.getDetParticles().size()<<endl;
      
      cout<<" start time "<<c12.event()->getStartTime()<<" DAQ event number "<<c12.runconfig()->getEvent()<<endl;;
  }
    good = c12.grabEvent(100000005);
    if(good){
      std::cout<<"event : rows  "<<c12.event()->getRows()<<" runconfig : rows "<<c12.runconfig()->getRows()<<" number of particles "<< c12.getDetParticles().size()<<endl;
      
      cout<<" start time "<<c12.event()->getStartTime()<<" DAQ event number "<<c12.runconfig()->getEvent()<<endl;;
    }
    
    
  
}

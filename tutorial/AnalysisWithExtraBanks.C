{

  HipoChain chain;
  chain.Add("/home/dglazier/clas12/clas12root/tutorial/skim.hipo");
  chain.SetReaderTags({0});  //create clas12reader with just tag 0 events

  //chain.Add("/where/is/myHipo.hipo");
  auto config_c12=chain.GetC12Reader();
  config_c12->addExactPid(11,1);    //exactly 1 electron
  config_c12->addExactPid(211,1);    //exactly 1 pi+
  config_c12->addExactPid(-211,1);    //exactly 1 pi-
  config_c12->addExactPid(2212,1);    //exactly 1 proton
  config_c12->addExactPid(22,2);    //exactly 2 gamma

  //Add extra bank for reading and get its ID
  auto idx_RECPart= config_c12->addBank("REC::Particle");
  //Add an item in the bank for reading and get its ID
  auto iPid= config_c12->getBankOrder(idx_RECPart,"pid");
  
  //get track based hits id and layers
  //Add extra bank for reading and get its ID
  auto idx_TRCKHits= config_c12->addBank("TimeBasedTrkg::TBHits");
  //Add an item in the bank for reading and get its ID
  auto iTrckId =  config_c12->getBankOrder(idx_TRCKHits,"id");
  auto iTrckLayer =  config_c12->getBankOrder(idx_TRCKHits,"layer");
  
  //create particles before looping to be more efficient
  TLorentzVector p4_gamma1;
  TLorentzVector p4_gamma2;

  //Create histogram
  TH1F hmass("pi0mass","Invariant Mass to 2#gamma",100,0,0.6);
  TH1F htime("DeltaTime","Time difference of 2#gamma",100,-10,10);
  
  cout<<"START EVENT LOOP "<<endl;
  //now get reference to (unique)ptr for accessing data in loop
  //this will point to the correct place when file changes
  auto& c12=chain.C12ref();
  //loop over all events in the file
  while(chain.Next()==true){
       
    if(c12->getDetParticles().empty())
      continue;

    //Loop over entries in the bank for this event using its ID = idx_RECPart
    //get the value of its PID from its id = iPid
    for(auto ipa=0;ipa<c12->getBank(idx_RECPart)->getRows();ipa++){
      cout<<"particle "<<ipa<<" pid "<<c12->getBank(idx_RECPart)->getInt(iPid,ipa)<<endl;
    }
      
    //Loop over track based hits
    for(auto itr=0;itr<c12->getBank(idx_TRCKHits)->getRows();itr++){
      cout<<"track "<<itr<<" id "<<c12->getBank(idx_TRCKHits)->getInt(iTrckId,itr)<<" layer "<<c12->getBank(idx_TRCKHits)->getInt(iTrckLayer,itr)<<endl;
    }
      
    auto parts=c12->getDetParticles();
       
    auto electron=c12->getByID(11)[0];
    auto gamma1=c12->getByID(22)[0];
    auto gamma2=c12->getByID(22)[1];
    auto proton=c12->getByID(2212)[0];
    auto pip=c12->getByID(211)[0];
    auto pim=c12->getByID(-211)[0];
 								 
    p4_gamma1.SetXYZM(gamma1->par()->getPx(),gamma1->par()->getPy(),gamma1->par()->getPz(),0);
    p4_gamma2.SetXYZM(gamma2->par()->getPx(),gamma2->par()->getPy(),gamma2->par()->getPz(),0);

    auto pi0 = p4_gamma1 + p4_gamma2;
     
    //Fill histograms if gammas are in FD
    if(gamma1->getRegion()==FD && gamma2->getRegion()==FD){
      hmass.Fill(pi0.M());
      htime.Fill(gamma1->getTime() - gamma2->getTime() );
    }
  }
  


  TCanvas can;
  can.Divide(2,1);
  can.cd(1);
  hmass.Draw();
  can.cd(2);
  htime.Draw();
}

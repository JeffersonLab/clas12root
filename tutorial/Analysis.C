{

  HipoChain chain;
  // chain.Add("/WHERE/IS/MY/HIPO/file.hipo");


  chain.Add("/work/jlab/clas12data/v16/skim9_5038.hipo");

  //create particles before looping to be more efficient
  TLorentzVector p4_gamma1;
  TLorentzVector p4_gamma2;

  //Create histogram
  TH1F hmass("pi0mass","Invariant Mass to 2#gamma",100,0,0.6);
  TH1F htime("DeltaTime","Time difference of 2#gamma",100,-10,10);
  
  //loop over files
  for(int ifile=0;ifile<chain.GetNFiles();++ifile){
    clas12reader c12{chain.GetFileName(ifile).Data()};

    c12.addExactPid(11,1);    //exactly 1 electron
    c12.addExactPid(211,1);    //exactly 1 pi+
    c12.addExactPid(-211,1);    //exactly 1 pi-
    c12.addExactPid(2212,1);    //exactly 1 proton
    c12.addExactPid(22,2);    //exactly 2 gamma
    
    /////////c12.addZeroOfRestPid();  //nothing else
    c12.useFTBased(); //and use the Pids from RECFT
  
    //loop over all events in the file
    while(c12.next()==true){
       
      if(c12.getDetParticles().empty())
	continue;
      auto parts=c12.getDetParticles();
      
       auto electron=c12.getByID(11)[0];
       auto gamma1=c12.getByID(22)[0];
       auto gamma2=c12.getByID(22)[1];
       auto proton=c12.getByID(2212)[0];
       auto pip=c12.getByID(211)[0];
       auto pim=c12.getByID(-211)[0];

       p4_gamma1.SetXYZM(gamma1->par()->getPx(),gamma1->par()->getPy(),gamma1->par()->getPz(),0);
       p4_gamma2.SetXYZM(gamma2->par()->getPx(),gamma2->par()->getPy(),gamma2->par()->getPz(),0);

       auto pi0 = p4_gamma1 + p4_gamma2;
     
       //Fill histograms if gammas are in FD
       if(gamma1->getRegion()==FD && gamma2->getRegion()==FD){
	 hmass.Fill(pi0.M());
	 htime.Fill(gamma1->getTime() - gamma2->getTime() );
       }
     }
  
  }

  TCanvas can;
  can.Divide(2,1);
  can.cd(1);
  hmass.Draw();
  can.cd(2);
  htime.Draw();
}

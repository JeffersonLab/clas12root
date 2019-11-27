{
  BankHist bankDraw("/where/is/myHipo.hipo");
 

  bankDraw.Hist2D("REC::Particle::Px:REC::Particle::Py",100,-3,3,100,-3,3,"");
  
  bankDraw.Hist1D("REC::Particle::Pid",10000,-3000,3000,"");
  
  bankDraw.Hist1D("sqrt(REC::Particle::Px*REC::Particle::Px"
                   "+ REC::Particle::Py*REC::Particle::Py"
                   "+ REC::Particle::Pz*REC::Particle::Pz)"
		  ,200,0,10,"")->Draw("(3x1)colz");
  
  bankDraw.Save("BankHists.root");
}

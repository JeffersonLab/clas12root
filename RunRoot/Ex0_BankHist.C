{
  BankHist bankDraw("/where/is/my/file.hipo");
  bankDraw.Hist1D("REC::Event::StartTime",100,0,200,"")->Draw();

  bankDraw.Hist1D("REC::Particle::Pid",1000,-100,2500,"");
  bankDraw.Hist1D("REC::Particle::Px",100,-5,5,"");
  bankDraw.Hist1D("REC::Particle::Py",100,-5,5,"");
  bankDraw.Hist1D("REC::Particle::Pz",100,-5,5,"")->Draw("(2x2)");


  bankDraw.Hist2D("REC::Particle::Px:REC::Particle::Py",100,-3,3,100,-3,3,"");
  bankDraw.Hist1D("REC::Particle::Pid",1000,-100,2500,"");
  bankDraw.Hist1D("sqrt(REC::Particle::Px*REC::Particle::Px+REC::Particle::Py*REC::Particle::Py+REC::Particle::Pz*REC::Particle::Pz)",200,0,10,"")->Draw("(3x1)colz");
 
}

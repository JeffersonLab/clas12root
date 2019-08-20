{
  BankHist bankDraw("/work/jlab/clas12data/dst_skim4_5038.hipo");
  bankDraw.Hist1D("REC::Event::StartTime",100,0,200,"")->Draw();

  bankDraw.Hist1D("REC::Particle::Pid",1000,-100,2500,"");
  bankDraw.Hist1D("REC::Particle::Px",100,-5,5,"");
  bankDraw.Hist1D("REC::Particle::Py",100,-5,5,"");
  bankDraw.Hist1D("REC::Particle::Pz",100,-5,5,"")->Draw("(2x2)");


}

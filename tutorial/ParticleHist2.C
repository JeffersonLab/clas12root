{

  //Draw HTCC photon number versus particle momentum 
  hists.Hist2D("PBANK.P:HTCC.Nphe",200,0,10,50,0,50,"");
  //Draw "speed" versus beta
  hists.Hist2D("PBANK.Beta:FTOF1A.Path/(FTOF1A.Time-EVNT.StartTime)",
	       200,0,1,100,0,100,"");
  //Draw EC inner enerfy verus particle momentum
  hists.Hist2D("P.P : ECIN.Energy",200,0,10,100,0,1,"");
  //Draw sum of calorimeter energy versus region particle Det energy for e- in FD
  hists.Hist2D("P.DetEnergy : ECOUT.Energy + ECIN.Energy +PCAL.Energy",
	       200,0,3,100,0,3,"P.Pid==11&&P.Region==FD")->Draw("(2x2)col1");
}

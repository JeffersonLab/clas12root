{
  //Draw electron momentum
  hists.Hist1D("P.P",200,0,10,"P.Pid==11");
  //calculate electron momentum from REC::Particle bank
  hists.Hist1D("sqrt(PBANK.Px*PBANK.Px+PBANK.Py*PBANK.Py+PBANK.Pz*PBANK.Pz)"
	       ,200,0,10,"P.Pid==11")->Draw("");

  //Caclulate particle time of flight
  hists.Hist1D("P.Time-EVNT.StartTime",1000,0,100,"P.Time");
  //calculate the time difference between FTOF layers
  hists.Hist1D("FTOF1A.Time-FTOF1B.Time",1000,-10,10,
	       "FTOF1A.Time&&FTOF1B.Time");
  //Calculate Time of flight for CTOF
  hists.Hist1D("CTOF.Time-EVNT.StartTime",1000,0,100,"CTOF.Time");
  //Check the difference between FT start time and FD startime
  hists.Hist1D("EVNT.FTBStartTime-EVNT.StartTime",1000,-200,200,
	       "EVNT.FTBStartTime")->Draw("(2x2)");

}


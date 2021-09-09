//particleTree /dir/file.hipo out.root Ex4_TreeMaker.C
{
  //treemaker.SetEntries(1E5);//only process given number of events
  //add event header branch, includes start time
  //treemaker.UseEventData();
  
  //make branch with given formula and alias it to name Time
  //give branch type with /F = float etc.
  treemaker.Branch("P.Time-PBANK.Vt/F","ToF");
  treemaker.Branch("P.Time-PBANK.FTBVt/F","FTBToF");
  treemaker.Branch("P.Time/F");
  treemaker.Branch("P.Path/F");
  treemaker.Branch("P.DetEnergy/P.P/F","SampFrac");
  treemaker.Branch("HTCC.Nphe/F");

  treemaker.Branch("PBANK.Px/F");
  treemaker.Branch("PBANK.Py/F");
  treemaker.Branch("PBANK.Pz/F");
  treemaker.Branch("PBANK.Vx/F");
  treemaker.Branch("PBANK.Vy/F");
  treemaker.Branch("PBANK.Vz/F");
  treemaker.Branch("PBANK.Pid/I");
  treemaker.Branch("PBANK.FTBPid/I");//FT based PID

  //Or for banks without links create one yourself
  //Useful for trajectories which require specific detector elements
  //!!!! WARNING the label e.g. TRAJFTOFFTOF1A must only contain alphanumeric characters !!!!!
  //treemaker.CreateBankLink("TRAJFTOFFTOF1A","p->traj(FTOF,FTOF1A)->");
  //treemaker.Branch("TRAJFTOFFTOF1A.X/F");
  //treemaker.Branch("TRAJFTOFFTOF1A.Y/F");
  //treemaker.Branch("TRAJFTOFFTOF1A.Z/F");

  //e.g. Only save electron information
  //treemaker.AddParticleCut("PBANK.Pid==11");

  //Event topology cuts
  //treemaker.AddAtLeastPid(211,1); //at least 1 pi+
  // treemaker.AddExactPid(11,1);    //exactly 1 electron
  // treemaker.AddZeroOfRestPid();  //nothing else, if not this line any of anything else
 
  treemaker.Fill();
}

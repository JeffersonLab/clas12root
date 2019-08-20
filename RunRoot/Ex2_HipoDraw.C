//particleDraw /dir/file.hipo Ex2_HipoDraw.C
{

  hists.Hist1D("P.Theta*TMath::RadToDeg()",180,0,180,"PBANK.Px");
  hists.Hist1D("P.Phi*TMath::RadToDeg()",180,-180,180,"PBANK.Px");
  hists.Hist1D("P.P",100,0.1,12,"PBANK.Px");
  hists.Hist1D("P.Time-EVNT4.StartTime",1000,-200,200,"P.Time&&PBANK.Px"); //Note zero suppression

  hists.Hist1D("atan2(sqrt(PBANK.Px*PBANK.Px+PBANK.Py*PBANK.Py),PBANK.Pz)*57.295780",180,0,180,"P.Time&&PBANK.Px");
  hists.Hist1D("atan2(PBANK.Py,PBANK.Px)*TMath::RadToDeg()",180,-180,180,"P.Time&&PBANK.Px");
  hists.Hist1D("sqrt(PBANK.Px*PBANK.Px+PBANK.Py*PBANK.Py+PBANK.Pz*PBANK.Pz)",100,0.1,12);
  hists.Hist1D("FTOF1B.Time-EVNT4.StartTime",1000,-200,200,"P.Time&&PBANK.Px&&PBANK.Px");

  hists.Hist1D("P.Theta*TMath::RadToDeg()",180,0,180,"P.Pid==11&&PBANK.Px");
  hists.Hist1D("P.Phi*TMath::RadToDeg()",180,-180,180,"P.Pid==11&&PBANK.Px");
  hists.Hist1D("P.P",100,0.1,12,"P.Pid==11");
  hists.Hist1D("P.Time-EVNT4.StartTime",1000,-200,200,"P.Time&&P.Pid==11&&PBANK.Px");

  hists.Hist1D("P.Theta*TMath::RadToDeg()",180,0,180,"P.Pid==2212&&PBANK.Px");
  hists.Hist1D("P.Phi*TMath::RadToDeg()",180,-180,180,"P.Pid==2212&&PBANK.Px");
  hists.Hist1D("P.P",100,0.1,12,"P.Pid==2212&&PBANK.Px");
  hists.Hist1D("P.Time-EVNT4.StartTime",1000,-200,200,"P.Time&&P.Pid==2212&&PBANK.Px")->Draw("(4x3)");

  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==11");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==-11");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==22");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==2212");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==211");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==-211");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==321");
  hists.Hist2D("P.Theta*TMath::RadToDeg():P.Phi*TMath::RadToDeg()",180,0,180,180,-180,180,"P.Pid==-321")->Draw("(4x2)col1");

  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FT");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==CD")->Draw("(2x2)col1");
  
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==11");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==-11");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==22");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==2212");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==211");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==-211");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==321");
  hists.Hist2D("P.P:P.Beta",100,0,10,100,0,2,"P.Beta&&P.Region==FD&&P.Pid==-321")->Draw("(2x2)col1");

  hists.Hist1D("P.CalcMass",100,0.05,2,"P.Time");
  hists.Hist1D("P.CalcMass",100,0.05,2,"P.Time&&P.Region==FT");
  hists.Hist1D("P.CalcMass",100,0.05,2,"P.Time&&P.Region==FD");
  hists.Hist1D("P.CalcMass",100,0.05,2,"P.Time&&P.Region==CD")->Draw("(2x2)");
}

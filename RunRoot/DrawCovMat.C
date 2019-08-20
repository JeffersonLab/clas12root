//         clas12root
//         BankHist hists("/WHERE/IS/MY/HIPO/file.hipo");
{
  hists.Hist1D("CovMat.C22",100,1E-10,30E1,"CovMat.C22&&P.Region==FD");
  hists.Hist1D("CovMat.C33",100,1E-10,2E-3,"CovMat.C33&&P.Region==FD");
  hists.Hist1D("CovMat.C55",100,1E-10,60E-5,"CovMat.C55&&P.Region==FD");
  
  hists.Hist1D("CovMat.C22",100,1E-10,30E-6,"CovMat.C22&&P.Region==CD");
  hists.Hist1D("CovMat.C33",100,1E-10,2E-3,"CovMat.C33&&P.Region==CD");
  hists.Hist1D("CovMat.C55",100,1E-10,60E-6,"CovMat.C55&&P.Region==CD")->Draw("(3x2)");

}

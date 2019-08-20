//Load all the libraries for use in PROOF-lite
// to set the workers run like :
//clas12root 'LoadProofLib.C(N)'
void LoadProofLib(Int_t Nworkers=1){


  Bool_t ISMAC=kFALSE;
  if(TString(gSystem->Getenv("OSTYPE"))==TString("darwin"))
    ISMAC=kTRUE;
  
  gSystem->Load("libProof");
  
  TProof *proof =nullptr;
  if(!gProof) 
    proof = TProof::Open("://lite");
  else
    proof=gProof;
  
  Int_t NCores=Nworkers;
  proof->SetParallel(NCores);
  if(gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/liblz4.so",kTRUE)==0){}
  // get the sandbox directroy
  TString sandbox="~/.proof";
  if(TString(gEnv->GetValue("ProofLite.Sandbox",""))!=TString()){
    sandbox=gEnv->GetValue("ProofLite.Sandbox","");
  }
  //  copy pcm files to sandbox/cache
  gSystem->Exec(Form("cp $CLAS12ROOT/lib/libHipo4_rdict.pcm %s/cache/.",sandbox.Data()));
  gSystem->Exec(Form("cp $CLAS12ROOT/lib/libClas12Banks_rdict.pcm %s/cache/.",sandbox.Data()));
  gSystem->Exec(Form("cp $CLAS12ROOT/lib/libClas12Root_rdict.pcm %s/cache/.",sandbox.Data()));

  if(!ISMAC){
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libHipo4.so",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Banks.so",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Root.so",kTRUE);
  }
  else{
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libHipo4.dylib",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Banks.dylib",kTRUE);
    gProof->Load(TString(gSystem->Getenv("CLAS12ROOT"))+"/lib/libClas12Root.dylib",kTRUE);

  }
  
}

# Clas12Tool

Data Analysis Tools for hipo4 data format.


Examples are given for running in interactive ROOT sessions and ROOT-Jupyter notebooks.


## Clas12Banks -> Clas12Root

NEW We now use an external hipo4 repository. This must be pointed at with the variable HIPO when installing. The files from hipo/hipo4 will be copied here to Hipo4.

The Hipo c++ reader library can be used independent of specific banks and ROOT, but depends on Hipo.

The Clas12Banks implementation can be used independent of ROOT, although currently ROOT dictionaries are created for the classes via cmake (this could be removed). This defines the specific CLAS12 DST banks and provides an interface to the data.

For actual Clas12Banks definitions see [HIPO4 DSTs](https://clasweb.jlab.org/wiki/index.php/CLAS12_DSTs)

## Also see c++ function for accessing banks "Cheat sheet" AccesssingBankDataInCpp.txt in the top level directory.

The Clas12Root package depends on both Hipo and Clas12Banks. This provides ROOT-like analysis tools for operating on clas12 hipo DSTs.

    HipoDraw
    HipoTreeMaker
    HipoProof

## To Download

git clone --recurse-submodules https://github.com/dglazier/Clas12Tool.git

cd Clas12Tool

git checkout restructure

## To setup Run ROOT

for cshrc

setenv CLAS12TOOL $PWD  (the actual path can be added in your bashrc or tchrc)

setenv PATH "$PATH":"$CLAS12TOOL/bin"

setenv HIPO /Where/Is/hipo

or for bash

export CLAS12TOOL=$PWD

export PATH="$PATH":"$CLAS12TOOL/bin"

export HIPO=/Where/Is/hipo

## To install (either Hipo3 or Hipo4)

   installC12Root


If there are issues with cmake and your ROOTSYS you can try using the local FindROOT file. Edit the CMakeList.txt.hipo3 or 4 files removing the lines with comment ##USEROOTSYS and uncomment the line

   	 #######include("cmake/FindROOT.cmake")

**Note just to help confuse you the first letter in bank items have been capitilised so where you might expect to use REC::Particle::pz, or particle->getpz(), you will find Pz will work better, similarly Time, Energy...You can check the bank header files e.g. Clas12Banks4/particle.h and look at the get function declarations e.g. getPz() rather than getpz() **
	 
## interactive root session

To start an interactive session with pre-loaded Clas12Root use clas12root instead of root on the command line.

## Ex 0 Plotting an item from any bank
 
This is faster than the particle draw as it only requires the reading of 1 bank.

         clas12root

         BankHist bankDraw("/WHERE/IS/MY/HIPO/file.hipo");
         bankDraw.Hist1D("REC::Particle::Pz",100,0,10,"")->Draw()
         bankDraw.Hist1D("REC::Scintillator::Time",1000,0,200,"")->Draw()

You can group histograms together for lazy execution if they all come from the same bank.

	       bankDraw.Hist1D("REC::CovMat::C11",100,0,1,"")
	       bankDraw.Hist1D("REC::CovMat::C22",100,0,1,"")
	       bankDraw.Hist1D("REC::CovMat::C33",100,0,1,"")
	       bankDraw.Hist1D("REC::CovMat::C44",100,0,1,"")	
	       bankDraw.Hist1D("REC::CovMat::C55",100,0,1,"")->Draw("(3x2)")


## Ex 1  Looping over events and getting particle information

The clas12reader class performs the correlation of particle and detector inofmation (aka reverse indexing). When looping over particles you are looping over region_particle (see Clas12Banks for full reference). Each region, FT, FD and CD has its own definition of a region_particle so it will only return meaningful data (i.e. a CD particle will return 0 for FD detector information). In addition the getTime, getPath, getDetEnergy functions have predefined meaning for each region, e.g. for FT getTime returns FTCAL time, for FD it returns FTOF1A if it exists, if not it will try FTOF1B, FTOF2 then PCAL.

  ###NEW you can add hipo file tags to clas12reader e.g.

         clas12reader c12("file.hipo",{0,1,6});

You can insepct the code [$CLAS12TOOL/RunRoot/Ex1_CLAS12Reader.C](https://github.com/dglazier/Clas12Tool/blob/master/RunRoot/Ex1_CLAS12Reader.C) for more guidance on how to use it.

To run:

       clas12root $CLAS12TOOL/RunRoot/Ex1_CLAS12Reader.C+ --in=/WHERE/IS/MY/HIPO/file.hipo

Note the use of the + sign after the macro name. This compiles the script meaning it will run much faster.

### Jupyter

To install rootbooks see https://root.cern.ch/how/how-create-rootbook

mkdir myNotebooks

cp -r $CLAS12TOOL/RunRoot/jupy myNotebooks/.

cd myNotebooks/jupy

Start a ROOT note book :

      	root --notebook

Click on the notebook CLAS12Reader3Pi.ipynb and follow the tutorial


## Ex 2 Drawing particle histograms from hipo files

       particleDraw /WHERE/IS/MY/HIPO/file.hipo

Or chain together files with wildcard, note the ' '

       particleDraw '/WHERE/IS/MY/HIPO/file_*.hipo'


You will get an interactive ROOT prompt where you can draw histograms:

    ParticleHist [0] hists.Hist1D("P.P",100,0,10,"P.P")
    ParticleHist [1] hists.Hist2D("P.P:P.DetEnergy",100,0,10,100,0,5,"P.P")->Draw("(2x1)")

Note you only have to call draw once, and then it only has to loop over the date once. The option (2x1) specifies the dimensions of the pads in the produced canvas, the parenthesis is required.

Remember at the end you can save all the histograms to file :

    ParticleHist [0] hists.Save("HistFile.root");

Instead of drawing histograms interactively at the prompt you may give predefined histograms via a script e.g. :

	particleDraw /WHERE/IS/MY/HIPO/file.hipo Ex2_HipoDraw.C

See $CLAS12TOOL/RunRoot/Ex2_HipoDraw.C for details.

There are predefined aliases for DST bank detector layers :

        ECIN. , ECOUT. , PCAL. , FTOF1A. , FTOF1B. , FTOF2. , CTOF. , CND1. , CND2. , CND3. , FTCAL. , FTHODO. , HTCC. , LTCC. , DC. , CVT. 
        e.g. ECIN.Energy  , HTCC.Nphe , DC.TrChi2 , CTOF.Time

The REC::Particle bank should be directly accessed with 

        PBANK.
        e.g. PBANK.Pid , PBANK.Px

The FT based equaivalent PID varaibles can be accessed from the particle bank by

        e.g. PBANK.FTBPid , PBANK.FTBBeta
        
The region particle has derived quantities such as theta and phi as well as selected variabels for a particle for example time from a particular ToF layer. Note the order of precedence for the FD is TOF1B, TOF1A, TOF2, PCAL and DeltaEnergy is the corresponding timing detector energy. These should be accessed with 

        P.
        e.g. P.Theta , P.P , P.Phi , P.Region , P.Time , P.DetEnergy , P.DeltaEnergy , P.Path , P.Pid , P.CalcMass
	e.g. P.Region==FT,  P.Region==FD

For REC::EVNT use (adding FTB for RECFT::EVNT banks)

        e.g. EVNT.StartTime or EVNT.FTBStartTime

For Run::config

       e.g. RUN.Trigger

### Jupyter

Go to directory containing notebooks e.g. $CLAS12ROOT/RunRoot/jupy

Start a ROOT note book :

      	root --notebook
	
Click on the notebook HipoDraw.ipynb and follow the tutorial

## Ex 4 Filtering and Skimming into a ROOT ntuple (tree)

       particleTree  /WHERE/IS/MY/HIPO/file.hipo /OUTPUT/tree.root Ex4_TreeMaker.C
       
Or chain together files with wildcard, note the ' '

       particleTree  '/WHERE/IS/MY/HIPO/file_*.hipo' /OUTPUT/tree.root Ex4_TreeMaker.C

The script $CLAS12ROOT/RunRoot/Ex4_TreeMaker.C defines which branches are to be written and what cuts to put on the event topology. You can copy and edit this file to do what you want. The branches should use the conventions above for accessing different bank items e.g.

     treemaker.Branch("P.Time/F"); //create tree with time branch
     treemaker.Branch("PBANK.Px/F"); //create tree with particle Px branch


You can perform some arithmetic and define a new branch e.g.

    	treemaker.Branch("P.Time-EVNT.StartTime/F","Time"); //branch name Time
  	treemaker.Branch("P.Time-EVNT.FTBStartTime/F","FTBTime"); //branch name FTBTime


     treemaker.AddExactPid(11,1); //filter events with exactly 1 e-
     treemaker.AddAtLeastPid(211,1);//and at least 1 pi+
     treemaker.AddZeroOfRestPid(); //and zero of any other particle type (default is any)

### Jupyter

Go to directory containing notebooks e.g. $CLAS12ROOT/RunRoot/jupy

Start a ROOT note book :

      	root --notebook

Click on the notebook HipoToRootTree.ipynb and follow the tutorial


## Ex 3 Using HipoSelector & PROOFLite


This assumes you are aware of and understand the ROOT TSelector and PROOF scheme. See https://root.cern.ch/proof.

Create a HipoSelector (similar to tree->MakeSelector("mySelector");), using the executable makeHipoSelector :

       makeHipoSelector mySelector

You should use some meaningful name rather than mySelector.
Edit it to perform the tasks you would like. But use the ProcessEvent function instead of the Process function as you would in a TSelector. You can use the _c12 clas12reader object to access all the data as shown in Ex1_CLAS12Reader.C

e.g.

	 Bool_t HipoFileSelector::ProcessEvent(){

   	    _hist1->Fill(_c12->head()->getStartTime());
    	    return kTRUE;    
  	 }

You may also add event selections as above using the AddFilter function

         void testSelector::AddFilter(){
              _c12->addExactPid(11,1);    //exactly 1 electron
          }


To execute (note the + is important) :


       clas12proof 4 mySelector.C+ Ex3_ProofLite.C

Note 4 = number of workers used, you should change this to however many you would like.

Note mySelector is hard-coded in Ex3_ProofLite.C so for your own selector you should copy and edit this script.

As a more complete example you can check testSelector in RunRoot which implements the particle analysis and histogramming from Ex1. This can be run with Ex3b_TestSelector.C once you change the HipoChain files :

         clas12proof 8 RunRoot/testSelector.C+  RunRoot/Ex3b_TestSelector.C


### Jupyter

Go to directory containing notebooks e.g. $CLAS12ROOT/RunRoot/jupy

Start a ROOT note book :

      	root --notebook

Click on the notebook CreateHipoSelector.ipynb and follow instructions. This creates the selector you wish to run.

Once this is complete you should open the notebook HipoProof.ipynb and process the selector with PROOF

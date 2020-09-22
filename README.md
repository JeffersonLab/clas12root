# Clas12Root

Data Analysis Tools for hipo4 data format.


Examples are given for running in interactive ROOT sessions and ROOT-Jupyter notebooks.


## Clas12Banks -> Clas12Root

We now use an external hipo4 repository. This must be pointed at with the variable HIPO when installing. The files from hipo/hipo4 will be copied here to Hipo4.

### NEW

A default hipo implementation is now packed with clas12root. If you prefer to use this do not set the enviroment variable HIPO. If you would like to use a different version of the hipo library set HIPO. You may get the most up to data hipo library from


For Hipo library see https://github.com/gavalian/hipo 

The Clas12Banks implementation can be used independent of ROOT, although currently ROOT dictionaries are created for the classes via cmake (this could be removed). This defines the specific CLAS12 DST banks and provides an interface to the data.

For actual Clas12Banks definitions see [HIPO4 DSTs](https://clasweb.jlab.org/wiki/index.php/CLAS12_DSTs)

An interface to the c++ [Run Conditions DataBase](https://github.com/JeffersonLab/rcdb/wiki/Cpp) requires downloading the relevant code from https://github.com/JeffersonLab/rcdb . This is optional and depends on the existence of an environment variable containing the path to the RCDB code. The interface also requires having MySQL installed.

clas12root provides an interface to the clasqaDB c++ code to allow skimming of events based on the Data Quality Assurance. This is optional and depends on the existence of an environment variable containing the path to the clasqaDB code, which must be downloaded from https://github.com/c-dilks/clasqaDB/tree/master.

## Also see c++ function for accessing banks "Cheat sheet" AccesssingBankDataInCpp.txt in the top level directory.

The Clas12Root package depends on both Hipo and Clas12Banks. This provides ROOT-like analysis tools for operating on clas12 hipo DSTs.

    HipoDraw
    HipoTreeMaker
    HipoProof

## To Download
```bash
git clone --recurse-submodules https://github.com/jeffersonlab/clas12root.git
cd clas12root
#To download the RCDB repository
git clone --recurse-submodules https://github.com/jeffersonlab/rcdb.git
#To download the clasqaDB repository 
git clone --recurse-submodules https://github.com/c-dilks/clasqaDB.git
```

## To setup Run ROOT

for cshrc
```sh
setenv CLAS12ROOT $PWD  (the actual path can be added in your bashrc or tchrc)
setenv PATH "$PATH":"$CLAS12ROOT/bin"
setenv HIPO /Where/Is/hipo
#To use the RCDB interface 
setenv RCDB_HOME /Where/Is/rcdb
#To use clasqaDB interface
setenv CLASQADB_HOME /Where/Is/clasqaDB
```

or for bash
```bash
export CLAS12ROOT=$PWD
export PATH="$PATH":"$CLAS12ROOT/bin"
export HIPO=/Where/Is/hipo
#To use the RCDB interface 
export RCDB_HOME /Where/Is/rcdb
#To use clasqaDB interface
export CLASQADB_HOME /Where/Is/clasqaDB
```

## To install

Note on ifarm depending on which version on ROOT and gcc you are using you may have to set the CC and CXX variables or else you will get library linking errors. To do this you will need something like,

```sh
set CC =  `which gcc`
set CXX = `which g++`
```
Or just set the paths to CC and CXX directly.

```bash
installC12Root
```

If there are issues with cmake and your ROOTSYS you can try using the local FindROOT file. Edit the CMakeList.txt files removing the lines with comment ##USEROOTSYS and uncomment the line

   	 #######include("cmake/FindROOT.cmake")

**Note just to help confuse you the first letter in bank items have been capitilised so where you might expect to use `REC::Particle::pz`, or `particle->getpz()`, you will find Pz will work better, similarly Time, Energy...You can check the bank header files e.g. Clas12Banks4/particle.h and look at the get function declarations e.g. `getPz()` rather than `getpz()` **

## Running with docker

If you have docker installed you can also build and run clas12root in a container. 

To build the container you can use the same [download](#to-download) procedure and then once in the cals12root folder run.

```bash
docker build -t clas12root .
```

Once the container is built you can run the clas12root interactive session with:

```bash
docker run --rm -it -v $PWD:/work -w /work clas12root
```

While in the folder you can use this to run some of the examples.

```bash
docker run --rm -it -v $PWD:/work -w /work clas12root RunRoot/Ex8_RcdbReader.C
```

You can also run a jupyter server from the dockerfile with. 

```bash
docker run --rm -it -p 8888:8888 -v $HOME/Data:/data -v $PWD:/work -w /work --entrypoint='jupyter' clas12root notebook --allow-root --ip=0.0.0.0 --port=8888
```

After you run this copy and paste the url from the terminal into a web browser and start coding.


## interactive root session

To start an interactive session with pre-loaded Clas12Root use clas12root instead of root on the command line.

## Ex 0 Plotting an item from any bank
 
This is faster than the particle draw as it only requires the reading of 1 bank.

         clas12root

         BankHist bankDraw("/WHERE/IS/MY/HIPO/file.hipo");
         bankDraw.Hist1D("REC::Particle::Pz",100,0,10,"")->Draw()

Note you can only work with 1 bank at a time, so you cannot then run

         bankDraw.Hist1D("REC::Scintillator::Time",1000,0,200,"")->Draw()

but can do

         bankDraw.Hist1D("REC::Particle::Px",100,-10,10,"")->Draw()

You can group histograms together for lazy execution if they all come from the same bank.

	       bankDraw.Hist1D("REC::CovMat::C11",100,0,1,"")
	       bankDraw.Hist1D("REC::CovMat::C22",100,0,1,"")
	       bankDraw.Hist1D("REC::CovMat::C33",100,0,1,"")
	       bankDraw.Hist1D("REC::CovMat::C44",100,0,1,"")	
	       bankDraw.Hist1D("REC::CovMat::C55",100,0,1,"")->Draw("(3x2)")

### NEW

  You can set the number of events to process :

           bankDraw.SetEntries(1E6);

## Ex 1  Looping over events and getting particle information

The clas12reader class performs the correlation of particle and detector inofmation (aka reverse indexing). When looping over particles you are looping over region_particle (see Clas12Banks for full reference). Each region, FT, FD and CD has its own definition of a region_particle so it will only return meaningful data (i.e. a CD particle will return 0 for FD detector information). In addition the getTime, getPath, getDetEnergy functions have predefined meaning for each region, e.g. for FT getTime returns FTCAL time, for FD it returns FTOF1A if it exists, if not it will try FTOF1B, FTOF2 then PCAL.

You can add hipo file tags to clas12reader e.g.

         clas12reader c12("file.hipo",{0,1,6});

You can insepct the code [$CLAS12ROOT/RunRoot/Ex1_CLAS12Reader.C](https://github.com/jeffersonlab/clas12root/blob/master/RunRoot/Ex1_CLAS12Reader.C) for more guidance on how to use it.

To run:

       clas12root $CLAS12ROOT/RunRoot/Ex1_CLAS12Reader.C+ --in=/WHERE/IS/MY/HIPO/file.hipo

Note the use of the + sign after the macro name. This compiles the script meaning it will run much faster.



You can now try running this using HipoChain to process many files. See the example RunRoot/Ex1_CLAS12ReaderChain.C . This should look like,

    	clas12root::HipoChain chain;
	//Add lots of files
	chain.Add("/WHERE/IS/MY/HIPO/file1.hipo");
	auto c12=chain.GetC12Reader();
	c12->addExactPid(11,1); //if want some filters
	while (chain.Next()){
     	      c12=chain.GetC12Reader();
 	      auto electrons=c12->getByID(11);
	      ....
	}

run with

	clas12root RunRoot/Ex1_CLAS12ReaderChain.C+


Also you can check trigger bits directly

     	 c12.checkTriggerBit(24);
	 
### Jupyter

To install rootbooks see https://root.cern.ch/how/how-create-rootbook

       mkdir myNotebooks
       cp -r $CLAS12ROOT/RunRoot/jupy myNotebooks/.
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

	particleDraw /WHERE/IS/MY/HIPO/file.hipo $CLAS12ROOT/RunRoot/Ex2_HipoDraw.C

See $CLAS12ROOT/RunRoot/Ex2_HipoDraw.C for details.

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


### NEW

  You can set the number of events to process :

    ParticleHist [0] hists.SetEntries(1E6);


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

## Ex 5  Writing out specific events to a hipo file

The clas12writer class allows you to write out specific events to a new hipo file. The idea behind this is to avoid repeating the same event selection everytime you want to access information about a specific set of events. The writer is assigned a clas12reader from which it gets the event information, and is initialised with the desired location for the outputted hipo file. You can also choose not to write out certain banks to speed the process up.

You can insepct the code [$CLAS12ROOT/RunRoot/Ex5_CLAS12Writer.C](https://github.com/jeffersonlab/clas12root/blob/master/RunRoot/Ex1_CLAS12Writer.C) for more guidance on how to use it.

To run:

       clas12root $CLAS12ROOT/RunRoot/Ex5_CLAS12Writer.C+

Note the use of the + sign after the macro name. This compiles the script meaning it will run much faster. The script will then ask you for the locations of an input hipo file and an output file. The script is similar to Ex1_CLAS12Writer.C so you can compare the two.

## Ex 7 Scaler beam charge information

To normalise events you will need the beam charge. This is stored in 2 ways in clas12 DSTs. (1) accumalating in REC::Event::beamCharge (2) accumulating in tag=1 RAW::scaler::fcupgated . The problem with (1) for trains is that the events are not garaunteed to be in correct time order, so the last event in the hipo file may not have the largest beamCharge. And indeed if you have a large event filter you may not include any events with the largest beam cahrge in your train. (2) contains all the scaler reads and therefore the correct total charge, but these events are not in time order and the are not interspersed with the correct hipo events (I think).

To make sure the correct beam charge is accessed and time history rates can be produced for a file there is a scaler_reader class. At file initialisation this will read all the tag=1 events (which is quite fast) and sort all the scaler events by event number. It will return the difference between the highest and lowest beamCharge, so it should work for individual run files as well as files with whole runs. It also allows you to add counters for any monitoring and will increment REC::* information with the correct associated scaler event.

For example,

    clas12reader c12("file.hipo",{0}); //reader for tag 0 events

    //create the scaler reader and make some counters
    //the counters have an entry for each scaler read
    //you can increment them for each real event corresponding
    //to the scaler read in the normal event loop
    auto scal=c12.scalerReader();
    auto count_mesonex=scal->addLongCounter();
    auto count_events=scal->addLongCounter();
    while(c12.next()==true){
       auto currEvent=c12.runconfig()->getEvent(); 
       //check for valid charge events, i.e. RUN::Scaler::BeamCharge!<0
       if( scal->validCharge(currEvent) ==false ) continue;
       if( c12.checkTriggerBit(24) ) //check if mesonex trigger fired for this event
	   scal->incrementLong(count_mesonex,currEvent,1);
       //counter for all events
       scal->incrementLong(count_events,currEvent,1);

     }

Be aware that if your events have been filtered this will effect the rates.

To try the example

       clas12root --in=/work/jlab/clas12data/v16/skim9_5038.hipo RunRoot/Ex7_ScalerReader.C+

### Jupyter

Go to directory containing notebooks e.g. $CLAS12ROOT/RunRoot/jupy

Start a ROOT note book :

      	root --notebook

Click on the notebook CLAS12Writer3Pi.ipynb and follow the tutorial

## Ex 8 Reading from the Run Conditions DataBase

clas12reader can make a connection to the RCDB database and download the relevent information for a run. The downloaded information can be accessed via a rcdb_vals struct,

      clas12reader c12("/a/hipo/file.hipo");
      c12.queryRcdb(); //only check rcdb values when asked to (and RCDB_HOME was set)
      auto rcdbData=c12.getRcdbVals();//struct with all relevent rcdb values
      auto energy = rcdbData.beam_energy;
      
An interface to the run conditions database is implemented by the class rcdb_reader. It will open a connection to rcdb@clasdb.jlab.org/rcdb and allow you to retrieve condition values for a given run. This class interfaces to the Run Conditions DataBase c++ code, more information on it can be found at https://github.com/JeffersonLab/rcdb/wiki/Cpp and the database itself can be viewed at https://clasweb.jlab.org/rcdb/. A full list of the relevent variables can be found at https://clasweb.jlab.org/rcdb/conditions/

An example on how to use the raw rcdb_reader interface can be found in RunRoot/Ex8_RcdbReader.C. But users should just use the clas12reader functions rather than handling rcdb_reader directly to limit the number of connections and queries as given in Ex8b_RcdbReader.C.


To try the example

       clas12root RunRoot/Ex8b_RcdbReader.C+
 
In the case where many files are to be analysed use of HipoChain is recommended and this includes some rcdb features. Once a chain of file is created it can be used to access the RCDB and download the info for those files. This information is then saved locally in a root file which can be used directly when processing the HipoChain. Reading of the local rcdb file is then already automated in HipoSelector

      clas12root::HipoChain chain;
      chain.Add("/dir/files_*.hipo");//To creat rcdb data RCDB_HOME must be set prior to installation
      chain.WriteRcdbData("rcdb.root"); //Must use this first time to create local copy
      //Then when we have local copy can just use the following
      chain.SetRcdbFile("rcdb.root");


See also Ex1_CLAS12ReaderChain.C for the relevent lines.

## Ex 9 Skimming Based on Data Quality Assurance
clas12root can use the Quality Assurance database .json files found at https://github.com/c-dilks/clasqaDB/tree/master to reject events that have been identified as failing to meet certain requirements. This is implemented in an analysis using the clas12reader with the functions

      c12.applyQA("/absolute/path/to/qaDB.json");
      c12.getQAReader()->requireOkForAsymmetry(true);
      c12.getQAReader()->requireGolden(true);
      c12.getQAReader()->addQARequirement("MarginalOutlier");	

where applyQA takes as argument a .json file containing the QA database. requireOkForAsymmetry(true) requires only events that were identified as suitable for asymmetry calculations, and requireGolden(true) requires only events without any defects. addQARequirement("Requirement") allows to reject events that fail to meet the specified requirement. These can be:

    TotalOutlier: outlier N/F, but not terminal, marginal, or sector loss
    TerminalOutlier: outlier N/F of first or last file of run
    MarginalOutlier: marginal outlier N/F, within one stddev of cut line
    SectorLoss: N/F diminished within a sector for several consecutive files
    LowLiveTime: live time < 0.9
    Misc: miscellaneous defect

The QA database is contained in several .json files that can be found on the clasqaDB github [repository](https://github.com/c-dilks/clasqaDB/tree/master). These can be merged within clas12root using the jsonFileMerger class with the functions:

    jsonFileMerger merger("/absolute/path/for/output.json");
    merger.addFile("/absolute/path/for/input1.json");
    merger.addFile("/absolute/path/for/input2.json");
    merger.mergeAllFiles();

Example usage can be found at RunRoot/Ex9_QualityAssurance.C. More information on the Quality Assurance process can be found in the RGA analysis note.

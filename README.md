# Clas12Root

Data Analysis Tools for hipo4 data format.


Examples are given for running in interactive ROOT sessions and ROOT-Jupyter notebooks.

From version 1.9.0 an external `hipo` install is required before building clas12root

We now use an external `hipo` repository. This must be pointed at with the environment variable `$HIPO` when installing.

For convenience, the `hipo` source code is included here in `hipo_src/` as a submodule, and you may compile and install it with `./installHIPO`;
the default installation location is the top-level directory `hipo/`.

You may get the most up to date `hipo` source code from <https://github.com/gavalian/hipo>

## Clas12Banks and Clas12Root

The Clas12Banks implementation can be used independent of ROOT, although currently ROOT dictionaries are created for the classes via cmake (this could be removed). This defines the specific CLAS12 DST banks and provides an interface to the data.

For actual Clas12Banks definitions see [HIPO4 DSTs](https://clasweb.jlab.org/wiki/index.php/CLAS12_DSTs)

An interface to the c++ [Run Conditions DataBase](https://github.com/JeffersonLab/rcdb/wiki/Cpp) requires downloading the relevant code from https://github.com/JeffersonLab/rcdb . This is optional and depends on the existence of an environment variable containing the path to the RCDB code. The interface also requires having MySQL installed.

clas12root provides an interface to the clas12-qadb c++ code to allow skimming of events based on the Data Quality Assurance. This is optional and depends on the existence of an environment variable containing the path to the clas12-qadb code, which must be downloaded from https://github.com/c-dilks/clas12-qadb/tree/master.


To simplify installation of the dependencies, ccdb, rcdb, qadb are now includes as submodules tagged to specific releases. Now when you clone with   --recurse-submodules all 3 plus lz4 will also be downloaded into your clas12root directory. If you already have your own versions of these you may ignore these and just set the required paths to your own installation.

It is still required to build ccdb with CMake after you have cloned it (before running installC12Root); see below for example CMake commands. You will need to make sure you have the necessary depndencies for ccdb on your system. If you do not and do not want to use ccdb in anycase you may just not set the CCDB_HOME enviroment variable.  [https://github.com/JeffersonLab/ccdb]

ccdb is prone to giving warnings when you try and compile ROOT scripts via macros. To get rid of these wanrings you may need to copy the Directory.h file from ccdb_patch.

      cp $CLAS12ROOT/ccdb_patch/Directory.h $CCDB_HOME/include/CCDB/Model/Directory.h


## Also see c++ function for accessing banks "Cheat sheet" AccesssingBankDataInCpp.txt in the top level directory.

The Clas12Root package depends on both Hipo and Clas12Banks. This provides ROOT-like analysis tools for operating on clas12 hipo DSTs.

    HipoDraw
    HipoTreeMaker
    HipoProof

## To Download
```bash
git clone --recurse-submodules https://github.com/jeffersonlab/clas12root.git
cd clas12root



## To setup Enviroment

for cshrc
```sh
setenv CLAS12ROOT $PWD  (the actual path can be added in your bashrc or tchrc)
setenv PATH "$PATH":"$CLAS12ROOT/bin"
#To use the RCDB interface 
setenv RCDB_HOME /Where/Is/rcdb  (e.g. setenv RCDB_HOME ${CLAS12ROOT}/rcdb )
#To use the CCDB interface 
setenv CCDB_HOME /Where/Is/ccdb   (e.g. setenv CCDB_HOME ${CLAS12ROOT}/ccdb )
source ${CCDB_HOME}/environment.csh
#To use clas12-qadb interface 
setenv QADB /Where/Is/clas12-qadb (e.g. setenv QADB ${CLAS12ROOT}/clas12-qadb )
#Preinstalled hipo is a requirement
setenv HIPO /Where/Is/hipo
#If you wish to use, you should set a link to iguana
setenv IGUANA /Where/Is/Iguana
#We now also need root include path to support these pacakges
setenv ROOT_INCLUDE_PATH ${HIPO}/include:${IGUANA}/include:${ROOT_INCLUDE_PATH}
#and if you are using clas12root in other programmes it may help to include it
setenv ROOT_INCLUDE_PATH ${CLAS12ROOT}/Clas12Banks:${CLAS12ROOT}/Clas12Root:${CLAS12ROOT}/hipo4:${ROOT_INCLUDE_PATH}
setenv LD_LIBRARY_PATH $CLAS12ROOT/lib:$LD_LIBRARY_PATH
```

or for bash
```bash
export CLAS12ROOT=$PWD
export PATH="$PATH":"$CLAS12ROOT/bin"
#To use the RCDB interface 
export RCDB_HOME=/Where/Is/rcdb
#To use the CCDB interface 
export CCDB_HOME=/Where/Is/ccdb
source ${CCDB_HOME}/environment.csh
#To use clas12-qadb interface
export QADB=/Where/Is/clas12-qadb
#Preinstalled hipo is a requirement
export HIPO=/Where/Is/hipo
#If you wish to use, you should set a link to iguana
export IGUANA=/Where/Is/Iguana
#We now also need root include path to support these pacakges
export ROOT_INCLUDE_PATH=${HIPO}/include:${IGUANA}/include:${ROOT_INCLUDE_PATH}
#and if you are using clas12root in other programmes it may help to include it
export ROOT_INCLUDE_PATH=${CLAS12ROOT}/Clas12Banks:${CLAS12ROOT}/Clas12Root:${CLAS12ROOT}/hipo4:${ROOT_INCLUDE_PATH}
export LD_LIBRARY_PATH=$CLAS12ROOT/lib:$LD_LIBRARY_PATH
```

## To install

Note on ifarm depending on which version on ROOT and gcc you are using you may have to set the CC and CXX variables or else you will get library linking errors. To do this you will need something like,

```sh
which gcc
   gives /myz/gcc
setenv CC   /xyz/gcc
which g++
  gives /myz/c++
setenv CXX  /myz/c++
```
Or just set the paths to CC and CXX directly.

Remember to build ccdb with CMake if you are using it before installing clas12root. If you alredy have CCDB_HOME set to somewhere else on your system then you will not need to do this. For example,

```bash
cmake -S ccdb -B ccdb_build --install-prefix $CCDB_HOME  # where CCDB_HOME is your preferred installation location for CCDB
cmake --build ccdb_build
cmake --install ccdb_build
```

New 

cmake was modernised. You will now need to make sure $CLAS12ROOT/lib is in your LD_LIBRARY_PATH. Report any issues to the forum.

Then build clas12root:

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
	
	//Get a c12 object for configuring
	auto config_c12=chain.GetC12Reader();
	config_c12->addExactPid(11,1); //if want some filters

	//get c12 obect for running
	auto& c12=chain.C12ref();
	while (chain.Next()){
     
 	      auto electrons=c12->getByID(11);
	      ....
	}

run with

	clas12root RunRoot/Ex1_CLAS12ReaderChain.C+


Also you can check trigger bits directly

     	 c12.checkTriggerBit(24);
	 


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



  You can set the number of events to process :

    ParticleHist [0] hists.SetEntries(1E6);



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

Or for banks without links create one yourself, this is useful for trajectories which require specific detector elements.

!!!! WARNING the label e.g. TRAJFTOFFTOF1A must only contain alphanumeric characters !!!!!

     treemaker.CreateBankLink("TRAJFTOFFTOF1A","p->traj(FTOF,FTOF1A)->");
     treemaker.Branch("TRAJFTOFFTOF1A.X/F");
     treemaker.Branch("TRAJFTOFFTOF1A.Y/F");
     treemaker.Branch("TRAJFTOFFTOF1A.Z/F");

You can also add standard clas12root filtering

     treemaker.AddExactPid(11,1); //filter events with exactly 1 e-
     treemaker.AddAtLeastPid(211,1);//and at least 1 pi+
     treemaker.AddZeroOfRestPid(); //and zero of any other particle type (default is any)


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

## Ex 5  Writing out specific events to a hipo file

The clas12writer class allows you to write out specific events to a new hipo file. The idea behind this is to avoid repeating the same event selection everytime you want to access information about a specific set of events. The writer is assigned a clas12reader from which it gets the event information, and is initialised with the desired location for the outputted hipo file. You can also choose not to write out certain banks to speed the process up.

You can insepct the code [$CLAS12ROOT/RunRoot/Ex5_CLAS12WriterChain.C](https://github.com/jeffersonlab/clas12root/blob/master/RunRoot/Ex5_CLAS12WriterChain.C) for more guidance on how to use it.

To run:

       clas12root $CLAS12ROOT/RunRoot/Ex5_CLAS12WriterChain.C


### Writing using HipoChainWriter

You can also write chains of files using HipoChainWriter. Usage is basically the same as HipoChain with the addition of supply an output, wither file (for single file) or directory (to write 1 output file for each input file)

      clas12root::HipoChainWriter chain("outdir/"); or clas12root::HipoChainWriter chain("outfile.hipo");
      chain.Add("indir/*.hipo");
      //add options like Ex5
      chain.GetWriter().writeSpecialBanks(true);
      chain.GetWriter().skipBank("REC::Scintillator");
   
      ...
      while(chain.Next()){

	// some conditions on event
      	if(...) chain.WriteEvent();
      }
 	 


### Jupyter

Go to directory containing notebooks e.g. $CLAS12ROOT/RunRoot/jupy

Start a ROOT note book :

      	root --notebook

Click on the notebook CLAS12Writer3Pi.ipynb and follow the tutorial


## Clas12Databases

Currently there are 3 database pacakges integrated with clas12root, RCDB, CCDB and QADB, where the latter provides event quality information based on clas12monitoring.
To use any database you must set the corresponding environment variables

setenv RCDB_HOME /where/is/rcdb
setenv CCDB_HOME /where/is/ccdb
setenv QADB /where/is/clas12-qadb

ands pecify the location of the database before you create your clas12root object

        clas12databases::SetCCDBLocalConnection("/where/to/ccdb.sqlite");
  	clas12databases::SetRCDBRootConnection("/where/to/rcdb.root");

These local files can be created using the $CLAS12ROOT/RunRoot/PrepareDatabases.C macro. You are best copying this locally and editing for any datafiles you want to add to the RCDB file list.

It is also possible to make remote connections for RCDB and CCDB, but this is not a good idea so please do not, other than in the PrepareDatabases.C script.

  	clas12databases::SetCCDBRemoteConnection(); //not recommended
  	clas12databases::SetRCDBRemoteConnection(); //not recommended

It is generally recommended to run clas12root via HipoChain for processing of more than 1 file. In case you do not use this you can still connect the databases to your clas12reader

      clas12reader c12("/this/is/my/file.hipo",{0});
      clas12databases db;
      c12.connectDataBases(&db);

When using HipoChain the databases are connected automatically as long as you have the SetCCDB etc, lines in your code.

      auto c12=chain.GetC12Reader();
      c12->db()->ccdb().....

Example of using clas12databases are given in Ex10_clas12Databases.C  Ex10_clas12DatabasesChain.C

### Reading from the Run Conditions DataBase

  Supposing you have successfully setup clas12databases you can access the RCDB table via, 

      auto& rcdbData= c12->rcdb()->current();//using HipoChain
      OR auto& rcdbData= c12.rcdb()->current();//using standalone clas12reader object
      auto energy = rcdbData.beam_energy;



An interface to the run conditions database is implemented by the class rcdb_reader. It will open a connection to rcdb@clasdb.jlab.org/rcdb and allow you to retrieve condition values for a given run. This class interfaces to the Run Conditions DataBase c++ code, more information on it can be found at https://github.com/JeffersonLab/rcdb/wiki/Cpp and the database itself can be viewed at https://clasweb.jlab.org/rcdb/. A full list of the relevent variables can be found at https://clasweb.jlab.org/rcdb/conditions/

### Reading from the Calibration Conditions DataBase

  Supposing you have successfully setup clas12databases you can access a CCDB table via, for example,

     auto&  ccdbElSF=c12->ccdb()->requestTableDoubles("/calibration/eb/electron_sf");//using HipoChain
    OR  auto&  ccdbElSF=c12.ccdb()->requestTableDoubles("/calibration/eb/electron_sf");//using standalone clas12reader object

Where ccdbElSF is a std::vector<std::vector<double>> and so you can access the elements like ccdbElSF[0][2];... When using HipoChain these values will update when you change file as the auto& => a reference to the table values rather than a fixed value. 


### Skimming Based on Data Quality Assurance

clas12root can use the Quality Assurance database .json files found at https://github.com/c-dilks/clas12-qadb/tree/master to reject events that have been identified as failing to meet certain requirements. This is implemented in an analysis using the clas12reader with the functions

      c12.db().applyQA(GETPASSSTRINGHERE);//GETPASSSTRINGHERE="latest", "pass1, "pass2",...
      c12.db().qadb_addQARequirement("MarginalOutlier");
      c12.db().qadb_addQARequirement("TotalOutlier");
      c12.db().qadb_addQARequirement("TerminalOutlier");
      c12.db().qadb_addQARequirement("MarginalOutlier");
      c12.db().qadb_addQARequirement("SectorLoss");
      c12.db().qadb_addQARequirement("LowLiveTime");


Or in case you use HipoChain (also for when running PROOF/HipoSelector)

      auto config_c12=chain.GetC12Reader();
      config_c12->applyQA(GETPASSSTRINGHERE);//GETPASSSTRINGHERE="latest", "pass1, "pass2",...
      config_c12->db()->qadb_addQARequirement("MarginalOutlier");
      config_c12->db()->qadb_addQARequirement("TotalOutlier");
      config_c12->db()->qadb_addQARequirement("TerminalOutlier");
      config_c12->db()->qadb_addQARequirement("MarginalOutlier");
      config_c12->db()->qadb_addQARequirement("SectorLoss");
      config_c12->db()->qadb_addQARequirement("LowLiveTime");
  
addQARequirement("Requirement") allows to reject events that fail to meet the specified requirement. These can be:

    TotalOutlier: outlier N/F, but not terminal, marginal, or sector loss
    TerminalOutlier: outlier N/F of first or last file of run
    MarginalOutlier: marginal outlier N/F, within one stddev of cut line
    SectorLoss: N/F diminished within a sector for several consecutive files
    LowLiveTime: live time < 0.9
    Misc: miscellaneous defect

The clas12-qadb software also returns the accumulated charge for events that have passed the quality assurance requirements. This is accessed with: 

    c12.db()->qadb()->getAccCharge();

Or if using a HipoChain

     chain.TotalBeamCharge()

More information on the Quality Assurance process can be found in the RGA analysis note.

### Fast Accumulated Charge Lookup

The qadb_reader used by the clas12reader to read the Quality Assurance Database can also perform a fast lookup of the accumulated charge of a given list of runs. An example is given in Example 9, Quality Assurance quick lookup script here https://github.com/dglazier/clas12root/blob/qadb_update/RunRoot/Ex9_QualityAssurance_quickLookup.C.

### Using databases with HipoSelector

In your run script you must just set the paths to the data as normal and configure the QADB conditions as above.

Inside your Selector class for CCDB tables :

1) Include CCDBTable datamember for any table you will use, in YourSelector.h class defintion

      clas12::CCDBTable _sampleFrac;

2) In SlaveBegin() request the table,

     //get links to items in CCDB table
      _sampleFrac = requestCCDBTable("/calibration/eb/electron_sf");

3) In ProcessEvent() use the table entries

     Double_t aTableEntry = (&_sampleFrac)[0][0];


Inside your Selector class for RCDB tables :

1)  In Notify() access the rcdb table values e.g.

     _beam.SetE( _c12->rcdb()->current().beam_energy/1000);


See Ex3b_TestSelector.C and testSelector.C and .h for implementation example. To use the databases you need to comment in the lines

    clas12databases::SetCCDBLocalConnection("ccdb.sqlite");
    clas12databases::SetRCDBRootConnection("rcdb.root");

Having run PrepareDatabases.C with the  HipoChain set for the files you wish to process.

## Example GrabEvent

To create a clas12reader and go straight o a particular event in the file,

         clas12reader c12("/hdd/jlab/clas12data/skim3_005644.hipo",{0});
	 auto good= c12.grabEvent(12345);
    	 if(good){
	    //do stuff with clas12reader as in normal event loop
	 }


Note the event number is just its position in the file, not the DST RUN::Config::Event.

## Ex 11 Iguana interface

To run iguana routines in clas12root you should first set the environment to point to an
installed version of iguana, by setting the IGUANA variable to the INSTALLATION directory.

There are 2 methods of using Iguana. For speed and simplicity both use just the iguana
action function and will not operate on the underlying banks structures.
In the first method, which can use any iguan algorithm, you just directly create the iguana
algorithm and run it yourself. See Ex11_Iguana_example_01_bank_rows.C

The second method is though a higher level interface which simplifies the usage, but is not
garuanteed to be able to use all algorithms. In these cases you may revert to the first method
to apply any additional algorithms. The interface for this is in the $CLAS12ROOT/iguana/ directory
and an example is available at Ex11_Iguana_MomentumCorrection.C.

These example use a HipoChain and you will need to set file paths yourself etc.
Using clas12root:region_particles means that information for
alogithms is readily available and so we can just pass these particle objects
into the action functions.

Highlighting the iguana parts for method 2 :

      //clas12root-iguana interface
      clas12root::Iguana ig;

      //choose some algorithms to apply
      ig.GetTransformers().Use("clas12::MomentumCorrection");
      ig.GetFilters().Use("clas12::ZVertexFilter");
      ig.GetCreators().Use("physics::InclusiveKinematics");

      ig.SetOptionAll("log", "debug");
      ig.Start();


      ...
      // get particles by type
      // note we applied a filter to ensure size of all ==1
      auto electron=c12->getByID(11)[0];
      auto pip=c12->getByID(211)[0];
      auto pim=c12->getByID(-211)[0];

      ///Now do some iguana analysis!!

      //filter on z-vertices of the particles
      //note I can pass a vector of clas12root particles
      if( !(ig.GetFilters().doZVertexFilter({electron,pip,pim})) ) {
        continue;
      }


      //correct momentum and get 4-vectors
      //I pass a vector of clas12root particles and LorentzVectors
      ig.GetTransformers().doMomentumCorrections({electron,pip,pim},{&p4el,&p4pip,&p4pim});

      //calculate inclusive kinematics
      //use original bank info
      auto kine = ig.GetCreators().doInclusiveKinematics(electron);
      //use momentum corrected momentum
      auto corrkine = ig.GetCreators().doInclusiveKinematics(p4el);

      ...



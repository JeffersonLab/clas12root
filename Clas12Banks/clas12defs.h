#ifndef CLAS12DEFS_H
#define CLAS12DEFS_H


namespace clas12{
  //regions
  static const short FT = 1000;
  static const short FD = 2000;
  static const short CD = 3000;

  
  //detectors
  static const short FTOF = 12;
  static const short CTOF = 4;
  static const short CND  = 3;
  static const short CVT   = 5;
  static const short DC   = 6;
  static const short EC   = 7;
  static const short ECAL   = 7;
  static const short FTCAL   = 10;
  static const short FTTRK   = 13;
  static const short FTHODO   = 11;
  static const short HTCC   = 15;
  static const short LTCC   = 16;
  static const short BMT   = 1;
  static const short FMT   = 8;
  static const short RF   = 17;
  static const short RICH   = 18;
  static const short RTPC   = 19;
  static const short HEL   = 20;
  static const short BAND   = 21;


  //layers
  static const short FTOF1A = 1;
  static const short FTOF1B = 2;
  static const short FTOF2 = 3;
  //CDET scint layers same as detectors
  //static const short CND  = 3;
  //static const short CTOF = 4;
  static const short CND1  = 1;
  static const short CND2  = 2;
  static const short CND3  = 3;
 
  static const short PCAL   = 1;
  static const short ECIN   = 4;
  static const short ECOUT  = 7;

  //additional TRAJECTORY layers
   static const short DC1  = 6;
  static const short DC2  = 12;
  static const short DC3  = 18;
  static const short DC4  = 24;
  static const short DC5  = 30;
  static const short DC6  = 36;

  static const short CVT1  = 1;
  static const short CVT2  = 2;
  static const short CVT3  = 3;
  static const short CVT4  = 4;
  static const short CVT5  = 5;
  static const short CVT6  = 6;
  static const short CVT7  = 7;
  static const short CVT8  = 8;
  static const short CVT9  = 9;
  static const short CVT10  = 10;
  static const short CVT11  = 11;
  static const short CVT12  = 12;
   // using indices =vector<short>;
  
  /* std::map<TString,vector<TString>> */
  /*   detectorAndLayer={{"FTOF",{"FTOF1A","FTOF1B","FTOF2"}}, */
  /* 		      {"DC",{"DC1","DC2","DC3","DC4","DC5","DC6"}}, */
  /* 		      {"CVT",{"CVT1","CVT2","CVT3","CVT4","CVT5","CVT6","CVT7","CVT8","CVT9","CVT10","CVT11","CVT12"}}, */
  /* 		      {"CND",{"CND1","CND2","CND3"}}, */
  /* 		      {"EC",{"ECIN","ECOUT","PCAL"}}, */
  /* }; */
}

#endif

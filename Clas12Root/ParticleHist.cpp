#include "ParticleHist.h"
#include "HipoRootAction.h"
#include "clas12reader.h"
#include <TObjArray.h>

namespace clas12root {

  ParticleHist::ParticleHist(TString filename): HipoHist(filename){
    _tempActionName="ParticleHistAction";

    //Calorimters
    _mapOfParts["ECIN"]="p->cal(ECIN)->";
    _mapOfParts["ECOUT"]="p->cal(ECOUT)->";
    _mapOfParts["PCAL"]="p->cal(PCAL)->";

    //Scintillators
    _mapOfParts["FTOF1A"]="p->sci(FTOF1A)->";
    _mapOfParts["FTOF1B"]="p->sci(FTOF1B)->";
    _mapOfParts["FTOF2"]="p->sci(FTOF2)->";
    _mapOfParts["CTOF"]="p->sci(CTOF)->";
    _mapOfParts["CND1"]="p->sci(CND1)->";
    _mapOfParts["CND2"]="p->sci(CND2)->";
    _mapOfParts["CND3"]="p->sci(CND3)->";

    //FT
    _mapOfParts["FTCAL"]="p->ft(FTCAL)->";
    _mapOfParts["FTHODO"]="p->ft(FTHODO)->";

    //Cherenkov
    _mapOfParts["HTCC"]="p->che(HTCC)->";
    _mapOfParts["LTCC"]="p->che(LTCC)->";

    //Trackers
    _mapOfParts["DC"]="p->trk(DC)->";
    _mapOfParts["CVT"]="p->trk(CVT)->";

    //Covariance Matrix
    _mapOfParts["CovMat"]="p->covmat()->";
   
    //Particles
    _mapOfParts["PBANK"]="p->par()->";

    //Particle Region
    _mapOfParts["P"]="p->";

    //RUN
     _mapOfParts["RUN"]="c12.runconfig()->";

     //EVENT
    _mapOfParts["EVNT"]="c12.event()->";
    _mapOfParts["EVNT4"]="c12.event()->"; //kept for backward compatability hipo3->hipo4
    _mapOfParts["EVNTFT"]="c12.ftbevent()->";

     //C12
    _mapOfParts["C12"]="c12.";
    _mapOfParts["C12.NPid"]="c12.getNPid";

    //Trajectories
    //_mapOfParts["TRAJ[FTOF,FTOF1A]"]="p->traj(FTOF,FTOF1A)->";
  }
  ParticleHist::~ParticleHist(){
    }
  void ParticleHist::Loop(){
    while(IsMoreFiles())
      gHipoRootAction->Action(this);
    
  }
 
}

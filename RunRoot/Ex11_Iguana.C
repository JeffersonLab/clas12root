/**
 * Example demonstrating how to read a HIPO file with clas12root,
 * and use Iguana algorithms to process its banks
 *
 * USAGE:
 * ```
 * clas12root RunRoot/Ex11_Iguana.C+ --in=hipo_file_1.hipo --in=hipo_file_2.hipo
 * ```
 */

// ROOT
#include <TApplication.h>
#include <TRegexp.h>

// clas12root
#include <HipoChain.h>

// include iguana algorithms
#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>
#include <iguana/algorithms/clas12/SectorFinder/Algorithm.h>
#include <iguana/algorithms/clas12/MomentumCorrection/Algorithm.h>
#include <iguana/algorithms/physics/InclusiveKinematics/Algorithm.h>

void Ex11_Iguana() {

  // parse arguments, which should be HIPO filename(s) prefixed with `--in=`; add them to a `HipoChain`
  clas12root::HipoChain chain;
  for(int i=2; i<gApplication->Argc(); i++) {
    TString inputFile = gApplication->Argv(2);
    inputFile(TRegexp("^--in=")) = "";
    std::cout << "reading file " << inputFile << std::endl;
    chain.Add(inputFile);
  }
  if(chain.GetNFiles() == 0) {
    std::cerr << " *** please provide HIPO file name(s)" << std::endl;
    exit(1);
  }

  // read tag-0 events only
  chain.SetReaderTags({0});

  // create iguana algorithms
  iguana::clas12::ZVertexFilter        algo_z_vertex_filter;      // filter the z-vertex (a filter algorithm)
  iguana::clas12::SectorFinder         algo_sector_finder;        // get the sector for each particle (a creator algorithm)
  iguana::clas12::MomentumCorrection   algo_momentum_correction;  // momentum corrections (a transformer algorithm)
  iguana::physics::InclusiveKinematics algo_inclusive_kinematics; // calculate inclusive kinematics (a creator algorithm)

  /*
   * configure iguana algorithms here (see iguana documentation)
   */
  // algo_sector_finder.SetOption("log", "trace"); // for example, more detailed logging

  // start iguana algorithms, after configuring them
  algo_z_vertex_filter.Start();
  algo_sector_finder.Start();
  algo_momentum_correction.Start();
  algo_inclusive_kinematics.Start();

  // create bank objects, which creator-type algorithms will populate
  // - by creating them here, this macro is the owner of these bank objects, but
  //   iguana algorithms' Run functions will handle them by reference
  auto created_bank_sector = algo_sector_finder.GetCreatedBank();
  auto created_bank_inclusive_kinematics = algo_inclusive_kinematics.GetCreatedBank();

  // define a lambda function that processes HIPO banks, in particular, with iguana
  // - this function will be executed by `clas12reader` as soon as each event's `hipo::bank`
  //   objects are filled, i.e., before `clas12reader` further processes the banks, within
  //   functions such as:
  //   - `clas12reader::next()`
  //   - `HipoChain::Next()`
  // - if, for example, a momentum corrections algorithm is used, which corrects momenta
  //   of particles in the `REC::Particle` bank, then clas12root objects which are based
  //   on `REC::Particle`, such as `region_particle`, will have the corrected momenta
  // - this example macro owns algorithm and created bank objects, therefore the lambda
  //   function must capture them by reference
  // - the parameter `cr` is a pointer, which is a `clas12reader` instance, whether you are
  //   using `clas12reader` directly (see `RunRoot/Ex1_CLAS12Reader.C`) or
  //   using `HipoChain` (see `RunRoot/Ex1_CLAS12ReaderChain.C`)
  //   - when you call functions like `clas12reader::next()` or `HipoChain::Next()`, the
  //     corresponding `clas12reader` instance will be passed into this lambda function
  //   - use `cr` to access DST banks from the `clas12reader`, using `cr->getBankName()` methods,
  //     where "BankName" is the name of the bank without the colons (`::`); for example, use
  //     `cr->getRECFTParticle` to access the `RECFT::Particle` bank
  auto iguana_action = [
    // captured algorithms; use the ampersand (`&`) to capture them by reference
    &algo_z_vertex_filter,
    &algo_sector_finder,
    &algo_momentum_correction,
    &algo_inclusive_kinematics,
    // captured banks, again by reference
    &created_bank_sector,
    &created_bank_inclusive_kinematics
  ](clas12::clas12reader* cr)
  {
    // call Iguana `Run` functions
    // - the choice of ordering is yours; for example, do you correct momenta before or after
    //   applying a filter which depends on momenta?
    // - these `Run` functions take `hipo::bank` objects as parameters (technically, lvalue references)
    //   - some bank objects may be updated, depending on the algorithm type
    //     - filters tend to filter certain bank's rows
    //     - transformers tend to mutate certain values
    //     - creators create new banks, i.e., the `created_bank_*` objects will be populated with data
    // - DST banks are read from the `clas12reader` instance, `cr`
    // - created banks must exist so they can be filled; this is why we created them beforehand
    // - `Run` functions return boolean, which we can use to skip an event at any time

    // z-vertex filter returns false if no electrons pass the filter
    if( ! algo_z_vertex_filter.Run(
          cr->getRECParticle(),
          cr->getRUNconfig()
          )
      )
      return false;

    // momentum corrections require sector information, so call the sector finder first
    algo_sector_finder.Run(
        cr->getRECParticle(),
        cr->getRECTrack(),
        cr->getRECCalorimeter(),
        cr->getRECScintillator(),
        created_bank_sector
        );
    algo_momentum_correction.Run(
        cr->getRECParticle(),
        created_bank_sector,
        cr->getRUNconfig()
        );

    // finally, calculate inclusive kinematics; just return its return value, which is
    // true only if the kinematics were calculated (e.g., if a scattered electron was found)
    return algo_inclusive_kinematics.Run(
        cr->getRECParticle(),
        cr->getRUNconfig(),
        created_bank_inclusive_kinematics
        );
  };

  // attach the iguana-running lambda function to the `clas12reader` event reader
  // - `chain.Next()` will call `iguana_action` internally, just after the HIPO banks have been
  //   read, but before any bank-dependent objects are created, such as `region_particle`
  chain.GetC12Reader()->SetReadAction(iguana_action);

  // now get reference to (unique)ptr for accessing data in loop
  // this will point to the correct place when file changes
  auto const& c12 = chain.C12ref();

  // loop over events
  for(int numEvents=0; chain.Next() && numEvents++ < 3;) // loop over just a few events
  // while(chain.Next()) // loop over all events
  {

    // print DST banks
    c12->getRECParticle().show();

    // print iguana algorithms' created banks
    created_bank_sector.show();
    created_bank_inclusive_kinematics.show();

  }

}

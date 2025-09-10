/**
 * Example demonstrating how to read a HIPO file with clas12root,
 * and use Iguana algorithms to process its banks
 *
 * USAGE:
 * ```
 * clas12root RunRoot/Ex11_Iguana.C+ hipo_file.hipo
 * ```
 */

#include <TApplication.h>
#include <clas12reader.h>

// include iguana algorithms
#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>
#include <iguana/algorithms/clas12/SectorFinder/Algorithm.h>
#include <iguana/algorithms/clas12/MomentumCorrection/Algorithm.h>
#include <iguana/algorithms/physics/InclusiveKinematics/Algorithm.h>

void Ex11_Iguana() {

  // parse argument, which should be a HIPO file
  if(gApplication->Argc() != 3) {
    std::cerr << " *** please provide a file name" << std::endl;
    exit(1);
  }
  std::string inputFile = gApplication->Argv(2);
  std::cout << "reading file " << inputFile << std::endl;

  // create iguana algorithms
  iguana::clas12::ZVertexFilter        algo_z_vertex_filter;      // filter the z-vertex (a filter algorithm)
  iguana::clas12::SectorFinder         algo_sector_finder;        // get the sector for each particle (a creator algorithm)
  iguana::clas12::MomentumCorrection   algo_momentum_correction;  // momentum corrections (a transformer algorithm)
  iguana::physics::InclusiveKinematics algo_inclusive_kinematics; // calculate inclusive kinematics (a creator algorithm)

  /*
   * configure iguana algorithms here (see iguana documentation)
   */

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
  //   - when you call functions like `clas12reader::next()`, the corresponding `clas12reader`
  //     instance will be passed into this lambda function
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
    algo_z_vertex_filter.Run(
        cr->getRECParticle(),
        cr->getRUNconfig()
        );
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
    algo_inclusive_kinematics.Run(
        cr->getRECParticle(),
        cr->getRUNconfig(),
        created_bank_inclusive_kinematics
        );
  };

  // create the `clas12reader` instance
  clas12::clas12reader c12(inputFile, {0});

  // attach the iguana-running lambda function to the event reader
  // - `c12.next()` will call `iguana_action(c12)` internally, just after the HIPO banks have been
  //   read, but before any bank-dependent objects are created, such as `region_particle`
  // - if you are using `HipoChain`, use its `GetC12Reader()` method to access its `clas12reader` instance
  c12.SetReadAction(iguana_action);

  // loop over events
  while(c12.next()) {

    // print the created inclusive kinematics bank
    created_bank_inclusive_kinematics.show();

  }

}

/**
 * Example demonstrating how to read a HIPO file with clas12root,
 * and use Iguana algorithms to process its banks
 *
 * NOTE: this requires Iguana version 1.0.0 or newer
 *
 * USAGE:
 * ```
 * clas12root RunRoot/Ex11_Iguana.C+ --in=hipo_file_1.hipo --in=hipo_file_2.hipo
 * ```
 */

// ROOT
#include <TApplication.h>
#include <TRegexp.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TProfile.h>

// clas12root
#include <HipoChain.h>

// include iguana algorithms
#include <iguana/algorithms/clas12/ZVertexFilter/Algorithm.h>
#include <iguana/algorithms/clas12/SectorFinder/Algorithm.h>
#include <iguana/algorithms/clas12/rga/MomentumCorrection/Algorithm.h>
#include <iguana/algorithms/clas12/rga/FiducialFilterPass2/Algorithm.h>
#include <iguana/algorithms/physics/InclusiveKinematics/Algorithm.h>

void Ex11_Iguana() {

  // parse arguments, which should be HIPO filename(s) prefixed with `--in=`; add them to a `HipoChain`
  clas12root::HipoChain chain;
  for(int i=2; i<gApplication->Argc(); i++) {
    TString inputFile = gApplication->Argv(2);
    inputFile(TRegexp("^--in=")) = "";
    std::cout << "reading file " << inputFile << std::endl;
    chain.Add(inputFile);
    chain.SetReaderTags({0}); // read tag-0 events only
  }
  if(chain.GetNFiles() == 0) {
    std::cerr << " *** please provide HIPO file name(s)" << std::endl;
    exit(1);
  }

  //////////////////////////////////////////////////////////////////////////////////

  // histograms
  int const n_bins = 100;
  auto* Q2_vs_x = new TH2D("Q2_vs_x", "Q^{2} vs. x;x;Q^{2} [GeV^{2}]",       n_bins, 0,   1, n_bins, 0, 12);
  auto* Q2_vs_W = new TH2D("Q2_vs_W", "Q^{2} vs. W;W [GeV];Q^{2} [GeV^{2}]", n_bins, 0,   5, n_bins, 0, 12);
  auto* y_dist  = new TH1D("y_dist",  "y distribution;y",                    n_bins, 0,   1);
  auto* vz_dist = new TH1D("vz_dist", "electron v_{z};v_{z} [cm]",           n_bins, -30, 30);
  auto* deltaP_vs_P = new TH2D("deltaP_vs_P", "electron momentum correction;p_{meas} [GeV];p_{corr}-p_{meas} [GeV]", 10, 0, 12, n_bins, -0.2, 0.2);
  // histogram styles
  for(auto hist : {y_dist, vz_dist}) {
    hist->SetLineColor(kAzure);
    hist->SetFillColor(kAzure);
  }

  //////////////////////////////////////////////////////////////////////////////////

  // create iguana algorithms
  iguana::clas12::ZVertexFilter            algo_vz_filter;   // filter the z-vertex (a filter algorithm)
  iguana::clas12::SectorFinder             algo_sec_finder;  // get the sector for each particle (a creator algorithm)
  iguana::clas12::rga::FiducialFilterPass2 algo_fidu_filter; // fiducial cuts (a filter algorithm)
  iguana::clas12::rga::MomentumCorrection  algo_mom_corr;    // momentum corrections (a transformer algorithm)
  iguana::physics::InclusiveKinematics     algo_inc_kin;     // calculate inclusive kinematics (a creator algorithm)

  /*
   * configure iguana algorithms here (see iguana documentation)
   */
  // algo_sec_finder.SetOption("log", "trace"); // for example, more detailed logging

  // start iguana algorithms, after configuring them
  algo_vz_filter.Start();
  algo_sec_finder.Start();
  algo_fidu_filter.Start();
  algo_mom_corr.Start();
  algo_inc_kin.Start();

  // create bank objects, which creator-type algorithms will populate
  // - by creating them here, this macro is the owner of these bank objects, but
  //   iguana algorithms' Run functions will handle them by reference
  auto iguana_bank_sec     = algo_sec_finder.GetCreatedBank();
  auto iguana_bank_inc_kin = algo_inc_kin.GetCreatedBank();

  // let's also create a "cache" of particle momentum magnitudes, since we want to check the effects
  // of momentum corrections
  std::vector<double> p_measured;

  //////////////////////////////////////////////////////////////////////////////////

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
    &algo_vz_filter,
    &algo_sec_finder,
    &algo_fidu_filter,
    &algo_mom_corr,
    &algo_inc_kin,
    // captured banks, again by reference
    &iguana_bank_sec,
    &iguana_bank_inc_kin,
    // also capture our `p_measured` cache, since we want to use it outside the lambda
    &p_measured
  ](clas12::clas12reader* cr)
  {
    // before anything, let's "cache" our momentum magnitudes: loop over `REC::Particle`
    // and store |p| for each particle
    p_measured.clear();
    for(auto const& row : cr->getRECParticle().getRowList())
      p_measured.push_back(std::hypot(
            cr->getRECParticle().getFloat("px", row),
            cr->getRECParticle().getFloat("py", row),
            cr->getRECParticle().getFloat("pz", row)));

    // call Iguana `Run` functions
    // - the choice of ordering is yours; for example, do you correct momenta before or after
    //   applying a filter which depends on momenta?
    // - these `Run` functions take `hipo::bank` objects as parameters (technically, lvalue references)
    //   - some bank objects may be updated, depending on the algorithm type
    //     - filters tend to filter certain bank's rows
    //     - transformers tend to mutate certain values
    //     - creators create new banks, i.e., the `iguana_bank_*` objects will be populated with data
    // - DST banks are read from the `clas12reader` instance, `cr`
    // - created banks must exist so they can be filled; this is why we created them beforehand
    // - `Run` functions return boolean, which we can use to skip an event at any time, i.e., use
    //   the pattern `if(!algo.Run(...)) return false;`

    // start with the z-vertex filter; it returns false if no electrons pass the filter
    if(!algo_vz_filter.Run(
          cr->getRECParticle(), // REC::Particle
          cr->getRUNconfig()    // RUN::config
          )) return false;

    // next, let's apply the fiducial cuts; we'll skip FT cuts for now, but take a look
    // at the algorithm's documentation for other `Run` functions that use FT data
    if(!algo_fidu_filter.Run(
          cr->getRECParticle(),    // REC::Particle
          cr->getRUNconfig(),      // RUN::config
          cr->getRECCalorimeter(), // REC::Calorimeter
          cr->getRECTraj()         // REC::Traj
          )) return false;

    // momentum corrections require sector information, so call the sector finder first;
    // note that the momentum correction algorithm will alter the `px,py,pz` of certain
    // particles, which is why we filled `p_measured` first above
    if(!algo_sec_finder.Run(
          cr->getRECParticle(),     // REC::Particle
          cr->getRECTrack(),        // REC::Track
          cr->getRECCalorimeter(),  // REC::Calorimeter
          cr->getRECScintillator(), // REC::Scintillator
          iguana_bank_sec           // result of iguana::clas12::SectorFinder  ----.
          )) return false;          //                                             |
                                    //                                             |
    if(!algo_mom_corr.Run(          //                                             |
          cr->getRECParticle(),     // REC::Particle                               |
          iguana_bank_sec,          // result of iguana::clas12::SectorFinder  <---'
          cr->getRUNconfig()        // RUN::config
          )) return false;

    // finally, calculate inclusive kinematics
    if(!algo_inc_kin.Run(
        cr->getRECParticle(), // REC::Particle
        cr->getRUNconfig(),   // RUN::config
        iguana_bank_inc_kin   // result of iguana::physics::InclusiveKinematics
        )) return false;

    // all algorithms are done, return true to keep the event
    // (otherwise return false if you have some reason to not keep the event)
    return true;
  };

  // attach the iguana-running lambda function to the `clas12reader` event reader
  // - `chain.Next()` will call `iguana_action` internally, just after the HIPO banks have been
  //   read, but before any bank-dependent objects are created, such as `region_particle`
  chain.GetC12Reader()->SetReadAction(iguana_action);

  //////////////////////////////////////////////////////////////////////////////////

  // now get a pointer to the internal `clas12reader`
  // this will point to the correct place when file changes
  auto const& c12 = chain.C12ref();

  // loop over events
  for(int numEvents=0; chain.Next(); numEvents++) // loop over all events
  {

    // let's be verbose for the first few events, to demonstrate what Iguana did
    if(numEvents < 30) {
      std::cout << "===== EVENT " << c12->runconfig()->getEvent() << "===========\n";

      // print banks
      // -----------
      std::cout << "------- FULL PARTICLE BANK -------\n";
      c12->getRECParticle().show(true); // use `true`, otherwise only filter-allowed rows are printed
      std::cout << "----- FILTERED PARTICLE BANK -----\n";
      c12->getRECParticle().show();
      std::cout << "---------- IGUANA BANKS ----------\n";
      iguana_bank_sec.show();
      iguana_bank_inc_kin.show();
      std::cout << "----------------------------------\n";

      // Accessing bank rows and filtering
      // ---------------------------------
      // if you want to loop over filtered rows, use `hipo::bank::getRowList()`; otherwise,
      // just use the usual `for` loop from `0` up to `bank->getRows()`
      std::cout << "REC::Particle filter-allowed rows:";
      for(const auto& row : c12->getRECParticle().getRowList())
        std::cout << " " << row;
      std::cout << " (out of " << c12->getRECParticle().getRows() << " rows total)\n";

      // Get particles by type
      // ---------------------
      // NOTE: to make sure that only the particles which passed Iguana filters,
      // set the additional boolean argument to `true`, otherwise you will get ALL
      // the particles; this may done in any `region_particle` list accessor, such as:
      // - `clas12reader::getByID`
      // - `clas12reader::getByCharge`
      // - `clas12reader::getByRegion`
      // - `clas12reader::getDetParticles`
      std::cout << "electrons allowed by Iguana:";
      for(auto const* electron : c12->getByID(11, true)) // loops over electrons which passed the filter(s)
        std::cout << " " << electron->getIndex();
      std::cout << "\n";

      // alternatively, you could use `region_particle::isAllowed()` to filter as needed
      std::cout << "electrons filtered out by Iguana:";
      for(auto const* electron : c12->getByID(11)) { // loops over ALL electrons
        if( ! electron->isAllowed()) { // selects electrons which were filtered OUT
          std::cout << " " << electron->getIndex();
        }
      }
      std::cout << "\n";
    }
    else if(numEvents % 10000 == 0)
      std::cout << "read " << numEvents << " events\n";

    // from here, refer to other examples on how to proceed;
    // in this example, we'll fill the histograms

    // inclusive kinematics; the created bank has only 1 row
    Q2_vs_x->Fill(iguana_bank_inc_kin.getDouble("x", 0), iguana_bank_inc_kin.getDouble("Q2", 0));
    Q2_vs_W->Fill(iguana_bank_inc_kin.getDouble("W", 0), iguana_bank_inc_kin.getDouble("Q2", 0));
    y_dist->Fill(iguana_bank_inc_kin.getDouble("y", 0));

    // scattered electron pindex
    auto pindex_ele = iguana_bank_inc_kin.getShort("pindex", 0);

    // loop over electrons, and choose the scattered electron
    // note: it would be faster to use `c12->getRECParticle()` and choose row `pindex_ele`, but here
    // we want to demonstrate how to use `c12->getByID` to access the same information
    for(auto const* electron : c12->getByID(11, true)) { // loops over electrons which passed the filter(s)
      if(electron->getIndex() == pindex_ele) { // choose only the scattered electron
        // electron vertex
        auto vz = electron->par()->getVz();
        vz_dist->Fill(vz);
        // electron momentum, which was corrected by Iguana's momentum corrections
        auto p_corrected = electron->getP();
        deltaP_vs_P->Fill(p_corrected, p_corrected - p_measured.at(pindex_ele));
        // note: alternatively, we could have just gotten these values directly
        // from the `REC::Particle` bank; let's cross check the value to prove it:
        auto vz_from_bank = c12->getRECParticle().getFloat("vz", pindex_ele);
        auto p_from_bank  = std::hypot(
            c12->getRECParticle().getFloat("px", pindex_ele),
            c12->getRECParticle().getFloat("py", pindex_ele),
            c12->getRECParticle().getFloat("pz", pindex_ele));
        if(std::abs(vz - vz_from_bank) > 1e-4)
          throw std::runtime_error("vz cross check failed: " + std::to_string(vz) + " vs. " + std::to_string(vz_from_bank));
        if(std::abs(p_corrected - p_from_bank) > 1e-4)
          throw std::runtime_error("|p| cross check failed: " + std::to_string(p_corrected) + " vs. " + std::to_string(p_from_bank));
        // exit the loop over electrons
        break;
      }
    }

  } // end event loop

  //////////////////////////////////////////////////////////////////////////////////

  // stop the iguana algorithms, now that the event loop is done
  algo_vz_filter.Stop();
  algo_sec_finder.Stop();
  algo_fidu_filter.Stop();
  algo_mom_corr.Stop();
  algo_inc_kin.Stop();

  // draw the plots
  int const n_rows = 2;
  int const n_cols = 3;
  auto canv  = new TCanvas("canv", "canv", n_cols * 800, n_rows * 600);
  canv->Divide(n_cols, n_rows);
  for(int pad_num = 1; pad_num <= n_rows * n_cols; pad_num++) {
    auto pad = canv->GetPad(pad_num);
    pad->cd();
    pad->SetGrid(1, 1);
    pad->SetLeftMargin(0.12);
    pad->SetRightMargin(0.12);
    pad->SetBottomMargin(0.12);
    switch(pad_num) {
      case 1:
        pad->SetLogz();
        Q2_vs_x->Draw("colz");
        break;
      case 2:
        pad->SetLogz();
        Q2_vs_W->Draw("colz");
        break;
      case 3:
        y_dist->Draw();
        break;
      case 4:
        pad->SetLogy();
        vz_dist->Draw();
        break;
      case 5:
        pad->SetLogz();
        deltaP_vs_P->Draw("colz");
        auto prof = deltaP_vs_P->ProfileX("_pfx", 1, -1, "s");
        prof->SetLineColor(kBlack);
        prof->SetLineWidth(5);
        prof->Draw("same");
        break;
    }
  }

}

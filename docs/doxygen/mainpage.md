# Clas12root User's Guide

@todo Add more user documentation here! Populating CLAS12 Software documentation in general is an open service work project...

## Commonly Used Classes

- For reading HIPO files:
    - `clas12::clas12reader`: for reading a single HIPO file
    - `clas12root::HipoChain`: for reading multiple HIPO files
- For particles:
    - `clas12::region_particle`: base class region particle
        - `clas12::region_band`: for BAND
        - `clas12::region_cdet`: for Central Detector
        - `clas12::region_fdet`: for Forward Detector
        - `clas12::region_ft`: for Forward Tagger (FT)

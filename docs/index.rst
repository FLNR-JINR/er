.. ExpertRoot documentation master file, created by
.. sphinx-quickstart on Wed Nov 30 20:57:30 2016.
.. You can adapt this file completely to your liking, but it should at least
.. contain the root `toctree` directive.

ExpertRoot Documentation
========================

ExpertRoot (ER) is a `FairRoot <https://github.com/FairRootGroup/FairRoot>`_-based framework dedicated
to the simulation, reconstruction, data acquisition, and analysis of the nuclear physics experiments.
Primarily developed for the needs of experiments on fragment separator 
`ACCULLINA-2 <http://aculina.jinr.ru/a-2.html>`_ at Fleurov Nuclear Research in JINR and experiment
`EXPERT <http://aculina.jinr.ru/expert.html>`_ on fragment separator SUPER-FRS at FAIR in GSI.

**Key features:**
  * User-defined interactions: reactions and decays.
  * It is possible to take into account energy losses of projectiles in the target and the beam detector components such as ToF scintillators and MWPC stations.
  * Simulation and analysis of cocktail and defocusing beam.
  * Accounting for energy losses in the dead layers of the detector during reconstruction of ejectiles energies by experimental data.

**Services:**
  * github - https://github.com/FLNR-JINR/er/
  * development tracker - https://github.com/FLNR-JINR/er/projects/1
  * doxygen - http://er.jinr.ru/doxygen/
  * internal gitlab - http://er.jinr.ru/git

**Library Stack**

Simulation, reconstruction and analysis in the ER is based on
  * `FairSoft <https://github.com/FairRootGroup/FairSoft/tree/dev>`_ - includes widespread Geant4, ROOT, VMC and others utilities.
  * `FairRoot <https://github.com/FairRootGroup/FairRoot>`_ - framework which provides the base classes for IO-management, base classes for event-based simulation and reconstruction.

Experimental data processing
  * `Go4 <https://www.gsi.de/en/work/research/experiment_electronics/data_processing/data_analysis/the_go4_home_page.htm>`_ - framework for on-line and off-line processing of experimental data.
  * `AccDAQ <https://github.com/FLNR-JINR/ACCULINNA_go4_user_library>`_ - library for unpacking and transformation of the raw experimental data in *lmd* file format produced by Go4 to *root* format. Output files are applicable for the analysis in the ExpertRoot. 

*ER* extends FairRoot with experiment-specific tasks and adds several features which enables to use it
for full live cycle of low-energy physics experiment.

.. toctree::
  :maxdepth: 2
  :caption: Content:

  install
  generators
  decays
  beamdet
  si_detector_calibration
  qtelescope
  neurad
  gadast
  detdevelopment
  courses
  faq
  sadms
  howto

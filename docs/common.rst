Общее
=====

Framework for:
	* Detector response Monte-Carlo simulation
	* Event reconstruction algorithm
	* Data analysis algorithm
Simulation for:
	* Detectors construction optimization
	* Efficiency prediction
	* Signal/background ratio prediction
	* Developing analysis algorithms

Структура
---------

.. figure:: _images/sructure.png
       :scale: 100 %
       :align: center


First of all ExpertRoot is a  FairRoot based application.
FairRoot is a number of instruments and approaches for setting up and configuring simulation and reconstructions procedures.

The most of the classes of ExpertRoot are inherited from the FairRoot classes. There are detectors classes for geometry construction and simulations rules, generators classes for creating the stack of primariy tracks and tasks classes for digitization, reconstruction and analysis.

ERGenerator is used for reading tracks from the theory based generator called TEG-DDR.
ERGammaGenerator is used for creating gamma quanta emitted with the given energy in the Center of mass system by the relativistic ion.


Event based workflow
--------------------

.. figure:: _images/ev_based.png
       :scale: 100 %
       :align: center


ExpertRoot realizes event based workflow approach. 

MC event contains all the products to a single interaction of the interesting ion with the target.
The event may include also all kind of backgrounds both correlated and 
non correlated with the  interaction of interest.

Transport simulation gives the deposits of energy in any detector. 

Digitization takes into account readout granularity, inefficiency, noises and dark counts and produces a 
Signal similar to the one from a detector. 

Cluster and hit finding procedures build a hypothesis  of where the particle may have passed.

Track finder and the Particle identification are the reconstruction stages.

Physical analysis serves for plausibility tests and for deriving the  physical information.

Note that all the steps after digitization are the same as for the real data.

Пользовательские распады
------------------------

Классы Setup
------------
#!/bin/bash

# This macro is used to generate the ROOT dictionaries
# To use the ctest launchers one needs some environment variables which
# are only present when running CMake. To have the same environment at
# the time the dictionary is build this script is used which is build
# at the time cmake runs.

# Setup the needed environment
export LD_LIBRARY_PATH=/opt/fs_may16p1_inst/lib/root::/opt/AMDAPPSDK-2.9-1/lib/x86_64/
export DYLD_LIBRARY_PATH=/opt/fs_may16p1_inst/lib/root::/opt/AMDAPPSDK-2.9-1/lib/x86_64/
export ROOTSYS=/opt/fs_may16p1_inst

/opt/fs_may16p1_inst/bin/rootcint -f /home/muzalevsky/check/build/data/G__ERDataDict.cxx -inlineInputHeader -rmf /home/muzalevsky/check/build/lib/libERData.rootmap -rml ERData.so -c   -I/opt/fr_16.06_inst/include -I/opt/fs_may16p1_inst/include -I/opt/fs_may16p1_inst/include/root -I/home/muzalevsky/check/data -I/home/muzalevsky/check/data/NeuRadData -I/home/muzalevsky/check/data/muSiData -I/home/muzalevsky/check/data/NDData -I/home/muzalevsky/check/data/beamtime -I/opt/fs_may16p1_inst/include/root -I/opt/fs_may16p1_inst/include NeuRadData/ERNeuRadPoint.h NeuRadData/ERNeuRadPhotoElectron.h NeuRadData/ERNeuRadPixelSignal.h NeuRadData/ERNeuRadDigi.h NeuRadData/ERNeuRadStep.h NeuRadData/ERNeuRadHit.h NeuRadData/ERNeuRadHitWBT.h muSiData/ERmuSiPoint.h muSiData/ERmuSiHit.h muSiData/ERmuSiTrack.h muSiData/ERmuSiVertex.h GadastData/ERGadastCsIPoint.h GadastData/ERGadastLaBrPoint.h GadastData/ERGadastStep.h GadastData/ERGadastDigi.h ERStack.h ERMCTrack.h ERMCEventHeader.h EREventHeader.h ERLi10EventHeader.h ERLi10MCEventHeader.h NDData/ERNDPoint.h NDData/ERNDHit.h DSRDData/ERDSRDPoint.h DSRDData/ERDSRDHit.h beamtime/RawEvent.h beamtime/AEvent.h beamtime/TGo4EventElement.h beamtime/TLiEvent.h beamtime/cls_RootHit.h beamtime/cls_RootEvent.h beamtime/WCalHit.h beamtime/WCalEvent.h ERDataLinkDef.h

## Check of the track finding quality

Macros in this folder are intended for checking of a quality of the coordinate reconstruction procedure (track finding). 
A reconstructed data is compared with simulation data with the usage of the sensitive target in BeamDet detector simulation.
Point data on the entering into the target is used, so the thickness of target in the BeamDet must be thin to reduce a 	consistent error. 

Parameters to reproduce results from macros: 

- hardcoded in ERBeamDetSetup class:
```
Double_t ERBeamDetSetup::fTargetH2R = 3.;   //cm
Double_t ERBeamDetSetup::fTargetH2Z = 1e-3;   //cm
Double_t ERBeamDetSetup::fTargetShellThicknessSide = 1e-5;
Double_t ERBeamDetSetup::fTargetShellThicknessZ = 1e-5;
Bool_t   ERBeamDetSetup::fSensitiveTargetIsSet = false;
```

- in the BeamDet parameters xml-file:
```
<BeamDetParts>
	<ToFTypes>
		<ToF id="ToF1">
			<plasticGeometry X="100" Y="100" Z="0.01"/>
			<plasticMedia>BC408</plasticMedia>
		</ToF>
		<ToF id="ToF2">
			<plasticGeometry X="100" Y="100" Z="0.01"/>
			<plasticMedia>BC408</plasticMedia>
		</ToF>
	</ToFTypes>
	<MWPCTypes>
		<MWPC id="MWPC1">
			<gasVolGeometry X="5" Y="5" Z="8.2"/>
			<gasStripGeometry X="0.125" Y="5" Z="0.6"/>
			<gasPlanesOffset X="0." Y="0."/>
			<distBetweenXandYStrips>1</distBetweenXandYStrips>
			<cathodeThickness>0.0005</cathodeThickness>
			<kaptonWindowThickness>0.00125</kaptonWindowThickness>
			<anodeWireDiameter>0.002</anodeWireDiameter>
			<kaptonWindowMedia>kapton</kaptonWindowMedia>
			<cathodeMedia>aluminium</cathodeMedia>
			<anodeWireMedia>tungsten</anodeWireMedia>
			<gasMedia>CF4_CH4</gasMedia>
		</MWPC>
		<MWPC id="MWPC2">
			<gasVolGeometry X="5" Y="5" Z="8.2"/>
			<gasStripGeometry X="0.125" Y="5" Z="0.6"/>
			<gasPlanseOffset X="0." Y="0."/>
			<distBetweenXandYStrips>1</distBetweenXandYStrips>
			<cathodeThickness>0.0005</cathodeThickness>
			<kaptonWindowThickness>0.00125</kaptonWindowThickness>
			<anodeWireDiameter>0.002</anodeWireDiameter>
			<kaptonWindowMedia>kapton</kaptonWindowMedia>
			<cathodeMedia>aluminium</cathodeMedia>
			<anodeWireMedia>tungsten</anodeWireMedia>
			<gasMedia>CF4_CH4</gasMedia>
		</MWPC>
	</MWPCTypes>
```

![](https://github.com/ExpertRootGroup/er/tree/278_BeamDetTrackFindingQAcoordAndAng/macro/QA/BeamDet/ComparisonRecoAndSimCoordinates/compareCanvas.png  "BeamDetReconstructionQuality")
*X-coordinate difference [cm] (on the left pad) and Px/Pz on the right one*


The sequence of the macros running:

1. recoAndSimCoordAndAng_sim_digi.C --- simulation and digitization.
2. recoAndSimCoordAndAng_reco.C --- track finding procedure.
3. compareResults.C --- macro for building resulting histograms.

The input/ directory contains data for the non-uniform thin detector geometry: inhomogeneity maps and preparation macro.
In current EXP201811 simulation state here is only maps for the left thin detector.
For now, until clarification of the thin detector calibration, three maps are needed:
* sensitive thickness map (map_sens.root)
* map of the front dead layer for the single silicone station (map_deadFront.root)
* map of the back dead layer for the single silicone station (map_deadBack.root)

input/map_left.root --- non-uniformity map from the calibration stage.

The prepareMaps.C macro is needed to prepare maps after the calibration process.

Other files may be useful further and for now, aren't used

Use exp1811_sim.C + exp1811_reco.C to get reconstruction results.
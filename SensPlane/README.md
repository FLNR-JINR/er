# Sensetive plane

Sensitive plane is a detector for debug purposes.
It is a simple box with size, material and position defined in macro.
Unlike other detectors, the points of a given one are stored regardless of whether there was a loss of energy or not.
So, it can be used for the purpose of assessing gemetric efficiency separately from the efficiency of physical processes.

# Usage

```
ERSensPlane* sensPlane = new ERSensPlane();
sensPlane->SetDetectorGeometry(1., 1., 0.1, "vacuum");
sensPlane->SetDetectorPositioning(0., 0., 10.);
run->AddModule(sensPlane);
```

Plane of vacuum with size `2x2x0.2 cm` was created and placed at posisition `{0., 0., 10.}` (rotation is also allowed).


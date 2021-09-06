void calc_eloss() {
  FairLogger::GetLogger()->SetLogScreenLevel("DEBUG");
  // --------------------- Calculator initialization --------------------------
  ERElossCalculatorUtils *calculator = ERElossCalculatorUtils::Instance();
  calculator->Init();
  // --------------------------------------------------------------------------
  const float energy = 7.6869; // [MeV] 
  const float thickness = 40.0; // [um]
  const unsigned pid = 1000020040;
  const TString material = "silicon";
  const float number_of_steps = 1000.0; // number of integration steps
  const bool ifBackPropagation = false;

  const float eloss = calculator->IntegralEloss(pid, energy, material, 
                                                thickness, number_of_steps, ifBackPropagation);

  // A particle is `propagated` throughtout material in forth direction
  //  -> "direct energy losses" are summed up
  // Then particle is `propagated` backward
  //  -> "back energy acquisition" is summed up, energy recovers to initial value
  // Function returns difference between "direct energy losses" and "back energy losses"
  const float diff = calculator->BackAndForthPropagation(pid, energy, 
                                                         material, thickness, number_of_steps);
  std::cout << "diff " << diff << std::endl;
}

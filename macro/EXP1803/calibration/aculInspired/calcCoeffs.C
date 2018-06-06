void calcCoeffs () {
  long double alpha = 44;
  vector<long double> fCalibEnergies = {4.7844, 7.6869};
  long double dEdxDir_1 = 0.15045; // 0.46256
  long double dEdxDir_2 = 0.11065; // 0.33583
  // long double dEdxRot_1 = 0.21167; // 0.65701
  // long double dEdxRot_2 = 0.15527; // 0.47343
  TArrayD data(4 * 4);
  ifstream inDirect;
  ifstream inRotate;
  ofstream outParams;
  outParams.open("out_acbd.txt");
  inDirect.open("channelsDirect.txt");
  inRotate.open("channelsRotate.txt");
  for (Int_t subAddr_X = 0; subAddr_X < 32; subAddr_X++) {
    // long double channelDirect_min;  
    // long double channelDirect_max;  
    for (Int_t subAddr_Y = 0; subAddr_Y < 16; subAddr_Y++) {
      inDirect >> data[0 + 0 * 4];
      inDirect >> data[0 + 1 * 4];
      inRotate >> data[0 + 2 * 4];
      inRotate >> data[0 + 3 * 4];
      for (Int_t i = 0; i < 4; i++) {
        data[1 + i * 4] = 1;
      }
      data[2 + 0 * 4] = dEdxDir_1;
      data[2 + 1 * 4] = dEdxDir_2;
      data[2 + 2 * 4] = dEdxDir_1;
      data[2 + 3 * 4] = dEdxDir_2;

      data[3 + 0 * 4] = dEdxDir_1;
      data[3 + 1 * 4] = dEdxDir_2;
      data[3 + 2 * 4] = dEdxDir_1 / cos(alpha * TMath::DegToRad());
      data[3 + 3 * 4] = dEdxDir_2 / cos(alpha * TMath::DegToRad());
  
      TMatrixD eqMatrix(4, 4);
      eqMatrix.Use(4, 4, data.GetArray());
      TVectorD ans(4);
      ans[0] = fCalibEnergies[0];
      ans[1] = fCalibEnergies[1];
      ans[2] = fCalibEnergies[0];
      ans[3] = fCalibEnergies[1];
      if (subAddr_X == 14 && subAddr_Y == 15) {
        cout << " ";
        for (int i = 0; i < 4; i++) {
          cout << ans[i] << " ";
        }
        cout << endl;
      }
      TDecompLU lu(eqMatrix);
      lu.Solve(ans);

      for (int i = 0; i < 4; i++) {
        outParams << ans[i] << ",";
      }
      outParams << ";\t\n";
      if (subAddr_X == 14 &&  subAddr_Y == 15) {
        for (int i = 0; i < 4; i++) {
          for (int j = 0; j < 4; j++) {
            cout << eqMatrix[i][j] << " ";
          }
          cout << endl;
        }
        cout  << endl; 
      }

    }
    outParams << endl;
  }
  
  // cout << eqMatrix[0][0] << endl;
  // for (int i = 0; i < 4; i++) {
  //   for (int j = 0; j < 4; j++) {
  //     cout << eqMatrix[i][j] << " ";
  //   }
  //   cout << endl;
  // }

  // for (int i = 0; i < 4; i++) {
  //   cout << ans[i] << " ";
  // }
  // cout << endl;
  // cout << endl;
  // lu.Decompose();
  // for (int i = 0; i < 4; i++) {
  //   for (int j = 0; j < 4; j++) {
  //     cout << eqMatrix[i][j] << " ";
  //   }
  //   cout << endl;
  // }
  // cout << endl;
  // cout << endl;
  // for (int i = 0; i < 4; i++) {
  //   cout << ans[i] << " ";
  // }
  // cout << endl;
}


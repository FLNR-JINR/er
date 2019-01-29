struct SArrays
{
    Double_t* sAr;
    Double_t* BAr;
    Double_t* NAr;
};

bool Draw_Experimental_Points(TCanvas* cn, TLegend* leg);
bool Draw_Base_Cross_Section(TCanvas* cn, TLegend* leg);
SArrays* Fill_Arrays(Int_t anglesNumbers, Bool_t N15_B11_draw);
Int_t* GetnEventsInTarget(Int_t anglesNumbers, Bool_t N15_B11_draw);
Double_t* GetdPhiAr(Int_t anglesNumbers);
Double_t* GetThetaCMAr(Int_t anglesNumbers, Bool_t N15_or_B11); // N15_or_B11 kTRUE for N15 kFALSE for B11

//---------------------------------------------------------------------------------------------------------------------
void cross_section(Int_t nEvents = 100, Double_t begAng = 34., Int_t nThreads = 3, Int_t anglesNumbers = 0, Double_t STEP=1.,

                  Int_t case_n = 0, TString workDir = "output", Bool_t N15_B11_draw = kFALSE)
{
    //Double_t norm = 1.;
    nEvents = nEvents*nThreads;
    Double_t dTheta = 0.262822833*TMath::DegToRad();
    Double_t Radius = 218.; // mm
    //Double_t detH = 4.; // mm
    TCanvas* canv = new TCanvas("canv", "canv", 1000, 800);
    canv->SetLogy();
    TLegend* leg = new TLegend(1., 1., 0.80, 0.80);

    if ( !Draw_Base_Cross_Section(canv, leg) ) {
        cerr << "Draw_Cross_Section() Error " << endl;
        return;
    }
/*
    if ( !Draw_Experimental_Points(canv, leg) )
    {
        cerr << "Draw_Experimental_Points error\n";
        return;
    }
*/
    SArrays* Ar = Fill_Arrays(anglesNumbers, N15_B11_draw);
    if (!Ar)
    {
        cerr << "Fill_Arrays Error N15" << endl;
        return;
    }
    Double_t* summAr = Ar->sAr;
    Double_t* nB11Ar = Ar->BAr;
    Double_t* nN15Ar = Ar->NAr;

    Int_t* nEventsAr = GetnEventsInTarget(anglesNumbers, N15_B11_draw);// Identify events number in target for all curent angles
    if (!nEventsAr)
    {
        cerr << "GetnEventsInTarget Error " << endl;
        return;
    }

    Double_t* dPhiAr = GetdPhiAr(anglesNumbers); // Get dPhi ranges array
    if (!dPhiAr)
    {
        cerr << "GetdPhiAr Error " << endl;
        return;
    }

    // Output file
    ofstream fout("output/N15_cross_and_theta.txt");
    if (!fout.is_open())
    {
        cerr << "Error: missing output directory\n";
        return;
    }

    // Get thetaCM values for N15
    Bool_t N15_or_B11 = kTRUE;
    Double_t* ThetaCMAr = GetThetaCMAr(anglesNumbers, N15_or_B11);

    Int_t i = 0;
    Double_t ratio = 1.3626837426803;
    TVectorD sigmaCMN15(anglesNumbers);
    TVectorD tetN15(anglesNumbers);
    for (i = 0; i < anglesNumbers; i++) {
        // Curent theta for N15 calculate
        Double_t curAngle = TMath::DegToRad()*((Double_t)i*STEP + begAng);
        Double_t iA = ratio*sin(curAngle-dTheta)*sin(curAngle-dTheta);
        Double_t iB = cos(curAngle-dTheta)*sqrt(1. - ratio*ratio*sin(curAngle-dTheta)*sin(curAngle-dTheta));
        Double_t iC = acos(-iA + iB);
        Double_t theta1 = TMath::RadToDeg()*iC;
        iA = ratio*sin(curAngle+dTheta)*sin(curAngle+dTheta);
        iB = cos(curAngle+dTheta)*sqrt(1. - ratio*ratio*sin(curAngle+dTheta)*sin(curAngle+dTheta));
        iC = acos(-iA + iB);
        Double_t theta2 = TMath::RadToDeg()*iC;
        //tetN15(i) = 0.5*(theta2-theta1) + theta1;
        //cout << "N15: old Theta: " << tetN15(i) << ", new Theta: " << ThetaCMAr[i] << endl;
        tetN15(i) = ThetaCMAr[i];

        // Curent cross-section calculate
        nEvents = nEventsAr[i];
/*
        Double_t dPhi = TMath::DegToRad()*detH*180./(Radius*sin(curAngle)*TMath::Pi());
        Double_t Omega = 0.5;//dPhi*(cos(TMath::DegToRad()*theta1) - cos(TMath::DegToRad()*theta2))/4./TMath::Pi();
        Double_t Integrat = (Double_t)nEvents*2.*TMath::Pi()*Omega/summAr[i];
        dPhi = 1.;//TMath::DegToRad()*dPhiAr[i]/2./TMath::Pi();
        Double_t crossSecLab = (Double_t)nN15Ar[i]*dPhi/Integrat ;
        iA = 1. + ratio*ratio*cos(2.*curAngle);
        iB = 1. - ratio*ratio*sin(curAngle)*sin(curAngle);
        iC = sqrt(iB)/iA;
*/
        Double_t dPhiDet = 4.*180. / (TMath::Pi()*Radius*TMath::Sin(curAngle));
        Double_t dphi = dPhiAr[i]/dPhiDet;
        sigmaCMN15(i) = (Double_t)nN15Ar[i]*summAr[i]*dphi / (nEvents*2.*TMath::Pi()*TMath::Sin(TMath::DegToRad()*tetN15(i))*(theta2-theta1));
        fout << tetN15(i) << "\t" << sigmaCMN15(i) << endl;
    }
    fout.clear();
    fout.close();
    TGraph* simN15Gr = new TGraph(tetN15, sigmaCMN15);

    canv->cd();
    simN15Gr->Draw("P");
    simN15Gr->SetMarkerStyle(8);
    simN15Gr->SetMarkerColor(4);

    leg->AddEntry(simN15Gr, "N15 Points", "p");

    //N15_B11_draw = kTRUE;
    if (N15_B11_draw)
    {
        Ar = Fill_Arrays(anglesNumbers, N15_B11_draw);
        if (!Ar)
        {
            cerr << "Fill_Arrays Error B11" << endl;
            return;
        }
        delete []summAr;
        delete []nB11Ar;
        delete []nN15Ar;
        summAr = Ar->sAr;
        nB11Ar = Ar->BAr;
        nN15Ar = Ar->NAr;

        nEventsAr = GetnEventsInTarget(anglesNumbers, N15_B11_draw);
    }

    // Fill thetaCM array for B11
    N15_or_B11 = kFALSE;
    delete []ThetaCMAr;
    ThetaCMAr = GetThetaCMAr(anglesNumbers, kFALSE);
    fout.open("output/B11_cross_and_theta.txt");
    if (!fout.is_open())
    {
        cerr << "Error: missing output directory" << endl;
        return;
    }
    TVectorD sigmaCMB11(anglesNumbers);
    TVectorD tetB11(anglesNumbers);
    Int_t memNEv = nEvents;
    for (i = 0; i < anglesNumbers; i++) {
        // Curent theta for B11 calculate
        Double_t curAngle = TMath::DegToRad()*((Double_t)i*STEP + begAng);
        Double_t theta1 = 180. - 2.*TMath::RadToDeg()*(curAngle-dTheta);
        Double_t theta2 = 180. - 2.*TMath::RadToDeg()*(curAngle+dTheta);
        tetB11(i) = 0.5*(theta2-theta1) + theta1;
        //cout << "B11: old Theta: " << tetB11(i) << ", new Theta: " << ThetaCMAr[i] << endl;
        tetB11(i) = ThetaCMAr[i];
        // Curent cross-section for B11 calculate
        nEvents = nEventsAr[i];
/*
        Double_t dPhi = TMath::DegToRad()*detH*180./(Radius*sin(curAngle)*TMath::Pi());
        Double_t Omega = 0.5;//dPhi*(cos(TMath::DegToRad()*theta2) - cos(TMath::DegToRad()*theta1))/4./TMath::Pi();
        Double_t Integrat = (Double_t)nEvents*2.*TMath::Pi()*Omega/summAr[i];
        dPhi = 1.;//TMath::DegToRad()*dPhiAr[i]/2./TMath::Pi();
        Double_t crossSecLab = (Double_t)nB11Ar[i]*dPhi/Integrat;
*/
        Double_t dPhiDet = 4.*180. / (TMath::Pi()*Radius*TMath::Sin(curAngle));
        Double_t dphi = dPhiAr[i]/dPhiDet;
        cout << dPhiAr[i] << " / " << dPhiDet << " = " << dphi << endl;
        sigmaCMB11(i) = (Double_t)nB11Ar[i]*summAr[i]*dphi / (nEvents*TMath::Pi()*TMath::Sin(TMath::DegToRad()*tetB11(i))*(-theta2+theta1));
        fout << tetB11(i) << "\t" << sigmaCMB11(i) << endl;
    }
    fout.clear();
    fout.close();

    TGraph* simB11Gr = new TGraph(tetB11, sigmaCMB11);

    canv->cd();
    simB11Gr->Draw("P");
    simB11Gr->SetMarkerStyle(21);
    simB11Gr->SetMarkerColor(1);

    leg->AddEntry(simB11Gr, "B11 Points", "p");

    canv->cd();
    TString headerStr;
    headerStr.Form("Log scale, case %d", case_n);
    leg->SetHeader(headerStr);
    leg->Draw();

    gPad->SetGrid(4, 4);
    canv->SaveAs("result/resultGr.pdf");
}
//------------------------------------------------------------------------------------------------------------------

// ----------Globals--------------------------------------
TGraph* sigmaGr = NULL;

Double_t Sigma(Double_t *x, Double_t *par)
{
  return sigmaGr->Eval(x[0]);
}
//---------------------------------------------------------
bool Draw_Base_Cross_Section(TCanvas* cn, TLegend* leg)
{
    TString fileName = "input/nrv.txt";
    std::ifstream f;
    f.open(fileName.Data());
    if (!f.is_open()) {
      cout << "Can't open file " << fileName << endl;
      return kFALSE;
    }

    Int_t anglesNumbers = std::count(std::istreambuf_iterator<char>(f),
                               std::istreambuf_iterator<char>(), '\n');
    f.seekg(0, std::ios::beg);
    TVectorD tet(anglesNumbers);
    TVectorD sigma(anglesNumbers);

    Int_t i = 0;
    while (!f.eof())
    {
      if (i == anglesNumbers) break;
      f >> tet(i) >> sigma(i);
      i++;
    }

    sigmaGr = new TGraph(tet, sigma);

    cn->cd();
    TF1* sigmaFun = new TF1("Cross-Section", Sigma, 4.5, 180., 0);
    sigmaFun->Draw("C");
    sigmaFun->GetXaxis()->SetTitle("theta");
    sigmaFun->GetYaxis()->SetTitle("cross-section");
    sigmaFun->SetLineWidth(4);
    sigmaFun->SetMarkerStyle(8);
    sigmaFun->SetMinimum(1.0e-11);
    leg->AddEntry(sigmaFun, "Theory", "l");
    f.close();
    return kTRUE;
}

bool Draw_Experimental_Points(TCanvas* canv, TLegend* leg)
{
    ifstream fin("input/CS_15N_11B.txt");
    if (!fin.is_open())
    {
        cerr << "input/CS_15N_11B.txt read error\n";
        return kFALSE;
    }

    Int_t N15_B11_Points = 66;
    Int_t i;

    TVectorD sigN15_B11(N15_B11_Points);
    TVectorD thetaN15_B11(N15_B11_Points);
    for (i = 0; i < N15_B11_Points; i++)
    {
        Double_t theta;
        Double_t cs;
        fin >> theta;
        fin >> cs;
        thetaN15_B11(i) = theta;
        sigN15_B11(i) = cs;
        //cout << cs << ", " << theta << endl;
    }

    TGraph* N15_B11_Pgr = new TGraph(thetaN15_B11, sigN15_B11);
    canv->cd();
    N15_B11_Pgr->Draw("P");
    N15_B11_Pgr->SetMarkerStyle(22);
    N15_B11_Pgr->SetMarkerColor(8);
    leg->AddEntry(N15_B11_Pgr, "Experimental Points", "p");

    fin.clear();
    fin.close();
    return kTRUE;
}
SArrays* Fill_Arrays(Int_t n, Bool_t N15_B11_draw)
{
    ifstream fin;
    if (!N15_B11_draw)
        fin.open("input/out.txt");
    else
        fin.open("input/outB11.txt");
    if (!fin.is_open())
    {
        cerr << "out.txt file was not found\n";
        return NULL;
    }
    Double_t* summAr = new Double_t [n];
    Double_t* nB11Ar = new Double_t [n];
    Double_t* nN15Ar = new Double_t [n];

    Int_t iSumm = 0;
    Int_t inB11 = 0;
    Int_t inN15 = 0;
    TString countString;
    while (!fin.eof())
    {
        if (iSumm > n || inB11 > n || inN15 > n) return NULL;
        fin >> countString;
        Double_t vary;
        if (countString == "summ:")
        {
            fin >> vary;
            summAr[iSumm] = vary;
            iSumm++;
        }
        else if (countString == "nB11Gas:")
        {
            fin >> vary;
            nB11Ar[inB11] = vary;
            inB11++;
        }
        else if (countString == "nN15Gas:")
        {
            fin >> vary;
            nN15Ar[inN15] = vary;
            inN15++;
        }
    }
    fin.close();
/*
    for (Int_t i = 0; i < n; i++)
    {
        cout << summAr[i] << " " << nB11Ar[i] << " " << nN15Ar[i] << endl;
    }
*/
    SArrays* ar = new SArrays;
    ar->sAr = summAr;
    ar->BAr = nB11Ar;
    ar->NAr = nN15Ar;

    return ar;
}

Int_t* GetnEventsInTarget(Int_t anglesNumbers, Bool_t N15_B11_draw)
{
    TString fileName;
    if (!N15_B11_draw)
        fileName = "input/target_int_num.txt";
    else
        fileName = "input/target_int_numB11.txt";
    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        cerr << "Can't open file for events number in target" << fileName << endl;
        return NULL;
    }
    Int_t* nEventsAr = new Int_t [anglesNumbers];
    Int_t nAngles = 0;
    Bool_t activ = kFALSE;
    while (!fin.eof() && nAngles < anglesNumbers)
    {
        Int_t nEventsInTarget = 0;
        TString countString;
        fin >> countString;

        if (countString == "Angle:" || activ)
        {
            fin >> countString;
            while (!fin.eof() && countString != "Angle:")
            {
                if (countString == "target:")
                {
                    Int_t number;
                    fin >> number;
                    nEventsInTarget += number;
                }
                fin >> countString;
            }
            nEventsAr[nAngles] = nEventsInTarget;
            nAngles++;
            activ = kTRUE;
        }
    }
    fin.clear();
    fin.close();
    return nEventsAr;
}

Double_t* GetdPhiAr(Int_t anglesNumbers)
{
    TString dPhiFileName = "input/dPhi_info.txt";
    std::ifstream finDPhi(dPhiFileName.Data());
    if (!finDPhi.is_open())
    {
        cerr << "Can't open file " << dPhiFileName << endl;
        return NULL;
    }
    Double_t* dPhiAr = new Double_t [anglesNumbers];
    Int_t nAngles = 0;
    while (!finDPhi.eof())
    {
        TString countString;
        finDPhi >> countString;
        if (countString == "range:")
        {
            Double_t number;
            finDPhi >> number;
            dPhiAr[nAngles] = number;
            nAngles++;
            if (nAngles > anglesNumbers) break;
        }
    }
    finDPhi.clear();
    finDPhi.close();
    return dPhiAr;
}

Double_t* GetThetaCMAr(Int_t anglesNumbers, Bool_t N15_or_B11)
{
    std::ifstream fin;
    if (N15_or_B11)
        fin.open("input/thetaCMN15.txt");
    else
        fin.open("input/thetaCMB11.txt");
    if (!fin.is_open())
    {
        cerr << "Can't open file for thetaCM" << endl;
        return NULL;
    }
    Double_t* ThetaCMAr = new Double_t [anglesNumbers];
    Int_t nAngles = 0;
    while (!fin.eof())
    {
        fin >> ThetaCMAr[nAngles];
        nAngles++;
        if (nAngles > anglesNumbers) break;
    }
    fin.clear();
    fin.close();
    return ThetaCMAr;
}

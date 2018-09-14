struct SArrays
{
    Double_t* sAr;
    Double_t* BAr;
    Double_t* NAr;
};

bool Draw_Experimental_Points(TCanvas* cn, TLegend* leg);
bool Draw_Base_Cross_Section(TCanvas* cn, TLegend* leg);
SArrays* Fill_Arrays(Int_t anglesNumbers);
Int_t* GetnEventsInTarget(Int_t anglesNumbers);
Double_t* GetdPhiAr(Int_t anglesNumbers);

//---------------------------------------------------------------------------------------------------------------------
void cross_section(Int_t nEvents = 100, Double_t begAng = 34., Int_t nThreads = 3, Int_t anglesNumbers = 0, Double_t STEP=1.,
                   TString workDir = "output")
{
    nEvents = nEvents*nThreads;

    TCanvas* canv = new TCanvas("canv", "canv", 1000, 800);
    TLegend* leg = new TLegend(1., 1., 0.80, 0.80);

    if ( !Draw_Base_Cross_Section(canv, leg) ) {
        cerr << "Draw_Cross_Section() Error " << endl;
        return;
    }

    if ( !Draw_Experimental_Points(canv, leg) )
    {
        cerr << "Draw_Experimental_Points error\n";
        return;
    }

    SArrays* Ar = Fill_Arrays(anglesNumbers);
    if (!Ar)
    {
        cerr << "Fill_Arrays Error " << endl;
        return;
    }
    Double_t* summAr = Ar->sAr;
    Double_t* nB11Ar = Ar->BAr;
    Double_t* nN15Ar = Ar->NAr;

    Int_t* nEventsAr = GetnEventsInTarget(anglesNumbers);// Identify events number in target for all curent angles
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

    ofstream fout("output/N15_cross_and_theta.txt");
    if (!fout.is_open())
    {
        cerr << "Error: missing output directory\n";
        return;
    }
    Int_t i = 0;
    Double_t ratio = 1.3626837426803;
    TVectorD sigmaCMN15(anglesNumbers);
    TVectorD tetN15(anglesNumbers);
    for (i = 0; i < anglesNumbers; i++) {
        nEvents = nEventsAr[i];
        double Integrat = ( (double)nEvents/summAr[i] );
        double crossSecLab = ( (double)nN15Ar[i]*2.*TMath::Pi()/dPhiAr[i]/Integrat );
        double curAngle = ((double)i*STEP + begAng)*TMath::Pi()/180.;
        double iA = 1. + ratio*ratio*cos(2.*curAngle);
        double iB = 1. - ratio*ratio*sin(curAngle)*sin(curAngle);
        double iC = sqrt(iB)/iA;
        sigmaCMN15(i) = log10(0.5*crossSecLab*iC);
        iA = ratio*sin(curAngle)*sin(curAngle);
        iB = cos(curAngle)*sqrt(1. - ratio*ratio*sin(curAngle)*sin(curAngle));
        iC = acos(-iA + iB);
        tetN15(i) = iC*180./TMath::Pi();
        fout << 0.5*crossSecLab*iC << "\t" << tetN15(i) << endl;
    }

    TGraph* simN15Gr = new TGraph(tetN15, sigmaCMN15);

    canv->cd();
    simN15Gr->Draw("P");
    simN15Gr->SetMarkerStyle(8);
    simN15Gr->SetMarkerColor(4);

    leg->AddEntry(simN15Gr, "N15 Points", "p");

    TVectorD sigmaCMB11(anglesNumbers);
    TVectorD tetB11(anglesNumbers);
    Int_t memNEv = nEvents;
    for (i = 0; i < anglesNumbers; i++) {
        nEvents = nEventsAr[i];
        double Integrat = ( (double)nEvents/summAr[i] );
        double crossSecLab = ( (double)nB11Ar[i]*2.*TMath::Pi()/dPhiAr[i]/Integrat);
        double curAngle = ((double)i*STEP + begAng)*TMath::Pi()/180.;
        sigmaCMB11(i) = log10(0.25*crossSecLab/cos(curAngle));
        tetB11(i) = 180. - 2*curAngle*180./TMath::Pi();
    }

    TGraph* simB11Gr = new TGraph(tetB11, sigmaCMB11);

    canv->cd();
    simB11Gr->Draw("P");
    simB11Gr->SetMarkerStyle(21);
    simB11Gr->SetMarkerColor(1);

    leg->AddEntry(simB11Gr, "B11 Points", "p");

    canv->cd();
    leg->SetHeader("Result Graphs");
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
      sigma(i) = log10(sigma(i));
      i++;
    }

    sigmaGr = new TGraph(tet, sigma);

    cn->cd();
    TF1* sigmaFun = new TF1("Cross-Section", Sigma, 4.5, 180., 0);
    sigmaFun->Draw("C");
    sigmaFun->GetXaxis()->SetTitle("theta");
    sigmaFun->GetYaxis()->SetTitle("lg(cross-section)");
    sigmaFun->SetLineWidth(4);
    sigmaFun->SetMarkerStyle(8);
    leg->AddEntry(sigmaFun, "Theory", "l");
    f.close();
    return kTRUE;
}

bool Draw_Experimental_Points(TCanvas* canv, TLegend* leg)
{
    ifstream fin("input/N15_experimental_cs.txt");
    if (!fin.is_open())
    {
        cerr << "input/N15_experimental_cs.txt read error\n";
        return kFALSE;
    }

    Int_t N15_Points = 35;
    Int_t B11_Points = 32;
    Int_t i;

    TVectorD sigN15(N15_Points);
    TVectorD thetaN15(N15_Points);
    for (i = 0; i < N15_Points; i++)
    {
        Double_t cs;
        Double_t theta;
        fin >> cs;
        fin >> theta;
        sigN15(i) = log10(cs);
        thetaN15(i) = theta;
        //cout << cs << ", " << theta << endl;
    }

    TGraph* N15Pgr = new TGraph(thetaN15, sigN15);
    canv->cd();
    N15Pgr->Draw("P");
    N15Pgr->SetMarkerStyle(22);
    N15Pgr->SetMarkerColor(8);
    leg->AddEntry(N15Pgr, "N15 experimental Points", "p");

    fin.clear();
    fin.close();
    fin.open("input/B11_experimental_cs.txt");
    if (!fin.is_open())
    {
        cerr << "input/B11_experimental_cs.txt read error\n";
        return kFALSE;
    }

    TVectorD sigB11(B11_Points);
    TVectorD thetaB11(B11_Points);
    for (i = 0; i < B11_Points; i++)
    {
        Double_t cs;
        Double_t theta;
        fin >> cs;
        fin >> theta;
        sigB11(i) = log10(cs);
        thetaB11(i) = theta;
        //cout << cs << ", " << theta << endl;
    }

    TGraph* B11Pgr = new TGraph(thetaB11, sigB11);
    canv->cd();
    B11Pgr->Draw("P");
    B11Pgr->SetMarkerStyle(23);
    B11Pgr->SetMarkerColor(8);
    leg->AddEntry(B11Pgr, "B11 experimental Points", "p");

    fin.clear();
    fin.close();
    return kTRUE;
}
SArrays* Fill_Arrays(Int_t n)
{
    ifstream fin("input/out.txt");
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

Int_t* GetnEventsInTarget(Int_t anglesNumbers)
{
    TString fileName = "input/target_int_num.txt";
    std::ifstream fin(fileName);
    if (!fin.is_open()) {
        cerr << "Can't open file " << fileName << endl;
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
    return nEventsAr;
}

Double_t* GetdPhiAr(Int_t anglesNumbers)
{
    TString dPhiFileName = "input/dPhi_info.txt";
    std::ifstream finDPhi(dPhiFileName.Data());
    if (!finDPhi.is_open()) {
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
    return dPhiAr;
}

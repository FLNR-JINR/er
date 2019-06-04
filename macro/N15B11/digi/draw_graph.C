void draw_graph(Int_t index = 0, TString inputDirectory = "output_digi", TString outputDirectory = "digi_graphs")
{
    // Input file name form
    TString inputFileName;
    inputFileName.Form("%s/digi_%d.root", inputDirectory.Data(), index);

    // Open input file
    TFile* inputFile = new TFile(inputFileName, "READ");
    if (inputFile->IsZombie())
    {
        cerr << "Could not open file '" << inputFileName << endl;
        return;
    }
    else
    {
        cout << inputFileName << " has been opened successfull" << endl;
    }


    TTree* tree = (TTree*)inputFile->Get("er");
    if (!tree)
    {
        cerr << "Tree er was not found\n";
        return;
    }

    TCanvas* canv = new TCanvas("canv", "Gas vs Semi");
    tree->Draw("Gas_Layer_0_Det_1_Digi.fEdep:Semi_Layer_0_Det_1_Digi.fEdep");
    canv->Close();

    TCanvas* canv1 = new TCanvas("canv1", "graph", 1200, 800);
    TGraph* gr = new TGraph(tree->GetSelectedRows(), tree->GetV2(), tree->GetV1());
    cout << gr->GetN() << " points in the output graph" << endl;
    canv1->cd();
    gr->Draw("AP");
    gPad->SetGrid(2, 2);
    gr->SetMarkerStyle(7);
    gr->SetTitle("Gas Eloss vs. Semi Eloss");
    gr->GetYaxis()->SetTitle("Gas_Eloss (GeV)");
    gr->GetXaxis()->SetTitle("Semi_Eloss (GeV)");


    TString outputFileName;
    outputFileName.Form("%s/digi_graph.pdf", outputDirectory.Data());
    //canv1->SaveAs(outputFileName);
    //canv1->Close();

    TString outputGrFilename;
    outputGrFilename.Form("%s/graph_%d.root", outputDirectory.Data(), index);

    TFile* outputFile = new TFile(outputGrFilename, "RECREATE");
    gr->Write();
    outputFile->Close();
}

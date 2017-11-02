// Только Гаусс
// f = k * e^(-(x-mean)*(x-mean) / (2*sigma*sigma))
// 3 параметра
Double_t fitfG(Double_t *x, Double_t *par)
{
	Double_t fitval;
	fitval = par[0]*exp( -0.5*(x[0]-par[1])*(x[0]-par[1])/(par[2]*par[2]));
    return fitval;
}

void spectrumShape_gaussFitting()
{
	// Достаём спектр из файла
	TFile *f = new TFile("Spectrum1.root");
	TH1D *h = (TH1D*)f->Get("nxSpectrum_ch0");

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Вычитаем два отсчёта от исходной гистограммы
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	for (UInt_t i=1; i<=h->GetNbinsX(); i++) {
		if (h->GetBinContent(i) <= 2) {
			h->SetBinContent(i, 0);
		} else {
			h->SetBinContent(i, h->GetBinContent(i)-2);
		}
	}

	// Рисуем спектр
	TCanvas *c1 = new TCanvas("c1", "amp spectrum");
	c1->cd();
	h->Draw();

	// Установить диапазон по X для рисования
	h->GetXaxis()->SetRangeUser(0., 1200.);

	// Включить сетку на визуализаци
	gPad->SetGrid(1, 1);

	// Рисуем только Гаусса
	TF1 *funcG = new TF1("funcG", fitfG, 400., 900., 3);
	funcG->SetLineColor(kRed);
	funcG->SetLineWidth(4);

	// Фитирование Гауссом
	// Задаём начальные значения параметрам
	funcG->SetParameters(20., 600., 100.);
	// Собственно фит
	h->Fit("funcG", "R");

	// Вывод результатов в консоль
	cout << "k  = " << funcG->GetParameter(0) << endl;
	cout << "m  = " << funcG->GetParameter(1) << endl;
	cout << "s  = " << funcG->GetParameter(2) << endl;
}
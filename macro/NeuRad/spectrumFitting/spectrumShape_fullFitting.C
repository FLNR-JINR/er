// В результате фитирования спектра в диапазоне 400-900 только Гауссом
// были получены следующие параметры
// k = 52., mean = 433., sigma = 141.
// Затем Гаусс вычли из всего спектра и оставшееся фитировали
// кусочной функцией - {0, отрезок, парабола, 0}

// Кусочная функция - {0, отрезок, парабола, 0}
// 4 параметра
Double_t fitf2p(Double_t *x, Double_t *par)
{
	Double_t fitval;
	if (x[0]<63.) {
		fitval = 0.;
	}
	if (x[0]>=63. && x[0]<par[0]) {
		// Отрезок прямой
		fitval = (x[0]-63.) * (par[1]) / (par[0]-63.);
	}
	if (x[0]>=par[0] && x[0]<par[2]) {
		// Парабола
		fitval = par[1]*(x[0]-par[2])*(x[0]+par[2]-2*par[3])/((par[0]-par[2])*(par[0]+par[2]-2*par[3]));
	}
	if (x[0]>=par[2]) {
		fitval = 0.;
	}
	return fitval;
}

// Только Гаусс, обрезанный слева
// f = k * e^(-(x-mean)*(x-mean) / (2*sigma*sigma))
// 3 параметра
Double_t fitfG(Double_t *x, Double_t *par)
{
	Double_t fitval;
	if (x[0]<63.) {
		fitval = 0.;
	} else {
		fitval = par[0]*exp( -0.5*(x[0]-par[1])*(x[0]-par[1])/(par[2]*par[2]));
	}
	return fitval;
}

// Полная функция: кусочная + Гаусс
// 7 параметров
Double_t fitf(Double_t *x, Double_t *par)
{
	return fitf2p(x, par) + fitfG(x, &par[4]);
}

void spectrumShape_fullFitting()
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

	// Рисуем только Гаусса c заданными параметрами (52., 433., 141.)
	TF1 *funcG = new TF1("funcG", fitfG, 0., 2000., 3);
	funcG->SetParameters(52., 433., 141.);
	funcG->Draw("sames][");
	funcG->SetLineColor(kMagenta);

	// Вычитаем Гаусса из исходного спектра
	TH1D* h2 = (TH1D*)h->Clone();
	for (UInt_t i=1; i<=h->GetNbinsX(); i++) {
		h2->SetBinContent(i, h->GetBinContent(i) - funcG->Eval(h->GetXaxis()->GetBinCenter(i)));
	}

	// Рисуем результат вычитания
	h2->Draw("sames][");
	h2->SetLineColor(kCyan);

	// Кусочная функция - {0, отрезок, парабола, 0}
	TF1* func2p = new TF1("func2p", fitf2p, 0., 500., 4);
	func2p->SetParLimits(0, 70., 100.);
	func2p->SetParLimits(1, 20., 40.);
	func2p->SetParLimits(2, 440., 500.);
	func2p->SetParLimits(3, 440., 800.);
	func2p->SetParNames("x2", "y2", "x3", "b");
	// Фитирование результата вычитания кусочной функцией
	h2->Fit("func2p", "R");

	// Получение параметров кусочной функции
	Double_t x2, y2, x3, b;
	x2 = func2p->GetParameter(0);
	y2 = func2p->GetParameter(1);
	x3 = func2p->GetParameter(2);
	b = func2p->GetParameter(3);

	// Создание полной функции, описывающей форму исходного спектра
	TF1 *shape = new TF1("shape", fitf, 0., 1000., 7);
	shape->SetParameters(x2, y2, x3, b, 52., 433., 141.);
	shape->SetLineColor(kGreen);
	shape->SetLineWidth(4);
	shape->Draw("sames][");	

	// Вывод результатов в консоль
	cout << "x2 = " << x2 << endl;
	cout << "y2 = " << y2 << endl;
	cout << "x3 = " << x3 << endl;
	cout << "b  = " << b << endl;
	cout << "k  = " << "52." << endl;
	cout << "m  = " << "433." << endl;
	cout << "s  = " << "141." << endl;
}

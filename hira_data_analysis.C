#define XMIN "1070"
#define XMAX "1170"
#define YMIN "820"
#define YMAX "920"
#define CATMIN "500"
#define CATMAX "6000"

struct HistogramSet {
    TH2D* hist;
    TLegend* legend;
    
    HistogramSet(TH2D* h = nullptr, TLegend* l = nullptr) 
        : hist(h), legend(l) {}
};

HistogramSet g_tof_esum_set;
HistogramSet g_cathode_tof_set;
HistogramSet g_x_tof_set;

HistogramSet g_e1_eres_set;
HistogramSet g_e2_eres_set;
HistogramSet g_e3_eres_set;
HistogramSet g_e4_eres_set;

HistogramSet g_e1_esum_set;
HistogramSet g_e2_esum_set;
HistogramSet g_e3_esum_set;
HistogramSet g_e4_esum_set;

void UpdateCutInfo(HistogramSet& histSet) 
{
    if (!histSet.legend || !histSet.hist) return;
    
    Int_t event = gPad->GetEvent();
    if (event != kButton1Up && event != kButton1Down && event != kButton1Double) {
        return;
    }
    
    TCutG* currentCut = (TCutG*)gROOT->GetListOfSpecials()->FindObject("CUTG");
    if (!currentCut) {
        histSet.legend->Clear();
        histSet.legend->Draw();
        return;
    }
    
    double integral = currentCut->IntegralHist(histSet.hist);
    
    histSet.legend->Clear();
    TString area;
    area.Form("Area under the cut portion is: %.2f", integral);
    histSet.legend->AddEntry((TObject*)NULL, area.Data(), "");
    histSet.legend->Draw();
    gPad->Modified();
    gPad->Update();
}

void UpdateCutInfo_g_tof_esum() {
    UpdateCutInfo(g_tof_esum_set);
}

void UpdateCutInfo_g_cathode_tof() {
    UpdateCutInfo(g_cathode_tof_set);
}

void UpdateCutInfo_g_x_tof() {
    UpdateCutInfo(g_x_tof_set);
}

void UpdateCutInfo_g_e1_eres() {
    UpdateCutInfo(g_e1_eres_set);
}

void UpdateCutInfo_g_e2_eres() {
    UpdateCutInfo(g_e2_eres_set);
}

void UpdateCutInfo_g_e3_eres() {
    UpdateCutInfo(g_e3_eres_set);
}

void UpdateCutInfo_g_e4_eres() {
    UpdateCutInfo(g_e4_eres_set);
}

void UpdateCutInfo_g_e1_esum() {
    UpdateCutInfo(g_e1_esum_set);
}

void UpdateCutInfo_g_e2_esum() {
    UpdateCutInfo(g_e2_esum_set);
}

void UpdateCutInfo_g_e3_esum() {
    UpdateCutInfo(g_e3_esum_set);
}

void UpdateCutInfo_g_e4_esum() {
    UpdateCutInfo(g_e4_esum_set);
}

void ApplyHistogramStyle(TH1* hist, const char* xtitle, const char* ytitle) {
    hist->GetXaxis()->SetLabelFont(132);
    hist->GetXaxis()->SetTitleSize(0.05);
    hist->GetXaxis()->SetTitleOffset(0.89);
    hist->GetXaxis()->SetTitleFont(132);
    hist->GetXaxis()->CenterTitle(true);

    hist->GetYaxis()->SetLabelFont(132);
    hist->GetYaxis()->SetTitleSize(0.05);
    hist->GetYaxis()->SetTitleOffset(0.91);
    hist->GetYaxis()->SetTitleFont(132);
    hist->GetYaxis()->CenterTitle(true);

    if (hist->InheritsFrom(TH2::Class())) {
        ((TH2*)hist)->SetMinimum(1);
        ((TH2*)hist)->SetMaximum(100);
    }
}

TLegend* CreateStandardLegend() {
    TLegend* legend = new TLegend(0.28, 0.73, 0.68, 0.83);
    legend->SetLineWidth(0);
    legend->SetFillStyle(0);
    legend->SetTextFont(132);
    legend->SetTextSize(0.05);
    return legend;
}

void hira_data_analysis(const char* filename = "30Si_150Nd_E125pt5_q15_LR.001") {
    TChain niasRose("RoseNIAS");
    niasRose.Add(filename);

    gStyle->SetOptStat("nemri");
    
    /*-------------------------------------------------------*/

    // First canvas setup
    TCanvas *c1 = new TCanvas("c1", "Esum vs TOF");
    c1->SetWindowPosition(0, 0);
    g_tof_esum_set.hist = new TH2D("tof_esum", "TOF vs E_{sum};E_{sum};TOF", 
                                  1000, 0, 4500, 1000, 1, 8000);
    g_tof_esum_set.legend = CreateStandardLegend();

    ApplyHistogramStyle(g_tof_esum_set.hist, "E_{sum}", "TOF");
    niasRose.Draw("(TOF):(0.25*(E1_HG+E2_HG+E3_HG+E4_HG)) >> tof_esum", "", "COLZ");
    c1->AddExec("exec1", "UpdateCutInfo_g_tof_esum()");
    g_tof_esum_set.legend->Draw();

    /*-------------------------------------------------------*/    

    // Second canvas setup
    TCanvas *c2 = new TCanvas("c2", "Cathode vs TOF");
    c2->SetWindowPosition(100, 50);
    g_cathode_tof_set.hist = new TH2D("g_cathode_tof", "Cathode vs TOF;Cathode;TOF", 
                                     1000, 1, 8000, 1000, 1, 8000);
    g_cathode_tof_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_cathode_tof_set.hist, "E_{sum}", "TOF");
    niasRose.Draw("(TOF):(Cathode) >> g_cathode_tof", "", "COLZ");
    c2->AddExec("exec2", "UpdateCutInfo_g_cathode_tof()");
    g_cathode_tof_set.legend->Draw();

    /*-------------------------------------------------------*/

    // Third canvas setup
    TCanvas *c3 = new TCanvas("c3", "X vs TOF");
    c3->SetWindowPosition(200, 100);
    g_x_tof_set.hist = new TH2D("x_tof", "X vs TOF;X;TOF", 500, 1, 2000, 500, 1, 8000);
    g_x_tof_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_x_tof_set.hist, "X", "TOF");
    niasRose.Draw("(TOF):((XL-XR)+1000) >> x_tof", 
                  "((XL+XR) > " XMIN ") && ((XL+XR) < " XMAX ")", 
                  "COLZ");
    c3->AddExec("exec3", "UpdateCutInfo_g_x_tof()");
    g_x_tof_set.legend->Draw();


    /*-------------------------------------------------------*/

    TCanvas *c4 = new TCanvas("c4", "X vs Y");
    c4->SetWindowPosition(300, 150);
    TH2D *XY = new TH2D("XY", "X vs Y;X;Y", 500, 1, 2000, 500, 1, 2000);
    ApplyHistogramStyle(XY, "X", "Y");
    niasRose.Draw("((YU-YD)+1000):((XL-XR)+1000)>>XY",
              "((XL+XR)>" XMIN ") && ((XL+XR)<" XMAX ") &&" "((YU+YD)>" YMIN ") && ((YU+YD)<" YMAX ")",
              "COLZ");

    TCanvas *c5 = new TCanvas("c5", "ML");
    c5->SetWindowPosition(400, 200);
    TH1D *ml = new TH1D("ml", "M_{L} vs Count;M_{L};Count", 1000,0,8000);
    ApplyHistogramStyle(ml, "M_{L}", "Count");
    niasRose.Draw("(Mon_L) >> ml", "", "");

    TCanvas *c6 = new TCanvas("c6", "MR");
    c6->SetWindowPosition(500, 250);
    TH1D *mr = new TH1D("mr", "M_{R} vs Count;M_{R};Count", 1000,0,8000);
    ApplyHistogramStyle(mr, "M_{R}", "Count");
    niasRose.Draw("(Mon_R) >> mr", "", "");

    TCanvas *c7 = new TCanvas("c7", "Xsum");
    c7->SetWindowPosition(600, 300);
    TH1D *XSUM = new TH1D("XSUM", "X_{SUM} vs Count;XSUM;Count", 4096,1,4095);
    ApplyHistogramStyle(XSUM, "XSUM", "Count");
    niasRose.Draw("(XL+XR) >> XSUM", "", "");

    TCanvas *c8 = new TCanvas("c8", "Ysum");
    c8->SetWindowPosition(700, 350);
    TH1D *YSUM = new TH1D("YSUM", "Y_{SUM} vs Count;Y_{SUM};Count", 4096,1,4095);
    ApplyHistogramStyle(YSUM, "Y_{SUM}", "Count");
    niasRose.Draw("(YU+YD) >> YSUM", "", "");

/*--------------------------------------------------------------------*/

    TCanvas *c9 = new TCanvas("c9", "E1 vs Eresidual");
    c9->SetWindowPosition(800, 400);
    g_e1_eres_set.hist = new TH2D("g_e1_eres", "E_{1} vs E_{residual};E_{1};E_{residual}", 1000, 1, 4500, 1000, 1, 8000);
    g_e1_eres_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e1_eres_set.hist, "E_{1}", "E_{residual}");
    niasRose.Draw("(E1_HG):(0.33*(E2_HG+E3_HG+E4_HG)) >> g_e1_eres", "", "COLZ");
    c9->AddExec("exec4", "UpdateCutInfo_g_e1_eres()");
    g_e1_eres_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c10 = new TCanvas("c10", "E2 vs Eresidual");
    c10->SetWindowPosition(900, 450);
    g_e2_eres_set.hist = new TH2D("g_e2_eres", "E_{2} vs E_{residual};E_{2};E_{residual}", 1000, 1, 4500, 1000, 1, 8000);
    g_e2_eres_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e2_eres_set.hist, "E_{2}", "E_{residual}");
    niasRose.Draw("(E2_HG):(0.33*(E1_HG+E3_HG+E4_HG)) >> g_e2_eres", "", "COLZ");
    c10->AddExec("exec5", "UpdateCutInfo_g_e2_eres()");
    g_e2_eres_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c11 = new TCanvas("c11", "E3 vs Eresidual");
    c11->SetWindowPosition(1000, 500);
    g_e3_eres_set.hist = new TH2D("g_e3_eres", "E_{3} vs E_{residual};E_{3};E_{residual}", 1000, 0, 4500, 1000, 1, 8000);
    g_e3_eres_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e3_eres_set.hist, "E_{3}", "E_{residual}");
    niasRose.Draw("(E3_HG):(0.33*(E1_HG+E2_HG+E4_HG)) >> g_e3_eres", "", "COLZ");
    c11->AddExec("exec6", "UpdateCutInfo_g_e3_eres()");
    g_e3_eres_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c12 = new TCanvas("c12", "E4 vs Eresidual");
    c12->SetWindowPosition(1100, 550);
    g_e4_eres_set.hist = new TH2D("g_e4_eres", "E_{4} vs E_{residual};E_{4};E_{residual}", 1000, 1, 4500, 1000, 1, 8000);
    g_e4_eres_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e4_eres_set.hist, "E_{4}", "E_{residual}");
    niasRose.Draw("(E4_HG):(0.33*(E1_HG+E2_HG+E3_HG)) >> g_e4_eres", "", "COLZ");
    c12->AddExec("exec7", "UpdateCutInfo_g_e4_eres()");
    g_e4_eres_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c13 = new TCanvas("c13", "E1 vs Esum");
    c13->SetWindowPosition(1200, 600);
    g_e1_esum_set.hist = new TH2D("g_e1_esum", "E_{1} vs E_{sum};E_{sum};E_{1}", 1000, 1, 4500, 1000, 1, 8000);
    g_e1_esum_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e1_esum_set.hist, "E_{1}", "E_{sum}");
    niasRose.Draw("(0.25*(E1_HG+E2_HG+E3_HG+E4_HG)):(E1_HG) >> g_e1_esum", "", "COLZ");
    c13->AddExec("exec8", "UpdateCutInfo_g_e1_esum()");
    g_e1_esum_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c14 = new TCanvas("c14", "E2 vs Esum");
    c14->SetWindowPosition(0, 200);
    g_e2_esum_set.hist = new TH2D("g_e2_esum", "E_{2} vs E_{sum};E_{sum};E_{2}", 1000, 1, 4500, 1000, 1, 8000);
    g_e2_esum_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e2_esum_set.hist, "E_{2}", "E_{sum}");
    niasRose.Draw("(0.25*(E1_HG+E2_HG+E3_HG+E4_HG)):(E2_HG) >> g_e2_esum", "", "COLZ");
    c14->AddExec("exec9", "UpdateCutInfo_g_e2_esum()");
    g_e2_esum_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c15 = new TCanvas("c15", "E3 vs Esum");
    c15->SetWindowPosition(100, 250);
    g_e3_esum_set.hist = new TH2D("g_e3_esum", "E_{3} vs E_{sum};E_{sum};E_{3}", 1000, 1, 4500, 1000, 1, 8000);
    g_e3_esum_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e3_esum_set.hist, "E_{3}", "E_{sum}");
    niasRose.Draw("(0.25*(E1_HG+E2_HG+E3_HG+E4_HG)):(E3_HG) >> g_e3_esum", "", "COLZ");
    c15->AddExec("exec10", "UpdateCutInfo_g_e3_esum()");
    g_e3_esum_set.legend->Draw();

/*--------------------------------------------------------------------*/

    TCanvas *c16 = new TCanvas("c16", "E4 vs Esum");
    c16->SetWindowPosition(200, 300);
    g_e4_esum_set.hist = new TH2D("g_e4_esum", "E_{4} vs E_{sum};E_{sum};E_{4}", 1000, 1, 4500, 1000, 1, 8000);
    g_e4_esum_set.legend = CreateStandardLegend();
    ApplyHistogramStyle(g_e4_esum_set.hist, "E_{4}", "E_{sum}");
    niasRose.Draw("(0.25*(E1_HG+E2_HG+E3_HG+E4_HG)):(E4_HG) >> g_e4_esum", "", "COLZ");
    c16->AddExec("exec11", "UpdateCutInfo_g_e4_esum()");
    g_e4_esum_set.legend->Draw();

}

/*

How to run:

1. root 'hira_data_analysis.C("filename")'

or

2. go inside the code change filename to desired filename

*/
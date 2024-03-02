#include "TFile.h"
#include "TProfile.h"
#include "TKey.h"
#include "TClass.h"
#include "TROOT.h"
#include <iostream>
#include <vector>
#include <memory>
#include "TColor.h"
#include "TString.h"
#include "TH1D.h"
#include "TCanvas.h"

using namespace std;


TH1D* CalcDeltaObv(const TProfile* p_SS , const TProfile* p_OS , TH1D* h) {
  unique_ptr<TH1D> h_SS((TH1D*)p_SS->ProjectionX());
  unique_ptr<TH1D> h_OS((TH1D*)p_OS->ProjectionX());
  TString name = p_SS->GetName();
  name.ReplaceAll("SS" , "");
  h = new TH1D(name , name , h_SS->GetNbinsX() , h_SS->GetXaxis()->GetXmin() , h_SS->GetXaxis()->GetXmax());
  h->Add(h_SS.get() , h_OS.get() , -1 , 1);
  return h;
}


void Draw() {
  gStyle->SetErrorX(0);
  gStyle->SetPadLeftMargin(0.15);
  int ci[6];
  TColor* color[6];
  ci[0] = TColor::GetFreeColorIndex();
  color[0] = new TColor(ci[0],  240/255.,  102/255.,   70/255.);//红
  ci[1] = TColor::GetFreeColorIndex();
  color[1] = new TColor(ci[1],   79/255.,  194/255.,  216/255.);//蓝
  ci[2] = TColor::GetFreeColorIndex();
  color[2] = new TColor(ci[2],  254/255.,  198/255.,  101/255.);//黄
  ci[3] = TColor::GetFreeColorIndex();
  color[3] = new TColor(ci[3],  146/255.,  100/255.,  140/255.);//紫
  ci[4] = TColor::GetFreeColorIndex();
  color[4] = new TColor(ci[4],  125/255.,  200/255.,  165/255.);//绿
  ci[5] = TColor::GetFreeColorIndex();
  color[5] = new TColor(ci[5],   64/255.,   64/255.,   64/255.);//黑
  

  TFile* file = TFile::Open("result.root" , "READ");
  vector<TProfile*> vec_deltaSS_etaGap;
  vector<TProfile*> vec_deltaOS_etaGap;
  vector<TProfile*> vec_gammaSS_etaGap;
  vector<TProfile*> vec_gammaOS_etaGap;

  vector<TH1D*> vec_delta_etaGap;
  vector<TH1D*> vec_gamma_etaGap;

  vector<TProfile*> vec_deltaSS_sumPt;
  vector<TProfile*> vec_deltaOS_sumPt;
  vector<TProfile*> vec_gammaSS_sumPt;
  vector<TProfile*> vec_gammaOS_sumPt;

  vector<TH1D*> vec_delta_sumPt;
  vector<TH1D*> vec_gamma_sumPt;


  TProfile* p_deltaSS = (TProfile*)file->Get("p_deltaSS");
  TProfile* p_deltaOS = (TProfile*)file->Get("p_deltaOS");
  TProfile* p_gammaSS = (TProfile*)file->Get("p_gammaSS");
  TProfile* p_gammaOS = (TProfile*)file->Get("p_gammaOS");

  TH1D* h_delta = nullptr;
  TH1D* h_gamma = nullptr;

  h_delta = CalcDeltaObv(p_deltaSS , p_deltaOS , h_delta);
  h_gamma = CalcDeltaObv(p_gammaSS , p_gammaOS , h_gamma);

  h_delta->SetLineColor(ci[5]);
  h_delta->SetLineStyle(1);
  h_delta->SetLineWidth(2);
  h_delta->SetMarkerColor(ci[5]);
  h_delta->SetMarkerStyle(20);
  h_delta->SetMarkerSize(1.0);
  h_delta->SetStats(0);

  h_gamma->SetLineColor(ci[5]);
  h_gamma->SetLineStyle(1);
  h_gamma->SetLineWidth(2);
  h_gamma->SetMarkerColor(ci[5]);
  h_gamma->SetMarkerStyle(20);
  h_gamma->SetMarkerSize(1.0);
  h_gamma->SetStats(0);

  TIter next(file->GetListOfKeys());
  TKey* key;
  // 遍历文件中的所有键（Key）
  while ((key = (TKey*)next())) {
    // 获取对象的类名
    TClass* cl = gROOT->GetClass(key->GetClassName());
    if (!cl->InheritsFrom("TProfile")) continue; // 如果不是TProfile，跳过
    TProfile* profile = (TProfile*)key->ReadObj();

    TString name = key->GetName();
    //如果名字中存在"etaGap"
    if (name.Contains("etaGap")) {
      if (name.Contains("deltaSS")) {
        vec_deltaSS_etaGap.emplace_back(move(profile));
      }
      else if (name.Contains("deltaOS")) {
        vec_deltaOS_etaGap.emplace_back(move(profile));
      }
      else if (name.Contains("gammaSS")) {
        vec_gammaSS_etaGap.emplace_back(move(profile));
      }
      else if (name.Contains("gammaOS")) {
        vec_gammaOS_etaGap.emplace_back(move(profile));
      }
    }
    if (name.Contains("sumPt")) {
      if (name.Contains("deltaSS")) {
        vec_deltaSS_sumPt.emplace_back(move(profile));
      }
      else if (name.Contains("deltaOS")) {
        vec_deltaOS_sumPt.emplace_back(move(profile));
      }
      else if (name.Contains("gammaSS")) {
        vec_gammaSS_sumPt.emplace_back(move(profile));
      }
      else if (name.Contains("gammaOS")) {
        vec_gammaOS_sumPt.emplace_back(move(profile));
      }
    }
  }


  for (int i = 0; i < vec_deltaSS_etaGap.size(); i++) {
    vec_delta_etaGap.emplace_back(move(CalcDeltaObv(vec_deltaSS_etaGap[i] , vec_deltaOS_etaGap[i] , vec_delta_etaGap[i])));
    vec_gamma_etaGap.emplace_back(move(CalcDeltaObv(vec_gammaSS_etaGap[i] , vec_gammaOS_etaGap[i] , vec_gamma_etaGap[i])));
  }

  for (int i = 0; i < vec_deltaSS_sumPt.size(); i++) {
    vec_delta_sumPt.emplace_back(move(CalcDeltaObv(vec_deltaSS_sumPt[i] , vec_deltaOS_sumPt[i] , vec_delta_sumPt[i])));
    vec_gamma_sumPt.emplace_back(move(CalcDeltaObv(vec_gammaSS_sumPt[i] , vec_gammaOS_sumPt[i] , vec_gamma_sumPt[i])));
  }

  for (int i = 0; i < vec_delta_etaGap.size(); i++) {
    vec_delta_etaGap[i]->SetLineColor(ci[i]);
    vec_delta_etaGap[i]->SetLineStyle(1);
    vec_delta_etaGap[i]->SetLineWidth(2);
    vec_delta_etaGap[i]->SetMarkerColor(ci[i]);
    vec_delta_etaGap[i]->SetMarkerStyle(20);
    vec_delta_etaGap[i]->SetMarkerSize(1.0);
    vec_delta_etaGap[i]->SetStats(0);
    
    vec_gamma_etaGap[i]->SetLineColor(ci[i]);
    vec_gamma_etaGap[i]->SetLineStyle(1);
    vec_gamma_etaGap[i]->SetLineWidth(2);
    vec_gamma_etaGap[i]->SetMarkerColor(ci[i]);
    vec_gamma_etaGap[i]->SetMarkerStyle(20);
    vec_gamma_etaGap[i]->SetMarkerSize(1.0);
    vec_gamma_etaGap[i]->SetStats(0);
  }

  for (int i = 0; i < vec_delta_sumPt.size(); i++) {
    vec_delta_sumPt[i]->SetLineColor(ci[i]);
    vec_delta_sumPt[i]->SetLineStyle(1);
    vec_delta_sumPt[i]->SetLineWidth(2);
    vec_delta_sumPt[i]->SetMarkerColor(ci[i]);
    vec_delta_sumPt[i]->SetMarkerStyle(20);
    vec_delta_sumPt[i]->SetMarkerSize(1.0);
    vec_delta_sumPt[i]->SetStats(0);
    
    vec_gamma_sumPt[i]->SetLineColor(ci[i]);
    vec_gamma_sumPt[i]->SetLineStyle(1);
    vec_gamma_sumPt[i]->SetLineWidth(2);
    vec_gamma_sumPt[i]->SetMarkerColor(ci[i]);
    vec_gamma_sumPt[i]->SetMarkerStyle(20);
    vec_gamma_sumPt[i]->SetMarkerSize(1.0);
    vec_gamma_sumPt[i]->SetStats(0);
  }
  

  //TLegend* leg_etaGap = new TLegend(0.7 , 0.7 , 0.9 , 0.9);
  //左上角
  TLegend* leg_etaGap = new TLegend(0.2 , 0.5 , 0.5 , 0.85);
  leg_etaGap->SetBorderSize(0);
  leg_etaGap->AddEntry(h_delta , "No #eta gap" , "l");
  leg_etaGap->AddEntry(vec_delta_etaGap[0] , "|#eta_{#Lambda} - #eta_{p}| > 0.2" , "l");
  leg_etaGap->AddEntry(vec_delta_etaGap[1] , "|#eta_{#Lambda} - #eta_{p}| > 0.4" , "l");
  leg_etaGap->AddEntry(vec_delta_etaGap[2] , "|#eta_{#Lambda} - #eta_{p}| > 0.6" , "l");
  leg_etaGap->AddEntry(vec_delta_etaGap[3] , "|#eta_{#Lambda} - #eta_{p}| > 0.8" , "l");

  TLegend* leg_sumPt = new TLegend(0.2 , 0.5 , 0.5 , 0.85);
  leg_sumPt->SetBorderSize(0);
  leg_sumPt->AddEntry(h_delta , "All" , "l");
  leg_sumPt->AddEntry(vec_delta_sumPt[0] , "1 < p_{T}^{#Lambda} + p_{T}^{p} < 3 GeV/c" , "l");
  leg_sumPt->AddEntry(vec_delta_sumPt[1] , "3 < p_{T}^{#Lambda} + p_{T}^{p} < 5 GeV/c" , "l");
  leg_sumPt->AddEntry(vec_delta_sumPt[2] , "5 < p_{T}^{#Lambda} + p_{T}^{p} < 8 GeV/c" , "l");


  TCanvas* c1 = new TCanvas("c1" , "c1" , 1000 , 400);
  c1->Divide(2 , 1);
  c1->cd(1)->DrawFrame(20, -0.002, 70, 0.022)->SetTitle("Blast Wave wit LBC;centrality(%);#Delta#delta");
  for (auto h : vec_delta_etaGap) h->Draw("same LPF");
  for (auto h : vec_delta_etaGap) h->Draw("same HIST LF2");
  h_delta->Draw("same LPF");
  h_delta->Draw("same HIST LF2");
  leg_etaGap->Draw();
  c1->cd(2)->DrawFrame(20, -0.001, 70, 0.008)->SetTitle("Blast Wave wit LBC;centrality(%);#Delta#gamma");
  for (auto h : vec_gamma_etaGap) h->Draw("same LPF");
  for (auto h : vec_gamma_etaGap) h->Draw("same HIST LF2");
  h_gamma->Draw("same LPF");
  h_gamma->Draw("same HIST LF2");

  TCanvas* c2 = new TCanvas("c2" , "c2" , 1000 , 400);
  c2->Divide(2 , 1);
  c2->cd(1)->DrawFrame(20, -0.002, 70, 0.04) ->SetTitle("Blast Wave wit LBC;centrality(%);#Delta#delta");
  for (auto h : vec_delta_sumPt) h->Draw("same LPF");
  for (auto h : vec_delta_sumPt) h->Draw("same HIST LF2");
  h_delta->Draw("same LPF");
  h_delta->Draw("same HIST LF2");
  leg_sumPt->Draw();
  c2->cd(2)->DrawFrame(20, -0.001, 70, 0.02) ->SetTitle("Blast Wave wit LBC;centrality(%);#Delta#gamma");
  for (auto h : vec_gamma_sumPt) h->Draw("same LPF");
  for (auto h : vec_gamma_sumPt) h->Draw("same HIST LF2");
  h_gamma->Draw("same LPF");
  h_gamma->Draw("same HIST LF2");

  c1->SaveAs("c1.pdf");
  c2->SaveAs("c2.pdf");
}





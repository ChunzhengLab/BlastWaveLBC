#include <iostream>
#include <vector>
#include <fstream> // Include the necessary header files

#include "TF1.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TFile.h"
#include "TGraph.h"
#include "TSpline.h"
#include "TStopwatch.h"

#include "Particle.h"
#include "Event.h"
#include "par.h"


std::array<TF1*, 9> nbdFunctions;
void InitializeNBDFunctions() {
    for (int i = 0; i < (int)nbdFunctions.size(); ++i) {
        if (!nbdFunctions[i]) {
            nbdFunctions[i] = new TF1(Form("NBD%i", i), nameNBD, NBDLow[i], NBDHigh[i]);
        }
    }
}

unsigned int GetMultNBD(float centrality) {
  unsigned int multiplicity = 0;
  unsigned int centBin = centrality / 10;
  // 查看是否已经初始化了NBD函数，如果没有则初始化
  if (!nbdFunctions[centBin]) InitializeNBDFunctions();
  // Multiplicity determination
  float gMultPre = mu5TeV[centBin] / KinCutRatio[centBin];   
  float N_Mean  = gMultPre; //  Keep multiplicity to be around mu5TeV[centBin] in different fLCC
  float N_Sigma = N_Mean * 0.1;
  float N_Var = N_Sigma * N_Sigma;
  float pNBD = N_Mean / N_Var;
  float rNBD = N_Mean * N_Mean / (N_Var - N_Mean);
  if(isDebug) {
    cout<<"NBD parameters: "<<endl;
    cout<<"N_Mean = "<<N_Mean<<endl;
    cout<<"N_Var = "<<N_Sigma<<endl;
    cout<<"pNBD = "<<pNBD<<endl;
    cout<<"rNBD = "<<rNBD<<endl;
  }
  nbdFunctions[centBin] -> SetParameter(0, rNBD);
  nbdFunctions[centBin] -> SetParameter(1, pNBD);
  multiplicity = nbdFunctions[centBin] ->GetRandom();
  //我们只生成Lambda和Proton，大概是总Multiplicity的8%
  multiplicity = multiplicity * ratioProtonInclusive * (1 + ratioProtonLambda) / ratioProtonLambda;
  if(isDebug) cout<<"multiplicity = "<<multiplicity<<endl;
  if(isDebug) multiplicity = 100;
  return multiplicity;
}

int main(int argc, char* argv[]) {
// int main(int argc, char* argv[]) {
  TStopwatch timer;
  timer.Start();
  ///////////////////////////////////////////////////////////////////////////////////////////
  //读取参数 fTScale, fBetaScale, fRho2Scale, fLBC, fCentrality
  // if (argc < 5) {
  //   cout << "Usage: ./main fTScale fBetaScale fRho2Scale fLBC fCentrality" << endl;
  //   return 1;
  // }
  // 将字符串参数转换为浮点数
  // const float fTScale = std::stof(argv[1]);
  // const float fBetaScale = std::stof(argv[2]);
  // const float fRho2Scale = std::stof(argv[3]);
  // const float fLBC = std::stof(argv[4]);
  // const float fCentrality = std::stof(argv[5]);
  ///////////////////////////////////////////////////////////////////////////////////////////

  const float fCentrality = std::stof(argv[1]);
  const float fTScale = TScale;
  const float fBetaScale = BetaScale;
  //fRho2Scale 在Rho2Scale 的 0.5 ~ 1.5 之间晃动
  float fRho2Scale = Rho2Scale * (0.5 + 0.5 * gRandom->Rndm());
  float fLBC = 0.0;
  int cent_ = (int) fCentrality;
  //cent_ = 25 -> fLBC = FracLBC[0], cent_ = 35 -> fLBC = FracLBC[1], cent_ = 45 -> fLBC = FracLBC[2], 
  if (cent_ == 25)      fLBC = FracLBC[0];
  else if (cent_ == 35) fLBC = FracLBC[1];
  else if (cent_ == 45) fLBC = FracLBC[2];
  else if (cent_ == 55) fLBC = FracLBC[3];
  else if (cent_ == 65) fLBC = FracLBC[4];

  cout<<"Start a new run"<<endl;
  // 初始化NBD函数
  InitializeNBDFunctions();

  TH1D* h_pt_lambda = new TH1D("h_pt_lambda", "h_pt_lambda", nPtBins, PtHistMin, PtHistMax);
  TH1D* h_pt_proton = new TH1D("h_pt_proton", "h_pt_proton", nPtBins, PtHistMin, PtHistMax);
  TProfile* p_v2_pt_lambda = new TProfile("p_v2_pt_lambda", "p_v2_pt_lambda", nV2Bins, V2HistMin, V2HistMax);
  TProfile* p_v2_pt_proton = new TProfile("p_v2_pt_proton", "p_v2_pt_proton", nV2Bins, V2HistMin, V2HistMax);
  TProfile* p_deltaOS = new TProfile("p_deltaOS", "p_deltaOS", 7, 0, 70);
  TProfile* p_deltaSS = new TProfile("p_deltaSS", "p_deltaSS", 7, 0, 70);
  TProfile* p_gammaOS = new TProfile("p_gammaOS", "p_gammaOS", 7, 0, 70);
  TProfile* p_gammaSS = new TProfile("p_gammaSS", "p_gammaSS", 7, 0, 70);
  TProfile* p_deltaOS_etaGap[4];
  TProfile* p_deltaSS_etaGap[4];
  TProfile* p_gammaOS_etaGap[4];
  TProfile* p_gammaSS_etaGap[4];
  TProfile* p_deltaOS_sumPt[3];
  TProfile* p_deltaSS_sumPt[3];
  TProfile* p_gammaOS_sumPt[3];
  TProfile* p_gammaSS_sumPt[3];
  float etaGap[4] = {0.2, 0.4, 0.6, 0.8};
  float sumPtMin[3] = {1, 3, 5};
  float sumPtMax[3] = {3, 5, 8};
  for (int i = 0; i < 4; i++) {
    p_deltaOS_etaGap[i] = new TProfile(Form("p_deltaOS_etaGap_%f", etaGap[i]), Form("p_deltaOS_etaGap_%f", etaGap[i]), 7, 0, 70);
    p_deltaSS_etaGap[i] = new TProfile(Form("p_deltaSS_etaGap_%f", etaGap[i]), Form("p_deltaSS_etaGap_%f", etaGap[i]), 7, 0, 70);
    p_gammaOS_etaGap[i] = new TProfile(Form("p_gammaOS_etaGap_%f", etaGap[i]), Form("p_gammaOS_etaGap_%f", etaGap[i]), 7, 0, 70);
    p_gammaSS_etaGap[i] = new TProfile(Form("p_gammaSS_etaGap_%f", etaGap[i]), Form("p_gammaSS_etaGap_%f", etaGap[i]), 7, 0, 70);
  }
  for (int i = 0; i < 3; i++) {
    p_deltaOS_sumPt[i] = new TProfile(Form("p_deltaOS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), Form("p_deltaOS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), 7, 0, 70);
    p_deltaSS_sumPt[i] = new TProfile(Form("p_deltaSS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), Form("p_deltaOS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), 7, 0, 70);
    p_gammaOS_sumPt[i] = new TProfile(Form("p_gammaOS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), Form("p_deltaOS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), 7, 0, 70);
    p_gammaSS_sumPt[i] = new TProfile(Form("p_gammaSS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), Form("p_deltaOS_sumPt_%f_%f", sumPtMin[i], sumPtMax[i]), 7, 0, 70);
  }

  TProfile* p_v2_lambda_sumPt = new TProfile("p_v2_sumPt_lambda", "p_v2_sumPt_lambda", 40, 0, 10);
  TProfile* p_v2_proton_sumPt = new TProfile("p_v2_sumPt_proton", "p_v2_sumPt_proton", 40, 0, 10);
  TProfile* p_v2_lambda_etaGap = new TProfile("p_v2_etaGap_lambda", "p_v2_etaGap_lambda", 40, 0., 1.6);
  TProfile* p_v2_proton_etaGap = new TProfile("p_v2_etaGap_proton", "p_v2_etaGap_proton", 40, 0., 1.6);
  TProfile* p_deltaOS_sumPt_thisCent = new TProfile("p_deltaOS_sumPt_thisCent", "p_deltaOS_sumPt_thisCent", 40, 0, 10);
  TProfile* p_deltaSS_sumPt_thisCent = new TProfile("p_deltaSS_sumPt_thisCent", "p_deltaSS_sumPt_thisCent", 40, 0, 10);
  TProfile* p_gammaOS_sumPt_thisCent = new TProfile("p_gammaOS_sumPt_thisCent", "p_gammaOS_sumPt_thisCent", 40, 0, 10);
  TProfile* p_gammaSS_sumPt_thisCent = new TProfile("p_gammaSS_sumPt_thisCent", "p_gammaSS_sumPt_thisCent", 40, 0, 10);
  TProfile* p_deltaOS_etaGap_thisCent = new TProfile("p_deltaOS_etaGap_thisCent", "p_deltaOS_etaGap_thisCent", 40, 0., 1.6);
  TProfile* p_deltaSS_etaGap_thisCent = new TProfile("p_deltaSS_etaGap_thisCent", "p_deltaSS_etaGap_thisCent", 40, 0., 1.6);
  TProfile* p_gammaOS_etaGap_thisCent = new TProfile("p_gammaOS_etaGap_thisCent", "p_gammaOS_etaGap_thisCent", 40, 0., 1.6);
  TProfile* p_gammaSS_etaGap_thisCent = new TProfile("p_gammaSS_etaGap_thisCent", "p_gammaSS_etaGap_thisCent", 40, 0., 1.6);
  TProfile* p_deltaOS_v2_lambda = new TProfile("p_deltaOS_v2_lambda", "p_deltaOS_v2_lambda", 40, 0, 0.4);
  TProfile* p_deltaSS_v2_lambda = new TProfile("p_deltaSS_v2_lambda", "p_deltaSS_v2_lambda", 40, 0, 0.4);
  TProfile* p_gammaOS_v2_lambda = new TProfile("p_gammaOS_v2_lambda", "p_gammaOS_v2_lambda", 40, 0, 0.4);
  TProfile* p_gammaSS_v2_lambda = new TProfile("p_gammaSS_v2_lambda", "p_gammaSS_v2_lambda", 40, 0, 0.4);
  TProfile* p_deltaOS_v2_proton = new TProfile("p_deltaOS_v2_proton", "p_deltaOS_v2_proton", 40, 0, 0.4);
  TProfile* p_deltaSS_v2_proton = new TProfile("p_deltaSS_v2_proton", "p_deltaSS_v2_proton", 40, 0, 0.4);
  TProfile* p_gammaOS_v2_proton = new TProfile("p_gammaOS_v2_proton", "p_gammaOS_v2_proton", 40, 0, 0.4);
  TProfile* p_gammaSS_v2_proton = new TProfile("p_gammaSS_v2_proton", "p_gammaSS_v2_proton", 40, 0, 0.4);

  if (isDebug) cout<<"Start generating events..."<<endl;
  for (unsigned int iEvent = 0; iEvent < nEvents; iEvent++) {
    //显示生成进度，每100个事件显示一次
    if (iEvent % 100 == 0) {
      cout << "Generating event " << iEvent << "..." << endl;
    }
    Event* event = new Event(fTScale, fBetaScale, fRho2Scale, fCentrality);
    unsigned int multiplicity = GetMultNBD(fCentrality);
    event->SetMultiplicity(multiplicity);
    event->SetfLBC(fLBC);
    if (isDebug) {
      cout<< "check event "<<iEvent<<" :"<<endl;
      event->Print();
    }
    if (isDebug) cout<<"start generating particles in this event"<<endl;
    event->GenerateParticles();
    if (isDebug) cout<<"particles generated"<<endl;

    event->CaluculateObservables();
    h_pt_lambda->Add(event->GetPtLambdaThisEvt());
    h_pt_proton->Add(event->GetPtProtonThisEvt());
    p_v2_pt_lambda->Add(event->GetV2PtLambdaThisEvt());
    p_v2_pt_proton->Add(event->GetV2PtProtonThisEvt());

    p_gammaSS->Add(event->GetGammaSSThisEvt());
    p_gammaOS->Add(event->GetGammaOSThisEvt());
    p_deltaSS->Add(event->GetDeltaSSThisEvt());
    p_deltaOS->Add(event->GetDeltaOSThisEvt());

    for (int i = 0; i < 4; i++) {
      p_gammaSS_etaGap[i]->Add(event->GetGammaSSThisEvtEtaGap()[i]);
      p_gammaOS_etaGap[i]->Add(event->GetGammaOSThisEvtEtaGap()[i]);
      p_deltaSS_etaGap[i]->Add(event->GetDeltaSSThisEvtEtaGap()[i]);
      p_deltaOS_etaGap[i]->Add(event->GetDeltaOSThisEvtEtaGap()[i]);
    }

    for (int i = 0; i < 3; i++) {
      p_gammaSS_sumPt[i]->Add(event->GetGammaSSThisEvtSumPt()[i]);
      p_gammaOS_sumPt[i]->Add(event->GetGammaOSThisEvtSumPt()[i]);
      p_deltaSS_sumPt[i]->Add(event->GetDeltaSSThisEvtSumPt()[i]);
      p_deltaOS_sumPt[i]->Add(event->GetDeltaOSThisEvtSumPt()[i]);
    }

    p_v2_lambda_sumPt -> Add(event->GetV2LambdaSumPt());
    p_v2_proton_sumPt ->Add(event->GetV2ProtonSumPt());
    p_v2_lambda_etaGap ->Add(event->GetV2LambdaEtaGap());
    p_v2_proton_etaGap ->Add(event->GetV2ProtonEtaGap());
    p_deltaOS_sumPt_thisCent ->Add(event->GetDeltaOSSumPtThisCent());
    p_deltaSS_sumPt_thisCent ->Add(event->GetDeltaSSSumPtThisCent());
    p_gammaOS_sumPt_thisCent ->Add(event->GetGammaOSSumPtThisCent());
    p_gammaSS_sumPt_thisCent ->Add(event->GetGammaSSSumPtThisCent());
    p_deltaOS_etaGap_thisCent ->Add(event->GetDeltaOSEtaGapThisCent());
    p_deltaSS_etaGap_thisCent ->Add(event->GetDeltaSSEtaGapThisCent());
    p_gammaOS_etaGap_thisCent ->Add(event->GetGammaOSEtaGapThisCent());
    p_gammaSS_etaGap_thisCent ->Add(event->GetGammaSSEtaGapThisCent());
    p_deltaOS_v2_lambda ->Add(event->GetDeltaOSV2Lambda());
    p_deltaSS_v2_lambda ->Add(event->GetDeltaSSV2Lambda());
    p_gammaOS_v2_lambda ->Add(event->GetGammaOSV2Lambda());
    p_gammaSS_v2_lambda ->Add(event->GetGammaSSV2Lambda());
    p_deltaOS_v2_proton ->Add(event->GetDeltaOSV2Proton());
    p_deltaSS_v2_proton ->Add(event->GetDeltaSSV2Proton());
    p_gammaOS_v2_proton ->Add(event->GetGammaOSV2Proton());
    p_gammaSS_v2_proton ->Add(event->GetGammaSSV2Proton());

    delete event;
    event = nullptr;
  }

  TFile* f = new TFile(Form("result_cent_%i.root", cent_), "RECREATE");
  h_pt_lambda->Write();
  h_pt_proton->Write();
  p_v2_pt_lambda->Write();
  p_v2_pt_proton->Write();
  p_gammaSS->Write();
  p_gammaOS->Write();
  p_deltaSS->Write();
  p_deltaOS->Write();
  for (int i = 0; i < 4; i++) {
    p_gammaSS_etaGap[i]->Write();
    p_gammaOS_etaGap[i]->Write();
    p_deltaSS_etaGap[i]->Write();
    p_deltaOS_etaGap[i]->Write();
  }
  for (int i = 0; i < 3; i++) {
    p_gammaSS_sumPt[i]->Write();
    p_gammaOS_sumPt[i]->Write();
    p_deltaSS_sumPt[i]->Write();
    p_deltaOS_sumPt[i]->Write();
  }
  p_v2_lambda_sumPt -> Write();
  p_v2_proton_sumPt -> Write();
  p_v2_lambda_etaGap -> Write();
  p_v2_proton_etaGap -> Write();
  p_deltaOS_sumPt_thisCent -> Write();
  p_deltaSS_sumPt_thisCent -> Write();
  p_gammaOS_sumPt_thisCent -> Write();
  p_gammaSS_sumPt_thisCent -> Write();
  p_deltaOS_etaGap_thisCent -> Write();
  p_deltaSS_etaGap_thisCent -> Write();
  p_gammaOS_etaGap_thisCent -> Write();
  p_gammaSS_etaGap_thisCent -> Write();
  p_deltaOS_v2_lambda -> Write();
  p_deltaSS_v2_lambda -> Write();
  p_gammaOS_v2_lambda -> Write();
  p_gammaSS_v2_lambda -> Write();
  p_deltaOS_v2_proton -> Write();
  p_deltaSS_v2_proton -> Write();
  p_gammaOS_v2_proton -> Write();
  p_gammaSS_v2_proton -> Write();

  f->Close();
  //cout << "Generating event TScale = " << fTScale << ", BetaScale = " << fBetaScale << ", Rho2Scale = " << fRho2Scale << ", Centrality = " << fCentrality << " finished." << endl;
  //cout << "Generating event fLBC = " << fLBC << ", Centrality = " << fCentrality << " finished." << endl;
  cout << "Generating event Centrality = " << fCentrality << " finished." << endl;
  timer.Stop();
  cout<< "the total time is "<<timer.RealTime()<<endl;
  return 0;
}


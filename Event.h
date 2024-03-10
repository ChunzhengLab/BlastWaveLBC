#pragma once
#include <vector>
#include <array>

#include "TF1.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "TProfile.h"

#include "Particle.h"
//#include "par.h"

using namespace std;

class Event
{
private:
  float fTScale;
  float fBetaScale;
  float fRho2Scale;
  float fCentrality;

  unsigned int multiplicity;
  float fRejectionFactor;

  bool isV2Calculated;
  float v2_thisEvt;
  float v2_lambda_thisEvt;
  float v2_proton_thisEvt;
  unsigned int nLambda;
  unsigned int nProton;

  //用于生成Particle in this Event
  void SeedEmissionPoint(float& x , float& y);
  TVector3 GetBoostVector(const TVector2& xy);
  int GivePidBasedOnRatio(float ratio);

  //存储Particle的vector
  vector<unique_ptr<Particle>> vecParticles;
  void Reserve(size_t size) { vecParticles.reserve(size); }
  void AddParticle(unique_ptr<Particle> p) { vecParticles.emplace_back(std::move(p)); }
  void CaluculateV2ThisEvt();

  //用于存储每个Event的观测量
  TH1D* h_pt_lambda_thisEvt;
  TH1D* h_pt_proton_thisEvt;
  TProfile* p_v2_pt_lambda_thisEvt;
  TProfile* p_v2_pt_proton_thisEvt;
  TProfile* p_gammaSS_thisEvt;
  TProfile* p_gammaOS_thisEvt;
  TProfile* p_deltaSS_thisEvt;
  TProfile* p_deltaOS_thisEvt;
  array<TProfile* , 4> arr_p_deltaOS_etaGap_thisEvt;
  array<TProfile* , 4> arr_p_deltaSS_etaGap_thisEvt;
  array<TProfile* , 4> arr_p_gammaOS_etaGap_thisEvt;
  array<TProfile* , 4> arr_p_gammaSS_etaGap_thisEvt;

  array<TProfile* , 3> arr_p_deltaOS_sumPt_thisEvt;
  array<TProfile* , 3> arr_p_deltaSS_sumPt_thisEvt;
  array<TProfile* , 3> arr_p_gammaOS_sumPt_thisEvt;
  array<TProfile* , 3> arr_p_gammaSS_sumPt_thisEvt;

  // TProfile* p_gammaSS_prpr_thisEvt;
  // TProfile* p_gammaOS_prpr_thisEvt;
  // TProfile* p_deltaOS_prpr_thisEvt;
  // TProfile* p_deltaSS_prpr_thisEvt;

  // TProfile* p_gammaSS_lala_thisEvt;
  // TProfile* p_gammaOS_lala_thisEvt;
  // TProfile* p_deltaOS_lala_thisEvt;
  // TProfile* p_deltaSS_lala_thisEvt;

  TProfile* p_v2_lambda_sumPt_thisEvt;
  TProfile* p_v2_proton_sumPt_thisEvt;
  TProfile* p_v2_lambda_etaGap_thisEvt;
  TProfile* p_v2_proton_etaGap_thisEvt;
  TProfile* p_deltaOS_sumPt_thisCent_thisEvt;
  TProfile* p_deltaSS_sumPt_thisCent_thisEvt;
  TProfile* p_gammaOS_sumPt_thisCent_thisEvt;
  TProfile* p_gammaSS_sumPt_thisCent_thisEvt;
  TProfile* p_deltaOS_etaGap_thisCent_thisEvt;
  TProfile* p_deltaSS_etaGap_thisCent_thisEvt;
  TProfile* p_gammaOS_etaGap_thisCent_thisEvt;
  TProfile* p_gammaSS_etaGap_thisCent_thisEvt;
  TProfile* p_deltaOS_v2_lambda_thisEvt;
  TProfile* p_deltaSS_v2_lambda_thisEvt;
  TProfile* p_gammaOS_v2_lambda_thisEvt;
  TProfile* p_gammaSS_v2_lambda_thisEvt;
  TProfile* p_deltaOS_v2_proton_thisEvt;
  TProfile* p_deltaSS_v2_proton_thisEvt;
  TProfile* p_gammaOS_v2_proton_thisEvt;
  TProfile* p_gammaSS_v2_proton_thisEvt;

public:
  Event(float TScale , float BetaScale , float Rho2Scale , float Centrality);
  ~Event();

  void SetMultiplicity(unsigned int multiplicity) { this->multiplicity = multiplicity; }
  void SetfLBC(float flbc) { fRejectionFactor = flbc * (2. - flbc); }

  bool GenerateParticles();
  void CaluculateObservables();
  void Print();

  void GetV2ThisEvt(float& v2, float& v2_lambda, float& v2_proton) { //should be very slow
    if(!isV2Calculated) CaluculateV2ThisEvt();
    v2 = v2_thisEvt; 
    v2_lambda = v2_lambda_thisEvt; 
    v2_proton = v2_proton_thisEvt; 
  }

  TH1D* GetPtLambdaThisEvt() { return h_pt_lambda_thisEvt; }
  TH1D* GetPtProtonThisEvt() { return h_pt_proton_thisEvt; }
  TProfile* GetV2PtLambdaThisEvt() { return p_v2_pt_lambda_thisEvt; }
  TProfile* GetV2PtProtonThisEvt() { return p_v2_pt_proton_thisEvt; }
  TProfile* GetGammaSSThisEvt() { return p_gammaSS_thisEvt; }
  TProfile* GetGammaOSThisEvt() { return p_gammaOS_thisEvt; }
  TProfile* GetDeltaSSThisEvt() { return p_deltaSS_thisEvt; }
  TProfile* GetDeltaOSThisEvt() { return p_deltaOS_thisEvt; }

  array<TProfile* , 4> GetDeltaOSThisEvtEtaGap() { return arr_p_deltaOS_etaGap_thisEvt; }
  array<TProfile* , 4> GetDeltaSSThisEvtEtaGap() { return arr_p_deltaSS_etaGap_thisEvt; }
  array<TProfile* , 4> GetGammaOSThisEvtEtaGap() { return arr_p_gammaOS_etaGap_thisEvt; }
  array<TProfile* , 4> GetGammaSSThisEvtEtaGap() { return arr_p_gammaSS_etaGap_thisEvt; }

  array<TProfile* , 3> GetDeltaOSThisEvtSumPt() { return arr_p_deltaOS_sumPt_thisEvt; }
  array<TProfile* , 3> GetDeltaSSThisEvtSumPt() { return arr_p_deltaSS_sumPt_thisEvt; }
  array<TProfile* , 3> GetGammaOSThisEvtSumPt() { return arr_p_gammaOS_sumPt_thisEvt; }
  array<TProfile* , 3> GetGammaSSThisEvtSumPt() { return arr_p_gammaSS_sumPt_thisEvt; }

  TProfile* GetV2LambdaSumPt() { return p_v2_lambda_sumPt_thisEvt; }
  TProfile* GetV2ProtonSumPt() { return p_v2_proton_sumPt_thisEvt; }
  TProfile* GetV2LambdaEtaGap() { return p_v2_lambda_etaGap_thisEvt; }
  TProfile* GetV2ProtonEtaGap() { return p_v2_proton_etaGap_thisEvt; }
  TProfile* GetDeltaOSSumPtThisCent() { return p_deltaOS_sumPt_thisCent_thisEvt; }
  TProfile* GetDeltaSSSumPtThisCent() { return p_deltaSS_sumPt_thisCent_thisEvt; }
  TProfile* GetGammaOSSumPtThisCent() { return p_gammaOS_sumPt_thisCent_thisEvt; }
  TProfile* GetGammaSSSumPtThisCent() { return p_gammaSS_sumPt_thisCent_thisEvt; }
  TProfile* GetDeltaOSEtaGapThisCent() { return p_deltaOS_etaGap_thisCent_thisEvt; }
  TProfile* GetDeltaSSEtaGapThisCent() { return p_deltaSS_etaGap_thisCent_thisEvt; }
  TProfile* GetGammaOSEtaGapThisCent() { return p_gammaOS_etaGap_thisCent_thisEvt; }
  TProfile* GetGammaSSEtaGapThisCent() { return p_gammaSS_etaGap_thisCent_thisEvt; }
  TProfile* GetDeltaOSV2Lambda() { return p_deltaOS_v2_lambda_thisEvt; }
  TProfile* GetDeltaSSV2Lambda() { return p_deltaSS_v2_lambda_thisEvt; }
  TProfile* GetGammaOSV2Lambda() { return p_gammaOS_v2_lambda_thisEvt; }
  TProfile* GetGammaSSV2Lambda() { return p_gammaSS_v2_lambda_thisEvt; }
  TProfile* GetDeltaOSV2Proton() { return p_deltaOS_v2_proton_thisEvt; }
  TProfile* GetDeltaSSV2Proton() { return p_deltaSS_v2_proton_thisEvt; }
  TProfile* GetGammaOSV2Proton() { return p_gammaOS_v2_proton_thisEvt; }
  TProfile* GetGammaSSV2Proton() { return p_gammaSS_v2_proton_thisEvt; }
};


Event::Event(float TScale , float BetaScale , float Rho2Scale , float Centrality) : fTScale(TScale) , fBetaScale(BetaScale) , fRho2Scale(Rho2Scale) , fCentrality(Centrality)
{
  this->multiplicity = 0;
  this->fRejectionFactor = 0;
  if (isDebug) cout << "Event constructor called" << endl;
}

Event::~Event()
{
  delete h_pt_lambda_thisEvt;
  delete h_pt_proton_thisEvt;
  delete p_v2_pt_lambda_thisEvt;
  delete p_v2_pt_proton_thisEvt;
  delete p_gammaSS_thisEvt;
  delete p_gammaOS_thisEvt;
  delete p_deltaSS_thisEvt;
  delete p_deltaOS_thisEvt;
  for (int i = 0; i < 4; i++) {
    delete arr_p_deltaOS_etaGap_thisEvt[i];
    delete arr_p_deltaSS_etaGap_thisEvt[i];
    delete arr_p_gammaOS_etaGap_thisEvt[i];
    delete arr_p_gammaSS_etaGap_thisEvt[i];
  }
  for (int i = 0; i < 3; i++) {
    delete arr_p_deltaOS_sumPt_thisEvt[i];
    delete arr_p_deltaSS_sumPt_thisEvt[i];
    delete arr_p_gammaOS_sumPt_thisEvt[i];
    delete arr_p_gammaSS_sumPt_thisEvt[i];
  }

  delete p_v2_lambda_sumPt_thisEvt;
  delete p_v2_proton_sumPt_thisEvt;
  delete p_v2_lambda_etaGap_thisEvt;
  delete p_v2_proton_etaGap_thisEvt;
  delete p_deltaOS_sumPt_thisCent_thisEvt;
  delete p_deltaSS_sumPt_thisCent_thisEvt;
  delete p_gammaOS_sumPt_thisCent_thisEvt;
  delete p_gammaSS_sumPt_thisCent_thisEvt;
  delete p_deltaOS_etaGap_thisCent_thisEvt;
  delete p_deltaSS_etaGap_thisCent_thisEvt;
  delete p_gammaOS_etaGap_thisCent_thisEvt;
  delete p_gammaSS_etaGap_thisCent_thisEvt;
  delete p_deltaOS_v2_lambda_thisEvt;
  delete p_deltaSS_v2_lambda_thisEvt;
  delete p_gammaOS_v2_lambda_thisEvt;
  delete p_gammaSS_v2_lambda_thisEvt;
  delete p_deltaOS_v2_proton_thisEvt;
  delete p_deltaSS_v2_proton_thisEvt;
  delete p_gammaOS_v2_proton_thisEvt;
  delete p_gammaSS_v2_proton_thisEvt;

  h_pt_lambda_thisEvt = nullptr;
  h_pt_proton_thisEvt = nullptr;
  p_v2_pt_lambda_thisEvt = nullptr;
  p_v2_pt_proton_thisEvt = nullptr;
  p_gammaSS_thisEvt = nullptr;
  p_gammaOS_thisEvt = nullptr;
  p_deltaSS_thisEvt = nullptr;
  p_deltaOS_thisEvt = nullptr;
  for (int i = 0; i < 4; i++) {
    arr_p_deltaOS_etaGap_thisEvt[i] = nullptr;
    arr_p_deltaSS_etaGap_thisEvt[i] = nullptr;
    arr_p_gammaOS_etaGap_thisEvt[i] = nullptr;
    arr_p_gammaSS_etaGap_thisEvt[i] = nullptr;
  }
  for (int i = 0; i < 3; i++) {
    arr_p_deltaOS_sumPt_thisEvt[i] = nullptr;
    arr_p_deltaSS_sumPt_thisEvt[i] = nullptr;
    arr_p_gammaOS_sumPt_thisEvt[i] = nullptr;
    arr_p_gammaSS_sumPt_thisEvt[i] = nullptr;
  }

  p_v2_lambda_sumPt_thisEvt = nullptr;
  p_v2_proton_sumPt_thisEvt = nullptr;
  p_v2_lambda_etaGap_thisEvt = nullptr;
  p_v2_proton_etaGap_thisEvt = nullptr;
  p_deltaOS_sumPt_thisCent_thisEvt = nullptr;
  p_deltaSS_sumPt_thisCent_thisEvt = nullptr;
  p_gammaOS_sumPt_thisCent_thisEvt = nullptr;
  p_gammaSS_sumPt_thisCent_thisEvt = nullptr;
  p_deltaOS_etaGap_thisCent_thisEvt = nullptr;
  p_deltaSS_etaGap_thisCent_thisEvt = nullptr;
  p_gammaOS_etaGap_thisCent_thisEvt = nullptr;
  p_gammaSS_etaGap_thisCent_thisEvt = nullptr;
  p_deltaOS_v2_lambda_thisEvt = nullptr;
  p_deltaSS_v2_lambda_thisEvt = nullptr;
  p_gammaOS_v2_lambda_thisEvt = nullptr;
  p_gammaSS_v2_lambda_thisEvt = nullptr;
  p_deltaOS_v2_proton_thisEvt = nullptr;
  p_deltaSS_v2_proton_thisEvt = nullptr;
  p_gammaOS_v2_proton_thisEvt = nullptr;
  p_gammaSS_v2_proton_thisEvt = nullptr;
}

bool Event::GenerateParticles()
{
  nLambda = 0, nProton = 0;
  Reserve(multiplicity);
  unsigned int iSerialNumber = 0;
  //当iSerialNumber大于multiplicity时，停止循环
  while (iSerialNumber < multiplicity) {
    //粒子序列号
    iSerialNumber++;
    //随机生成一个(x,y)坐标
    float x = 0. , y = 0.;
    SeedEmissionPoint(x , y);
    TVector3 boost_vector = GetBoostVector(TVector2(x , y));
    int pid = GivePidBasedOnRatio(ratioProtonLambda);

    Particle* particle = new Particle(fTScale , fCentrality);
    particle->SetSerialNumber(iSerialNumber);
    particle->SetPid(pid);
    particle->SetBoostVector(boost_vector);
    if (isDebug) particle->Print();

    //LBC Friend
    if (gRandom->Rndm() < fRejectionFactor) {
      iSerialNumber++;
      int pidLBC = 0;
      //只考虑相反符号的LBC Friend
      if (pid == 2212) pidLBC = -3122;
      else if (pid == -2212) pidLBC = 3122;
      else if (pid == 3122) pidLBC = -2212;
      else if (pid == -3122) pidLBC = 2212;
      else cout << "Error: pid not found" << endl;

      Particle* particleLBC = new Particle(fTScale , fCentrality);
      particleLBC->SetSerialNumber(iSerialNumber);
      particleLBC->SetPid(pidLBC);
      //设置于原来粒子相同的boost vector
      particleLBC->SetBoostVector(boost_vector);
      particleLBC->BuildParticle();
      particleLBC->SetSerialNumberLBCFriend(iSerialNumber - 1);
      AddParticle(unique_ptr<Particle>(particleLBC));
      particle->SetSerialNumberLBCFriend(iSerialNumber);
    }
    particle->BuildParticle();
    if (particle->GetPid() == 3122) nLambda++;
    if (particle->GetPid() == 2212) nProton++;
    AddParticle(unique_ptr<Particle>(particle));
  }
  if (nLambda == 0 || nProton == 0) return false;
  return true;
}

void Event::SeedEmissionPoint(float& x , float& y)
{
  float rads = sqrt(rndm->Rndm());
  float phis = TMath::TwoPi() * rndm->Rndm();
  //预先验证是否在火球内，如果不在就重新生成
  while (!RadSys(rads , phis , RxProton[(int)(fCentrality / 10)]) > 1.) {
    rads = sqrt(rndm->Rndm());
    phis = TMath::TwoPi() * rndm->Rndm();
  }
  x = rads * cos(phis);
  y = rads * sin(phis);
}

int Event::GivePidBasedOnRatio(float ratio)
{
  //按照1.5:1的比例随机抽取pid，2212为质子，3122为Λ
  int pid = 0;
  if (rndm->Rndm() < ratio / (ratio + 1.)) pid = 2212;
  else pid = 3122;
  //随机给予pid一个正符号
  if (rndm->Rndm() < 0.5) pid = -pid;
  return pid;
}

void Event::CaluculateObservables()
{
  if(!isV2Calculated) CaluculateV2ThisEvt();
  float deltaSS_thisEvt = 0;
  float deltaOS_thisEvt = 0;
  float gammaSS_thisEvt = 0;
  float gammaOS_thisEvt = 0;
  int nSSLoop = 0;
  int nOSLoop = 0;

  h_pt_lambda_thisEvt = new TH1D("h_pt_lambda_thisEvt" , "h_pt_lambda_thisEvt" , nPtBins , PtHistMin , PtHistMax);
  h_pt_proton_thisEvt = new TH1D("h_pt_proton_thisEvt" , "h_pt_proton_thisEvt" , nPtBins , PtHistMin , PtHistMax);
  p_v2_pt_lambda_thisEvt = new TProfile("p_v2_pt_lambda_thisEvt" , "p_v2_pt_lambda_thisEvt" , nV2Bins , V2HistMin , V2HistMax);
  p_v2_pt_proton_thisEvt = new TProfile("p_v2_pt_proton_thisEvt" , "p_v2_pt_proton_thisEvt" , nV2Bins , V2HistMin , V2HistMax);

  p_deltaOS_thisEvt = new TProfile("p_deltaOS_thisEvt" , "p_deltaOS_thisEvt" , 7 , 0 , 70);
  p_deltaSS_thisEvt = new TProfile("p_deltaSS_thisEvt" , "p_deltaSS_thisEvt" , 7 , 0 , 70);
  p_gammaOS_thisEvt = new TProfile("p_gammaOS_thisEvt" , "p_gammaOS_thisEvt" , 7 , 0 , 70);
  p_gammaSS_thisEvt = new TProfile("p_gammaSS_thisEvt" , "p_gammaSS_thisEvt" , 7 , 0 , 70);

  for (int i = 0; i < 4; i++) {
    arr_p_deltaOS_etaGap_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
    arr_p_deltaSS_etaGap_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
    arr_p_gammaOS_etaGap_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
    arr_p_gammaSS_etaGap_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
  }
  for (int i = 0; i < 3; i++) {
    arr_p_deltaOS_sumPt_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
    arr_p_deltaSS_sumPt_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
    arr_p_gammaOS_sumPt_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
    arr_p_gammaSS_sumPt_thisEvt[i] = new TProfile("" , "" , 7 , 0 , 70);
  }

  p_v2_lambda_sumPt_thisEvt = new TProfile("", "", 40, 0, 10);
  p_v2_proton_sumPt_thisEvt = new TProfile("", "", 40, 0, 10);
  p_v2_lambda_etaGap_thisEvt = new TProfile("", "", 40, 0., 1.6);
  p_v2_proton_etaGap_thisEvt = new TProfile("", "", 40, 0., 1.6);
  p_deltaOS_sumPt_thisCent_thisEvt = new TProfile("", "", 40, 0, 10);
  p_deltaSS_sumPt_thisCent_thisEvt = new TProfile("", "", 40, 0, 10);
  p_gammaOS_sumPt_thisCent_thisEvt = new TProfile("", "", 40, 0, 10);
  p_gammaSS_sumPt_thisCent_thisEvt = new TProfile("", "", 40, 0, 10);
  p_deltaOS_etaGap_thisCent_thisEvt = new TProfile("", "", 40, 0., 1.6);
  p_deltaSS_etaGap_thisCent_thisEvt = new TProfile("", "", 40, 0., 1.6);
  p_gammaOS_etaGap_thisCent_thisEvt = new TProfile("", "", 40, 0., 1.6);
  p_gammaSS_etaGap_thisCent_thisEvt = new TProfile("", "", 40, 0., 1.6);
  p_deltaOS_v2_lambda_thisEvt = new TProfile("", "", 100, 0, 1);
  p_deltaSS_v2_lambda_thisEvt = new TProfile("", "", 100, 0, 1);
  p_gammaOS_v2_lambda_thisEvt = new TProfile("", "", 100, 0, 1);
  p_gammaSS_v2_lambda_thisEvt = new TProfile("", "", 100, 0, 1);
  p_deltaOS_v2_proton_thisEvt = new TProfile("", "", 100, 0, 1);
  p_deltaSS_v2_proton_thisEvt = new TProfile("", "", 100, 0, 1);
  p_gammaOS_v2_proton_thisEvt = new TProfile("", "", 100, 0, 1);
  p_gammaSS_v2_proton_thisEvt = new TProfile("", "", 100, 0, 1);

  for (const auto& particle_1 : vecParticles) {
    // 舍去Lambda的y大于0.5,pT小于0.5大于10的情况
    if (abs(particle_1->GetPid()) == 3122) {
      if (abs(particle_1->GetMomentum().Rapidity()) > 0.5) continue;
      if (particle_1->GetMomentum().Pt() < 0.5) continue;
      if (particle_1->GetMomentum().Pt() > 10) continue;
    }
    // 舍去质子的eta大于0.8,pT小于0.2大于5的情况
    if (abs(particle_1->GetPid()) == 2212) {
      if (abs(particle_1->GetMomentum().PseudoRapidity()) > 0.8) continue;
      if (particle_1->GetMomentum().Pt() < 0.4) continue;
      if (particle_1->GetMomentum().Pt() > 5) continue;
    }

    float pt_1 = particle_1->GetMomentum().Pt();
    float phi_1 = particle_1->GetMomentum().Phi();
    float eta_1 = particle_1->GetMomentum().PseudoRapidity();
    float v2_1 = cos(2 * phi_1);

    if (abs(particle_1->GetPid()) == 3122) {
      // 谱只取|y|<0.5，pT>0.6的Lambda
      if (particle_1->GetMomentum().Pt() > 0.6) h_pt_lambda_thisEvt->Fill(pt_1);
      p_v2_pt_lambda_thisEvt->Fill(pt_1 , v2_1);
    }
    else if (abs(particle_1->GetPid()) == 2212) {
      // 谱只取|y|<0.5的质子
      if (abs(particle_1->GetMomentum().Rapidity()) < 0.5) h_pt_proton_thisEvt->Fill(pt_1);
      p_v2_pt_proton_thisEvt->Fill(pt_1 , v2_1);
    }

    // gamma and delta
    for (const auto& particle_2 : vecParticles) {
      // 舍去相同的粒子
      if (particle_1->GetSerialNumber() == particle_2->GetSerialNumber()) continue;
      // 仅考虑Lambda和质子之间的关联
      if (abs(particle_1->GetPid()) == abs(particle_2->GetPid())) continue;

      // 舍去Lambda的y大于0.5,pT小于0.5大于10的情况
      if (abs(particle_2->GetPid()) == 3122) {
        if (abs(particle_2->GetMomentum().Rapidity()) > 0.5) continue;
        if (particle_2->GetMomentum().Pt() < 0.5) continue;
        if (particle_2->GetMomentum().Pt() > 10) continue;
      }
      // 舍去质子的eta大于0.8,pT小于0.2大于5的情况
      if (abs(particle_2->GetPid()) == 2212) {
        if (abs(particle_2->GetMomentum().PseudoRapidity()) > 0.8) continue;
        if (particle_2->GetMomentum().Pt() < 0.4) continue;
        if (particle_2->GetMomentum().Pt() > 5) continue;
      }

      float pt_2 = particle_2->GetMomentum().Pt();
      float phi_2 = particle_2->GetMomentum().Phi();
      float eta_2 = particle_2->GetMomentum().PseudoRapidity();

      float gamma = cos(phi_1 + phi_2);
      float delta = cos(phi_1 - phi_2);
      float eta_gap = abs(eta_1 - eta_2);
      float sumPt = pt_1 + pt_2;

      TBits bits(4);
      bits[0] = eta_gap > 0.2;
      bits[1] = eta_gap > 0.4;
      bits[2] = eta_gap > 0.6;
      bits[3] = eta_gap > 0.8;

      TBits bits_sumPt(3);
      bits_sumPt[0] = sumPt > 1. && sumPt < 3.;
      bits_sumPt[1] = sumPt > 3. && sumPt < 5.;
      bits_sumPt[2] = sumPt > 5. && sumPt < 8.;

      //当pid同号时，认为是SS，反之认为是OS
      if (particle_1->GetPid() * particle_2->GetPid() > 0) {
        p_gammaSS_thisEvt->Fill(fCentrality , gamma);
        p_deltaSS_thisEvt->Fill(fCentrality , delta);
        p_deltaSS_etaGap_thisCent_thisEvt->Fill(eta_gap , delta);
        p_gammaSS_etaGap_thisCent_thisEvt->Fill(eta_gap , gamma);
        p_deltaSS_sumPt_thisCent_thisEvt->Fill(sumPt , delta);
        p_gammaSS_sumPt_thisCent_thisEvt->Fill(sumPt , gamma);
        deltaSS_thisEvt += delta;
        gammaSS_thisEvt += gamma;
        nSSLoop++;

        for (int i = 0; i < 4; i++) {
          if (bits[i]) {
            arr_p_gammaSS_etaGap_thisEvt[i]->Fill(fCentrality , gamma);
            arr_p_deltaSS_etaGap_thisEvt[i]->Fill(fCentrality , delta);
          }
        }
        for (int i = 0; i < 3; i++) {
          if (bits_sumPt[i]) {
            arr_p_gammaSS_sumPt_thisEvt[i]->Fill(fCentrality , gamma);
            arr_p_deltaSS_sumPt_thisEvt[i]->Fill(fCentrality , delta);
          }
        }

        if (abs(particle_1->GetPid()) == 3122) {
          p_v2_lambda_sumPt_thisEvt->Fill(sumPt , v2_1);
          p_v2_lambda_etaGap_thisEvt->Fill(eta_gap , v2_1);
        } else if (abs(particle_1->GetPid()) == 2212) {
          p_v2_proton_sumPt_thisEvt->Fill(sumPt , v2_1);
          p_v2_proton_etaGap_thisEvt->Fill(eta_gap , v2_1);
        }
      } else {
        p_gammaOS_thisEvt->Fill(fCentrality , gamma);
        p_deltaOS_thisEvt->Fill(fCentrality , delta);
        p_deltaOS_etaGap_thisCent_thisEvt->Fill(eta_gap , delta);
        p_gammaOS_etaGap_thisCent_thisEvt->Fill(eta_gap , gamma);
        p_deltaOS_sumPt_thisCent_thisEvt->Fill(sumPt , delta);
        p_gammaOS_sumPt_thisCent_thisEvt->Fill(sumPt , gamma);
        deltaOS_thisEvt += delta;
        gammaOS_thisEvt += gamma;
        nOSLoop++;

        for (int i = 0; i < 4; i++) {
          if (bits[i]) {
            arr_p_gammaOS_etaGap_thisEvt[i]->Fill(fCentrality , gamma);
            arr_p_deltaOS_etaGap_thisEvt[i]->Fill(fCentrality , delta);
          }
        }
        for (int i = 0; i < 3; i++) {
          if (bits_sumPt[i]) {
            arr_p_gammaOS_sumPt_thisEvt[i]->Fill(fCentrality , gamma);
            arr_p_deltaOS_sumPt_thisEvt[i]->Fill(fCentrality , delta);
          }
        }

        if (abs(particle_1->GetPid()) == 3122) {
          p_deltaOS_etaGap_thisCent_thisEvt->Fill(eta_gap , delta);
          p_gammaOS_etaGap_thisCent_thisEvt->Fill(eta_gap , gamma);
          p_deltaOS_sumPt_thisCent_thisEvt->Fill(sumPt , delta);
          p_gammaOS_sumPt_thisCent_thisEvt->Fill(sumPt , gamma);
        } else if (abs(particle_1->GetPid()) == 2212) {
          p_deltaOS_etaGap_thisCent_thisEvt->Fill(eta_gap , delta);
          p_gammaOS_etaGap_thisCent_thisEvt->Fill(eta_gap , gamma);
          p_deltaOS_sumPt_thisCent_thisEvt->Fill(sumPt , delta);
          p_gammaOS_sumPt_thisCent_thisEvt->Fill(sumPt , gamma);
        }
      }
    }
  }

  if (nSSLoop > 1e-6 && nOSLoop > 1e-6) {
    p_deltaSS_v2_lambda_thisEvt->Fill(v2_lambda_thisEvt , deltaSS_thisEvt / nSSLoop, nSSLoop);
    p_gammaSS_v2_lambda_thisEvt->Fill(v2_lambda_thisEvt , gammaSS_thisEvt / nSSLoop, nSSLoop);
    p_deltaSS_v2_proton_thisEvt->Fill(v2_proton_thisEvt , deltaSS_thisEvt / nSSLoop, nSSLoop);
    p_gammaSS_v2_proton_thisEvt->Fill(v2_proton_thisEvt , gammaSS_thisEvt / nSSLoop, nSSLoop);
    p_deltaOS_v2_lambda_thisEvt->Fill(v2_lambda_thisEvt , deltaOS_thisEvt / nOSLoop, nOSLoop);
    p_gammaOS_v2_lambda_thisEvt->Fill(v2_lambda_thisEvt , gammaOS_thisEvt / nOSLoop, nOSLoop);
    p_deltaOS_v2_proton_thisEvt->Fill(v2_proton_thisEvt , deltaOS_thisEvt / nOSLoop, nOSLoop);
    p_gammaOS_v2_proton_thisEvt->Fill(v2_proton_thisEvt , gammaOS_thisEvt / nOSLoop, nOSLoop);
  }
}

TVector3 Event::GetBoostVector(const TVector2& xy) {
  float rho0 = atanh(BetaTProton[(int)(fCentrality / 10)] * (1 + fBetaScale));
  float rho2 = Rho2Proton[(int)(fCentrality / 10)] * (1 + fRho2Scale);

  float rads = xy.Mod();
  float phis = xy.Phi();
  float phib = BosPhi(phis , RxProton[(int)(fCentrality / 10)]);
  float rhob = RadSys(rads , phis , RxProton[(int)(fCentrality / 10)]) * (rho0 + rho2 * cos(2. * phib));

  float csthetas = 2. * (rndm->Rndm() - 0.5);
  float thetas = acos(csthetas);
  float etas = -log(tan(thetas / 2.));
  TLorentzVector u;
  u.SetXYZT(sinh(rhob) * cos(phib) , sinh(rhob) * sin(phib) , cosh(rhob) * sinh(etas) , cosh(rhob) * cosh(etas));
  TVector3 boostvector = u.BoostVector();
  return boostvector;
}


void Event::Print() {
  cout << "======================" << endl;
  cout << "Event centrality: " << fCentrality << endl;
  cout << "Event multiplicity: " << multiplicity << endl;
  cout << "LBC rejection factor: " << fRejectionFactor << endl;
  cout << "======================" << endl;
}

void Event::CaluculateV2ThisEvt() {
  for(auto& particle : vecParticles) {
    float v2 = cos(2 * particle->GetMomentum().Phi());
    v2_thisEvt += v2;
    if (abs(particle->GetPid()) == 3122) {
      v2_lambda_thisEvt += v2;
    }
    if (abs(particle->GetPid()) == 2212) {
      v2_proton_thisEvt += v2;
    }
  }

  if (nLambda > 1e-6) v2_lambda_thisEvt /= nLambda;
  else                v2_lambda_thisEvt = 0.;
  if (nProton > 1e-6) v2_proton_thisEvt /= nProton;
  else                v2_proton_thisEvt = 0.;
  if (vecParticles.size() > 1e-6) v2_thisEvt /= vecParticles.size();
  else                            v2_thisEvt = 0.;

  if(isnan(v2_thisEvt) || isnan(v2_lambda_thisEvt) || isnan(v2_proton_thisEvt)
   ||isinf(v2_thisEvt) || isinf(v2_lambda_thisEvt) || isinf(v2_proton_thisEvt)) {
    //异常抛出
    throw std::runtime_error("v2 is nan or inf");
  }

  isV2Calculated = true;
}


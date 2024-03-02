
#pragma once
#include <iostream>
#include "TF1.h"
#include "TRandom3.h"

extern const float MassProton = 0.938272; // GeV
extern const float MassLambda = 1.115683; // GeV
//wenya的参数
// const float Rho2Proton[9] = {0.0538, 0.063, 0.11, 0.135, 0.15, 0.145, 0.121,0.115,0.0}; // Higher rhon2 to reach up to the v2 in Exp
// const float RxProton[9]   = {9.56096, 9.33685, 9.05065, 8.72321, 8.44751, 8.2290, 8.07123, 7.862, 7.000}; // Ry=10.0fm fixed (default value) BW fit.
// const float Rho2Proton[9] = {0.0538, 0.063, 0.11, 0.135, 0.15, 0.145, 0.121,0.115,0.0}; // Higher rhon2 to reach up to the v2 in Exp


namespace par
{
  // 参数列表
  const float TempProton[9] = {0.0905, 0.094, 0.097,0.101,0.108,0.115,0.129,0.147,0.161}; // GeV
  const float BetaTProton[9] = {0.6615, 0.655,0.643,0.622,0.595,0.557,0.506,0.435,0.355}; // BetaTProton = tanh(Rho0Proton)
  const float NProton[9] = {0.7355,0.739,0.771,0.828,0.908,1.052,1.262,1.678,2.423}; //FIXME dont know how to use it
  const float Rho2Proton[9] = {0.0538, 0.063, 0.11, 0.135, 0.15, 0.145, 0.121,0.115,0.0}; // Higher rhon2 to reach up to the v2 in Exp
  const float RxProton[9] = {1,1,1,1,1,1,1,1,1}; // Ry=10.0fm fixed (default value) BW fit.

  const float TempLambda[9] = {0.0905, 0.094, 0.097,0.101,0.108,0.115,0.129,0.147,0.161}; // GeV
  const float BetaTLambda[9] = {0.6615, 0.655,0.643,0.622,0.595,0.557,0.506,0.435,0.355}; // BetaTLambda = tanh(Rho0Lambda)
  const float NLambda[9] = {0.7355,0.739,0.771,0.828,0.908,1.052,1.262,1.678,2.423}; //FIXME dont know how to use it
  const float Rho2Lambda[9] = {0.0538, 0.063, 0.11, 0.135, 0.15, 0.145, 0.121,0.115,0.0}; // Higher rhon2 to reach up to the v2 in Exp
  const float RxLambda[9] = {1,1,1,1,1,1,1,1,1}; // Ry=10.0fm fixed (default value) BW fit.

  // NBD distribution of multiplicity
  const float KinCutRatio[9] = {0.43507719, 0.43209614, 0.431945, 0.428783, 0.4336, 0.428906, 0.427373, 0.423507, 0.423523}; // Ratio = multi(after pt,eta cut) / multi(before pt,eta cut)
  const float mu5TeV[9] = {2205 + 74, 1791 + 61, 1290 + 50, 851 + 50, 583.3 + 20, 359.5 + 10, 222.9, 106.4 + 10, 49.07};     // Read raw multiplicity value from ALICE; Also supported by PRL 106, 032301 (2011)
  const float NBDLow[9] = {1600, 1300, 900, 500, 400, 300, 0, 0, 0};
  const float NBDHigh[9] = {5000, 5000, 3000, 2000, 1200, 1000, 800, 0, 0}; // Low/High cut should be appropriate
  const float NStdDev[9] = {300, 300, 180, 124, 87, 63, 47, 28, 17};        // Read roughly from ALICE Pb-Pb 5TeV

  unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
  //unsigned int seed = 0;
  TRandom3 *rndm = new TRandom3(seed);
  TString nameNBD = "TMath::Binomial([0]+x-1, x)*TMath::Power(1-[1], x)*TMath::Power([1], [0])";
  TString nameBolz = "x*TMath::Sqrt(x*x-[0]*[0])*TMath::Exp(-x/[1])";

  inline bool RadSys(double rads, double phis, double Rx) {return sqrt(pow(rads,2)+pow(Rx,2)+2*rads*Rx*cos(phis));};
  float BosPhi(float phis, float Rx) {
    float phib = TMath::ATan(Rx*Rx*TMath::Tan(phis));
    if      (phis>TMath::Pi()/2. && phis<TMath::Pi()*3./2.)  phib += TMath::Pi();
    else if (phis<TMath::TwoPi() && phis>TMath::Pi()*3./2.)  phib += TMath::TwoPi();
    return phib;
  }

  const float TScale = 1.0;
  const float BetaScale = 0.;
  const float Rho2Scale = 0.5;
  const float FracLBC[] = {0.0348, 0.0372, 0.0375, 0.0480, 0.0580};

  float PtHistMax = 10.;
  float PtHistMin = 0.;
  int nPtBins = 100;

  float V2HistMax = 4;
  float V2HistMin = 0.2;
  int nV2Bins = 20;

  unsigned int nEvents = 1000;
  float ratioProtonLambda = 1.5;
  float ratioProtonInclusive = 5./120.;
  bool isDebug = false;
}






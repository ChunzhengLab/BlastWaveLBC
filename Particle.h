#pragma once
#include <vector>
#include "TF1.h"
#include "TLorentzVector.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TRandom3.h"
#include "par.h"

using namespace std;
using namespace par;

class Particle {
 private:
  //生成标识位
  bool isBuilt;
  //贯通参数
  float fTScale;
  float fCentrality;
  //Event中设置
  unsigned int serial_number;
  unsigned int serial_number_LBCFriend;
  int pid;
  TVector3 boost_vector;
  //par.h中读取
  float temperature;
  float mass;
  //Build后生成的参数
  TLorentzVector momentum;
  //抽样初始动量
  void SampleRawMomentum(float E);

 public:
  Particle(float TScale, float Centrality);
  ~Particle() { if(isDebug) cout << "Particle destructor called" << endl; }

  void SetSerialNumber(unsigned int serial_number) { this->serial_number = serial_number; }
  void SetSerialNumberLBCFriend(unsigned int serial_number_LBCFriend) { this->serial_number_LBCFriend = serial_number_LBCFriend; }
  void SetPid(int id);
  void SetBoostVector(TVector3 boost_vector) { this->boost_vector = boost_vector; }

  unsigned int GetSerialNumber() const { return serial_number; }
  int GetPid() const { return pid; }
  TLorentzVector GetMomentum() const { return momentum; }
  void BuildParticle();
  void Print();
};

Particle::Particle(float TScale, float Centrality) : fTScale(TScale), fCentrality(Centrality)
{
  this->isBuilt = false;
  this->pid = 0;
  this->mass = 0;
  this->temperature = 0;
}

void Particle::SetPid(int id) 
{
  unsigned int centBin = fCentrality / 10;
  this->pid = id;
  if (abs(pid) == 2212) {
    this->mass = MassProton;
    this->temperature = TempProton[centBin] * (1 + fTScale);
  } else if (abs(pid) == 3122) {
    this->mass = MassLambda;
    this->temperature = TempLambda[centBin] * (1 + fTScale);
  } else {
    cout << "Error: unknown pid " << pid << endl;
  }
}

void Particle::SampleRawMomentum(float E)
{
  float p1 = sqrt(E*E-mass*mass);
  float cstheta = 2.*(rndm->Rndm()-0.5);
  float theta = acos(cstheta);
  float pt1 = p1*sin(theta);
  float pz1 = p1*cstheta;
  float phi1 = 2.*TMath::Pi()*rndm->Rndm();
  this->momentum.SetPxPyPzE(pt1*cos(phi1),pt1*sin(phi1),pz1,E);
}

void Particle::BuildParticle()
{
  unique_ptr<TF1> fBoltz(new TF1("Boltz", nameBolz.Data(), mass, 10));
  fBoltz->SetParameter(0,mass);
  fBoltz->SetParameter(1,temperature);
  double energy_thermal = fBoltz->GetRandom();
  if(isDebug) {
    cout<<"mass = "<<mass<<endl;
    cout<<"temperature = "<<temperature<<endl;
    cout<<"energy_thermal = "<<energy_thermal<<endl;
  }
  SampleRawMomentum(energy_thermal);
  this->momentum.Boost(boost_vector);
  this->isBuilt = true;
}

void Particle::Print()
{
  cout <<">>>>>>>>>>>>>>>"<<endl;
  cout << "Has this particle been built? " << isBuilt << endl;
  cout << "Particle serial number: " << serial_number << endl;
  cout << "Particle pid: " << pid << endl;
  cout << "Particle pT: " << momentum.Pt() << endl;
  cout << "Particle eta: " << momentum.Eta() << endl;
  cout << "Particle phi: " << momentum.Phi() << endl;
  cout <<"<<<<<<<<<<<<<<<"<<endl;
}
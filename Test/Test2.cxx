#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Particle
{
private:
  double x, y, z;

public:
  Particle(double x, double y, double z) : x(x), y(y), z(z) {}
  double getR() { return sqrt(x * x + y * y + z * z); }
};

class Event
{
private:
  vector<Particle> particles;

public:
  Event(void) { vector<Particle>().swap(particles); }
  ~Event(void) { vector<Particle>().swap(particles); }
  void AddParticle(Particle *p);
  vector<Particle> GetParticles() { return particles; }
};

void Event::AddParticle(Particle *p)
{
  particles.push_back(*p);
}

int main()
{
  Event event;
  Particle p1(1, 2, 3);
  Particle p2(4, 5, 6);
  Particle *p3 = new Particle(7, 8, 9);
  event.AddParticle(&p1);
  event.AddParticle(&p2);
  event.AddParticle(p3);

  // 测试是否可以删除指针
  delete p3;
  p3 = nullptr;

  // 读取第1个粒子的R
  cout << "The first particle's R is: " << event.GetParticles()[0].getR() << endl;
  // 读取第3个粒子的R
  cout << "The third particle's R is: " << event.GetParticles()[2].getR() << endl;

  return 0;
}
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <thread>
#include "visioncheck.h"

void threadProc(const std::vector<Unit<float> >& units, std::vector<int>& visibleUnitsCounters, int start, int end)
{
  for (int i = start; i < end; i++)
  {
    int visibledUnitsCounter = 0;
    for (unsigned int j = 0; j < units.size(); j++)
      if(i != j)
        if (units[i].CheckVisibility(units[j]))
          visibledUnitsCounter++;
    visibleUnitsCounters[i] = visibledUnitsCounter;
  }
}

int main(int argc, char**argv)
{
  if (argc < 4)
  {
    std::cout << "usage: checkvision filename_in filename_out threads_count " << std::endl;
    return 1;
  }
  Unit<float>::SetDistance(2.0);
  Unit<float>::SetFov(135.5);
  std::vector<Unit<float> > units;
  units.reserve(10000);
  std::ifstream ifstr;
  ifstr.open(argv[1]);
  if (!ifstr.is_open())
  {
    std::cout << "error opening file " << argv[1] << std::endl;
    return 1;
  }
  ifstr.exceptions(std::ios_base::failbit);
  float positionx, positiony, directionx, directiony;
  while (1)
  {
    try
    {
      ifstr >> positionx;
      ifstr >> positiony;
      ifstr >> directionx;
      ifstr >> directiony;
    }
    catch (...) { break; }
    units.emplace(units.end(), positionx, positiony, directionx, directiony);
  }
  ifstr.close();
  int threadsCount = atoi(argv[3]);
  std::vector<int> visibleUnitsCounters(units.size(),  0);
  std::vector<std::thread> threads;
  for (int threadNum = 0; threadNum < threadsCount; threadNum++)
  {
    threads.emplace(threads.end(),
                    threadProc,
                    std::ref(units),
                    std::ref(visibleUnitsCounters),
                    units.size() * threadNum / threadsCount,
                    units.size() * (threadNum + 1) / threadsCount);
  }

  for (int threadNum = 0; threadNum < threadsCount; threadNum++)
  {
    threads[threadNum].join();
  }
  std::ofstream ofstr(argv[2]);
  if (!ofstr.is_open())
  {
    std::cout << "error opening file " << argv[2] << std::endl;
    return 1;
  }
  for (unsigned int i = 0; i != visibleUnitsCounters.size(); i++)
  {
    ofstr << "unit" << i + 1 << ": sees " << visibleUnitsCounters[i] << std::endl;
  }
  ofstr.close();
  std::cout << "done" << std::endl;
}
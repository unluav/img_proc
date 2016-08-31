//FrameRateMonitor
//This class will be designed to be a plug and play frame rate monitor.
//It will give instantaneous and average frame rate
#include <time.h>
#include <stdio.h>
#include "FrameRateMonitor.h"

using namespace std;

FrameRateMonitor::FrameRateMonitor() {
  avgFrameRate = 0;
  instantFrameRate = 0;
  startTime = 0;
  stopTime = 0;
  lastMark = 0;
  isStopped = 1;
  framesProcessed = 0;
}

double FrameRateMonitor::GetAverageRate() {
  return avgFrameRate;
}

double FrameRateMonitor::GetInstantRate() {
  return instantFrameRate;
}

double FrameRateMonitor::GetElapsedTime() {
  double elapsedTime;
  if (isStopped == 1) {
    elapsedTime = stopTime - startTime;
  } else {
    elapsedTime = clock() - startTime;
  }
  return elapsedTime;
}

void FrameRateMonitor::Start() {
  startTime = clock();
  isStopped = 0;
}

void FrameRateMonitor::Stop() {
  stopTime = clock();
  isStopped = 1;
}

void FrameRateMonitor::MarkFrame() {
  double timeSinceLastFrame = clock() - lastMark;
  lastMark = clock();
  instantFrameRate = 1 / ((float)timeSinceLastFrame/ CLOCKS_PER_SEC);
  avgFrameRate = (avgFrameRate * framesProcessed + instantFrameRate)/(framesProcessed + 1);
  framesProcessed++;
}

void FrameRateMonitor::DumpInfo() {
  printf("FrameRateMonitor {  ");
  printf("avgFrameRate :%f  ", avgFrameRate);
  printf("instantFrameRate :%f  ", instantFrameRate);
  printf("startTime :%f  ", (double)startTime);
  printf("stopTime :%f  ", (double)stopTime);
  printf("lastMark :%f  ", (double)lastMark);
  printf("isStopped :%d  ", isStopped);
  printf("}\n");
}

/*int main () {
  FrameRateMonitor frm;
  frm.Start();
  for (int i = 0; i < 200000000000; i++) {
    if (i % 100000 == 0) {
      frm.MarkFrame();
    }
  }
  frm.Stop();
  frm.DumpInfo();
}

*/

#ifndef FRAMERATEMONITOR_H
#define FRAMERATEMONITOR_H

class FrameRateMonitor {
  private:
    double avgFrameRate, instantFrameRate;
    clock_t startTime, stopTime, lastMark;
    int isStopped;
    int framesProcessed;
  public:
    FrameRateMonitor();
    double GetAverageRate();
    double GetInstantRate();
    double GetElapsedTime();
    void MarkFrame();
    void Start();
    void Stop();
    void DumpInfo();
};

#endif

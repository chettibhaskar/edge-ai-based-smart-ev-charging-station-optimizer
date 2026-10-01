#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

// Live bay status
extern String bayStatus;
extern float voltage, current, power, energywh, temperature;
extern unsigned long sessionStartMs;

// Edge AI
extern float predictedArrivalProb;
extern int predictedDurationMin;
extern int lastHourOfDay;
extern float predictionThreshold;

// Optimization
extern String loadDecision;
extern int throttleLevel;
extern int peakTariffStartHr;
extern int peakTariffEndHr;
extern bool overloadActive;
extern int overloadcurrentA;
extern int maxStationLoadW ;
#endif
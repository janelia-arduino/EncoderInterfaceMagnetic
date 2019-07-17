// ----------------------------------------------------------------------------
// 5x3.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef ENCODER_INTERFACE_MAGNETIC_5X3_CONSTANTS_H
#define ENCODER_INTERFACE_MAGNETIC_5X3_CONSTANTS_H
#include "Constants.h"


#if defined(__MK64FX512__)

namespace encoder_interface_magnetic
{
namespace constants
{
//MAX values must be >= 1, >= created/copied count, < RAM limit
enum {PIN_COUNT_MAX=1};

enum {SAMPLE_COUNT_MAX=4000};

struct Sample
{
  time_t time;
  unsigned long milliseconds;
  long position;
};

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
#endif

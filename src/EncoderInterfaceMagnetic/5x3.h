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
enum{PIN_COUNT_MAX=1};

enum {ENCODER_COUNT=1};
enum {OUTPUT_COUNT=4};
enum {SAMPLE_COUNT_MAX=2000};

extern const size_t encoder_a_pins[ENCODER_COUNT];
extern const size_t encoder_b_pins[ENCODER_COUNT];

extern const size_t output_pins[OUTPUT_COUNT];

struct Sample
{
  time_t time;
  unsigned long milliseconds;
  long positions[ENCODER_COUNT];
};

// Pins

// Units

// Properties
// Property values must be long, double, bool, long[], double[], bool[], char[], ConstantString *, (ConstantString *)[]
extern const bool invert_encoder_direction_default[ENCODER_COUNT];

// Parameters

// Functions

// Callbacks

// Errors
}
}
#endif
#endif

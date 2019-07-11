// ----------------------------------------------------------------------------
// Constants.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "Constants.h"


namespace encoder_interface_magnetic
{
namespace constants
{
CONSTANT_STRING(device_name,"encoder_interface_magnetic");

CONSTANT_STRING(firmware_name,"EncoderInterfaceMagnetic");
// Use semantic versioning http://semver.org/
const modular_server::FirmwareInfo firmware_info =
{
  .name_ptr=&firmware_name,
  .version_major=1,
  .version_minor=0,
  .version_patch=0,
};

CONSTANT_STRING(hardware_name,"encoder_interface_magnetic");

// Pins

// Units
CONSTANT_STRING(ms_units,"ms");

// Properties
CONSTANT_STRING(invert_encoder_direction_property_name,"invertEncoderDirection");
const bool invert_encoder_direction_element_default = false;

CONSTANT_STRING(sample_period_property_name,"samplePeriod");
const long sample_period_min = 1;
const long sample_period_max = 10000;
const long sample_period_default = 10;

// Parameters
CONSTANT_STRING(encoder_index_parameter_name,"encoder_index");

CONSTANT_STRING(position_parameter_name,"position");

// Functions
CONSTANT_STRING(get_positions_function_name,"getPositions");
CONSTANT_STRING(set_position_function_name,"setPosition");
CONSTANT_STRING(outputs_enabled_function_name,"outputsEnabled");
CONSTANT_STRING(sampling_function_name,"sampling");
CONSTANT_STRING(get_samples_function_name,"getSamples");
CONSTANT_STRING(get_sample_count_function_name,"getSampleCount");
CONSTANT_STRING(get_sample_count_max_function_name,"getSampleCountMax");

// Callbacks
CONSTANT_STRING(enable_outputs_callback_name,"enableOutputs");
CONSTANT_STRING(disable_outputs_callback_name,"disableOutputs");
CONSTANT_STRING(start_sampling_callback_name,"startSampling");
CONSTANT_STRING(stop_sampling_callback_name,"stopSampling");
CONSTANT_STRING(clear_samples_callback_name,"clearSamples");

// Errors
}
}

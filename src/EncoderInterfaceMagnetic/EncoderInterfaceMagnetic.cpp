// ----------------------------------------------------------------------------
// EncoderInterfaceMagnetic.cpp
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#include "../EncoderInterfaceMagnetic.h"


using namespace encoder_interface_magnetic;

void EncoderInterfaceMagnetic::setup()
{
  // Parent Setup
  ModularDeviceBase::setup();

  // Reset Watchdog
  resetWatchdog();

  // Event Controller Setup
  event_controller_.setup();

  // Pin Setup
  pinMode(constants::enable_pin,OUTPUT);
  enableOutputs();

  for (size_t output_index=0; output_index<constants::OUTPUT_COUNT; ++output_index)
  {
    pinMode(constants::output_pins[output_index],OUTPUT);
    digitalWrite(constants::output_pins[output_index],LOW);
  }

  // Sampling Setup
  stopSampling();

  // Set Device ID
  modular_server_.setDeviceName(constants::device_name);

  // Add Hardware
  modular_server_.addHardware(constants::hardware_info,
    pins_);

  // Add Firmware
  modular_server_.addFirmware(constants::firmware_info,
    properties_,
    parameters_,
    functions_,
    callbacks_);
  // Properties
  modular_server::Property & invert_encoder_direction_property = modular_server_.createProperty(constants::invert_encoder_direction_property_name,constants::invert_encoder_direction_default);
  invert_encoder_direction_property.attachPostSetElementValueFunctor(makeFunctor((Functor1<size_t> *)0,*this,&EncoderInterfaceMagnetic::invertEncoderDirectionHandler));

  modular_server::Property & sample_period_property = modular_server_.createProperty(constants::sample_period_property_name,constants::sample_period_default);
  sample_period_property.setUnits(constants::ms_units);
  sample_period_property.setRange(constants::sample_period_min,constants::sample_period_max);

  // Parameters
  modular_server::Parameter & encoder_index_parameter = modular_server_.createParameter(constants::encoder_index_parameter_name);
  encoder_index_parameter.setRange(0,constants::ENCODER_COUNT-1);

  modular_server::Parameter & position_parameter = modular_server_.createParameter(constants::position_parameter_name);
  position_parameter.setTypeLong();

  // Functions
  modular_server::Function & get_positions_function = modular_server_.createFunction(constants::get_positions_function_name);
  get_positions_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::getPositionsHandler));
  get_positions_function.setResultTypeArray();
  get_positions_function.setResultTypeLong();

  modular_server::Function & set_position_function = modular_server_.createFunction(constants::set_position_function_name);
  set_position_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::setPositionHandler));
  set_position_function.addParameter(encoder_index_parameter);
  set_position_function.addParameter(position_parameter);

  modular_server::Function & outputs_enabled_function = modular_server_.createFunction(constants::outputs_enabled_function_name);
  outputs_enabled_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::outputsEnabledHandler));
  outputs_enabled_function.setResultTypeBool();

  modular_server::Function & sampling_function = modular_server_.createFunction(constants::sampling_function_name);
  sampling_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::samplingHandler));
  sampling_function.setResultTypeBool();

  modular_server::Function & get_samples_function = modular_server_.createFunction(constants::get_samples_function_name);
  get_samples_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::getSamplesHandler));
  get_samples_function.setResultTypeLong();
  get_samples_function.setResultTypeArray();

  modular_server::Function & get_sample_count_function = modular_server_.createFunction(constants::get_sample_count_function_name);
  get_sample_count_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::getSampleCountHandler));
  get_sample_count_function.setResultTypeLong();

  modular_server::Function & get_sample_count_max_function = modular_server_.createFunction(constants::get_sample_count_max_function_name);
  get_sample_count_max_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::getSampleCountMaxHandler));
  get_sample_count_max_function.setResultTypeLong();

  // Callbacks
  modular_server::Callback & enable_outputs_callback = modular_server_.createCallback(constants::enable_outputs_callback_name);
  enable_outputs_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::enableOutputsHandler));

  modular_server::Callback & disable_outputs_callback = modular_server_.createCallback(constants::disable_outputs_callback_name);
  disable_outputs_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::disableOutputsHandler));

  modular_server::Callback & start_sampling_callback = modular_server_.createCallback(constants::start_sampling_callback_name);
  start_sampling_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::startSamplingHandler));

  modular_server::Callback & stop_sampling_callback = modular_server_.createCallback(constants::stop_sampling_callback_name);
  stop_sampling_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::stopSamplingHandler));


  modular_server::Callback & clear_samples_callback = modular_server_.createCallback(constants::clear_samples_callback_name);
  clear_samples_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::clearSamplesHandler));

  // Encoders Setup
  for (size_t encoder_index=0; encoder_index<constants::ENCODER_COUNT; ++encoder_index)
  {
    invertEncoderDirectionHandler(encoder_index);
  }
  // encoders_[0].attachPositiveFunctor(makeFunctor((Functor1<int32_t> *)0,*this,&EncoderInterfaceMagnetic::positiveEncoder0Handler));
  // encoders_[0].attachNegativeFunctor(makeFunctor((Functor1<int32_t> *)0,*this,&EncoderInterfaceMagnetic::negativeEncoder0Handler));

}

long EncoderInterfaceMagnetic::getPosition(size_t encoder_index)
{
  noInterrupts();
  long position = encoders_[encoder_index].read();
  interrupts();
  return position;
}

void EncoderInterfaceMagnetic::setPosition(size_t encoder_index,
  long position)
{
  encoders_[encoder_index].write(position);
}

void EncoderInterfaceMagnetic::enableOutputs()
{
  digitalWrite(constants::enable_pin,LOW);
  outputs_enabled_ = true;
}

void EncoderInterfaceMagnetic::disableOutputs()
{
  digitalWrite(constants::enable_pin,HIGH);
  outputs_enabled_ = false;
}

bool EncoderInterfaceMagnetic::outputsEnabled()
{
  return outputs_enabled_;
}

void EncoderInterfaceMagnetic::startSampling()
{
  if (event_controller_.eventsAvailable() < 1)
  {
    return;
  }
  sampling_ = true;
  long sample_period;
  modular_server_.property(constants::sample_period_property_name).getValue(sample_period);
  sampling_event_id_ = event_controller_.addInfiniteRecurringEvent(makeFunctor((Functor1<int> *)0,*this,&EncoderInterfaceMagnetic::sampleHandler),
    sample_period,
    -1);
  event_controller_.enable(sampling_event_id_);
}

void EncoderInterfaceMagnetic::stopSampling()
{
  if (sampling_)
  {
    event_controller_.clear(sampling_event_id_);
    sampling_ = false;
  }
}

bool EncoderInterfaceMagnetic::sampling()
{
  return sampling_;
}

void EncoderInterfaceMagnetic::clearSamples()
{
  samples_.clear();
}

size_t EncoderInterfaceMagnetic::getSampleCount()
{
  return samples_.size();
}

size_t EncoderInterfaceMagnetic::getSampleCountMax()
{
  return samples_.max_size();
}

// Handlers must be non-blocking (avoid 'delay')
//
// modular_server_.parameter(parameter_name).getValue(value) value type must be either:
// fixed-point number (int, long, etc.)
// floating-point number (float, double)
// bool
// const char *
// ArduinoJson::JsonArray *
// ArduinoJson::JsonObject *
// const ConstantString *
//
// For more info read about ArduinoJson parsing https://github.com/janelia-arduino/ArduinoJson
//
// modular_server_.property(property_name).getValue(value) value type must match the property default type
// modular_server_.property(property_name).setValue(value) value type must match the property default type
// modular_server_.property(property_name).getElementValue(element_index,value) value type must match the property array element default type
// modular_server_.property(property_name).setElementValue(element_index,value) value type must match the property array element default type

void EncoderInterfaceMagnetic::positiveEncoder0Handler(int32_t position)
{
  digitalWrite(constants::output_pins[0],
    !digitalRead(constants::output_pins[0]));
  digitalWrite(constants::output_pins[1],
    HIGH);
}

void EncoderInterfaceMagnetic::negativeEncoder0Handler(int32_t position)
{
  digitalWrite(constants::output_pins[0],
    !digitalRead(constants::output_pins[0]));
  digitalWrite(constants::output_pins[1],
    LOW);
}

void EncoderInterfaceMagnetic::invertEncoderDirectionHandler(size_t encoder_index)
{
  modular_server::Property & invert_encoder_direction_property = modular_server_.property(constants::invert_encoder_direction_property_name);
  bool invert_encoder_direction;
  invert_encoder_direction_property.getElementValue(encoder_index,invert_encoder_direction);

  if (!invert_encoder_direction)
  {
    encoders_[encoder_index].setup(constants::encoder_a_pins[encoder_index],
      constants::encoder_b_pins[encoder_index]);
  }
  else
  {
    encoders_[encoder_index].setup(constants::encoder_b_pins[encoder_index],
      constants::encoder_a_pins[encoder_index]);
  }
}

void EncoderInterfaceMagnetic::getPositionsHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  long position;
  for (size_t encoder_index=0; encoder_index<constants::ENCODER_COUNT; ++encoder_index)
  {
    position = getPosition(encoder_index);
    modular_server_.response().write(position);
  }

  modular_server_.response().endArray();

}

void EncoderInterfaceMagnetic::setPositionHandler()
{
  long encoder_index;
  modular_server_.parameter(constants::encoder_index_parameter_name).getValue(encoder_index);

  long position;
  modular_server_.parameter(constants::position_parameter_name).getValue(position);

  setPosition(encoder_index,position);
}

void EncoderInterfaceMagnetic::outputsEnabledHandler()
{
  bool outputs_enabled = outputsEnabled();
  modular_server_.response().returnResult(outputs_enabled);
}

void EncoderInterfaceMagnetic::samplingHandler()
{
  bool is_sampling = sampling();
  modular_server_.response().returnResult(is_sampling);
}

void EncoderInterfaceMagnetic::getSamplesHandler()
{
  modular_server_.response().writeResultKey();

  modular_server_.response().beginArray();

  for (size_t sample_index=0; sample_index<samples_.size(); ++sample_index)
  {
    constants::Sample sample = samples_[sample_index];
    modular_server_.response().beginArray();
    modular_server_.response().write(sample.time);
    modular_server_.response().write(sample.milliseconds);
    for (size_t encoder_index=0; encoder_index<constants::ENCODER_COUNT; ++encoder_index)
    {
      modular_server_.response().write(sample.positions[encoder_index]);
    }
    modular_server_.response().endArray();
  }

  modular_server_.response().endArray();
}

void EncoderInterfaceMagnetic::getSampleCountHandler()
{
  modular_server_.response().returnResult(getSampleCount());
}

void EncoderInterfaceMagnetic::getSampleCountMaxHandler()
{
  modular_server_.response().returnResult(getSampleCountMax());
}

void EncoderInterfaceMagnetic::enableOutputsHandler(modular_server::Pin * pin_ptr)
{
  enableOutputs();
}

void EncoderInterfaceMagnetic::disableOutputsHandler(modular_server::Pin * pin_ptr)
{
  disableOutputs();
}

void EncoderInterfaceMagnetic::startSamplingHandler(modular_server::Pin * pin_ptr)
{
  startSampling();
}

void EncoderInterfaceMagnetic::stopSamplingHandler(modular_server::Pin * pin_ptr)
{
  stopSampling();
}

void EncoderInterfaceMagnetic::clearSamplesHandler(modular_server::Pin * pin_ptr)
{
  clearSamples();
}

void EncoderInterfaceMagnetic::sampleHandler(int index)
{
  constants::Sample sample;
  if (timeIsSet())
  {
    sample.time = getTime();
  }
  else
  {
    sample.time = 0;
  }
  sample.milliseconds = millis();
  for (size_t encoder_index=0; encoder_index<constants::ENCODER_COUNT; ++encoder_index)
  {
    sample.positions[encoder_index] = getPosition(encoder_index);
  }
  samples_.push_back(sample);
}

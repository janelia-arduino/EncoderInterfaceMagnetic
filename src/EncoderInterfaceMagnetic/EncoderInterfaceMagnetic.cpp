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

  // Variable Setup
  position_ = 0;

  // Encoder Setup
  encoder_.setup(constants::chip_select_pin);

  // event Controller Setup
  event_controller_.setup();

  // Pin Setup

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
  modular_server::Property & sample_period_property = modular_server_.createProperty(constants::sample_period_property_name,constants::sample_period_default);
  sample_period_property.setUnits(constants::ms_units);
  sample_period_property.setRange(constants::sample_period_min,constants::sample_period_max);

  // Parameters
  modular_server::Parameter & position_parameter = modular_server_.createParameter(constants::position_parameter_name);
  position_parameter.setTypeLong();

  // Functions
  modular_server::Function & get_position_function = modular_server_.createFunction(constants::get_position_function_name);
  get_position_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::getPositionHandler));
  get_position_function.setResultTypeLong();

  modular_server::Function & set_position_function = modular_server_.createFunction(constants::set_position_function_name);
  set_position_function.attachFunctor(makeFunctor((Functor0 *)0,*this,&EncoderInterfaceMagnetic::setPositionHandler));
  set_position_function.addParameter(position_parameter);

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
  modular_server::Callback & start_sampling_callback = modular_server_.createCallback(constants::start_sampling_callback_name);
  start_sampling_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::startSamplingHandler));

  modular_server::Callback & stop_sampling_callback = modular_server_.createCallback(constants::stop_sampling_callback_name);
  stop_sampling_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::stopSamplingHandler));


  modular_server::Callback & clear_samples_callback = modular_server_.createCallback(constants::clear_samples_callback_name);
  clear_samples_callback.attachFunctor(makeFunctor((Functor1<modular_server::Pin *> *)0,*this,&EncoderInterfaceMagnetic::clearSamplesHandler));

}

long EncoderInterfaceMagnetic::getPosition()
{
  return position_;
}

void EncoderInterfaceMagnetic::setPosition(long position)
{
  noInterrupts();
  position_ = position;
  interrupts();
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

void EncoderInterfaceMagnetic::getPositionHandler()
{
  modular_server_.response().returnResult(getPosition());
}

void EncoderInterfaceMagnetic::setPositionHandler()
{
  long position;
  modular_server_.parameter(constants::position_parameter_name).getValue(position);

  setPosition(position);
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
    modular_server_.response().write(sample.position);
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
  sample.position = getPosition();
  samples_.push_back(sample);
}

// ----------------------------------------------------------------------------
// EncoderInterfaceMagnetic.h
//
//
// Authors:
// Peter Polidoro peterpolidoro@gmail.com
// ----------------------------------------------------------------------------
#ifndef ENCODER_INTERFACE_MAGNETIC_H
#define ENCODER_INTERFACE_MAGNETIC_H
#include <ArduinoJson.h>
#include <JsonStream.h>
#include <Array.h>
#include <Vector.h>
#include <ConstantVariable.h>
#include <Functor.h>

#include <EncoderFunctors.h>

#include <EventController.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "EncoderInterfaceMagnetic/Constants.h"


class EncoderInterfaceMagnetic : public ModularDeviceBase
{
public:
  virtual void setup();

  long getPosition(size_t encoder_index);
  void setPosition(size_t encoder_index,
    long position);

  void enableOutputs();
  void disableOutputs();
  bool outputsEnabled();

  void startSampling();
  void stopSampling();
  bool sampling();
  void clearSamples();
  size_t getSampleCount();
  size_t getSampleCountMax();

private:
  modular_server::Pin pins_[encoder_interface_magnetic::constants::PIN_COUNT_MAX];

  modular_server::Property properties_[encoder_interface_magnetic::constants::PROPERTY_COUNT_MAX];
  modular_server::Parameter parameters_[encoder_interface_magnetic::constants::PARAMETER_COUNT_MAX];
  modular_server::Function functions_[encoder_interface_magnetic::constants::FUNCTION_COUNT_MAX];
  modular_server::Callback callbacks_[encoder_interface_magnetic::constants::CALLBACK_COUNT_MAX];

  EncoderFunctors encoders_[encoder_interface_magnetic::constants::ENCODER_COUNT];

  bool outputs_enabled_;
  bool sampling_;

  EventController<encoder_interface_magnetic::constants::EVENT_COUNT_MAX> event_controller_;
  EventId sampling_event_id_;

  Array<encoder_interface_magnetic::constants::Sample,
    encoder_interface_magnetic::constants::SAMPLE_COUNT_MAX> samples_;

  // Handlers
  void positiveEncoder0Handler(int32_t position);
  void negativeEncoder0Handler(int32_t position);
  void invertEncoderDirectionHandler(size_t encoder_index);
  void getPositionsHandler();
  void setPositionHandler();
  void outputsEnabledHandler();
  void samplingHandler();
  void getSamplesHandler();
  void getSampleCountHandler();
  void getSampleCountMaxHandler();
  void enableOutputsHandler(modular_server::Pin * pin_ptr);
  void disableOutputsHandler(modular_server::Pin * pin_ptr);
  void startSamplingHandler(modular_server::Pin * pin_ptr);
  void stopSamplingHandler(modular_server::Pin * pin_ptr);
  void clearSamplesHandler(modular_server::Pin * pin_ptr);
  void sampleHandler(int index);

};

#endif

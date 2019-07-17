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

#include <AS5048.h>

#include <EventController.h>

#include <ModularServer.h>
#include <ModularDeviceBase.h>

#include "EncoderInterfaceMagnetic/Constants.h"


class EncoderInterfaceMagnetic : public ModularDeviceBase
{
public:
  virtual void setup();

  long getPosition();
  void setPosition(long position);
  long getPositionsPerRevolution();

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

  AS5048 encoder_;
  long angle_previous_;
  long position_;

  bool sampling_;

  EventController<encoder_interface_magnetic::constants::EVENT_COUNT_MAX> event_controller_;
  EventId sampling_event_id_;

  Array<encoder_interface_magnetic::constants::Sample,
    encoder_interface_magnetic::constants::SAMPLE_COUNT_MAX> samples_;

  // Handlers
  void getPositionHandler();
  void setPositionHandler();
  void getPositionsPerRevolutionHandler();
  void samplingHandler();
  void getSamplesHandler();
  void getSampleCountHandler();
  void getSampleCountMaxHandler();
  void startSamplingHandler(modular_server::Pin * pin_ptr);
  void stopSamplingHandler(modular_server::Pin * pin_ptr);
  void clearSamplesHandler(modular_server::Pin * pin_ptr);
  void sampleHandler(int index);

};

#endif

#ifndef _SOFTSPI_H_
#define _SOFTSPI_H_

#include <nan.h>
#include <mraa/gpio.h>

class SoftSpi : public Nan::ObjectWrap {
public:
  typedef enum {
    SPI_MODE0 = 0,
    SPI_MODE1 = 1,
    SPI_MODE2 = 2,
    SPI_MODE3 = 3
  } SpiMode;

  static const uint32_t DEFAULT_FREQUENCY = 10000;
  static const SpiMode DEFAULT_SPI_MODE = SPI_MODE0;
  static const uint32_t DEFAULT_BETWEEN_BYTE_DELAY_US = 0;
  static const uint32_t LOOPS_PER_SECOND = 67050000;

  static NAN_MODULE_INIT(Init);

  void setFrequency(uint32_t frequency);
  void setMode(SpiMode mode);
  void setBetweenByteDelay_us(uint32_t t);
  void write(uint8_t* buffer, uint32_t length);

private:
  mraa_gpio_context m_sck;
  mraa_gpio_context m_mosi;
  mraa_gpio_context m_miso;
  SpiMode m_mode;
  uint8_t m_cpol;
  uint32_t m_frequency;
  uint32_t m_clockSleepCount;
  uint32_t m_betweenByteSleepCount;

  explicit SoftSpi(
    mraa_gpio_context sck,
    mraa_gpio_context mosi,
    mraa_gpio_context miso
  );
  ~SoftSpi();

  static NAN_METHOD(New);
  static NAN_METHOD(write);
  static NAN_METHOD(frequency);
  static NAN_METHOD(mode);
  static NAN_METHOD(betweenByteDelay_us);
  static Nan::Persistent<v8::Function> constructor;

  uint8_t writeByte(uint8_t b);
  uint8_t writeBit(uint8_t b);
  void usleepByCounting(uint32_t sleepCount);
};

#endif

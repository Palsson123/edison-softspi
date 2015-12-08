#include "softspi.h"
#ifdef LOCAL
#define MRAA_GPIO_OUT 1
#define MRAA_GPIO_IN  2
typedef void* mraa_gpio_context;
mraa_gpio_context mraa_gpio_init(uint32_t pin) {
  printf("mraa_gpio_init %d\n", pin);
  return (void*)pin;
}
void mraa_gpio_dir(mraa_gpio_context ctx, int dir) {
  printf("mraa_gpio_dir %p %d\n", ctx, dir);
}
void mraa_gpio_use_mmaped(mraa_gpio_context ctx, int mmaped) {
  printf("mraa_gpio_use_mmaped %p %d\n", ctx, mmaped);
}
void mraa_gpio_close(mraa_gpio_context ctx) {
  printf("mraa_gpio_close %p\n", ctx);
}
void mraa_gpio_write(mraa_gpio_context ctx, int b) {
  printf("mraa_gpio_write %p %d\n", ctx, b);
}
int  mraa_gpio_read(mraa_gpio_context ctx) {
  printf("mraa_gpio_read %p\n", ctx);
  return 0;
}
#else
#include <mraa/gpio.h>
#endif

void usleep(uint32_t sleepLoopCount);

uint8_t softSpi_writeByte(
  mraa_gpio_context sck,
  mraa_gpio_context mosi,
  mraa_gpio_context miso,
  uint32_t spiMode,
  uint32_t frequency,
  uint8_t byte
);

uint8_t softSpi_writeBit(
  mraa_gpio_context sck,
  mraa_gpio_context mosi,
  mraa_gpio_context miso,
  uint32_t spiMode,
  uint32_t frequency,
  uint8_t bit
);

NAN_METHOD(softSpi_transfer) {
  unsigned char* buffer = (unsigned char*) node::Buffer::Data(info[0]->ToObject());
  uint32_t bufferLength = (uint32_t) node::Buffer::Length(info[0]->ToObject());
  v8::Local<v8::Object> options = info[1]->ToObject();
  uint32_t sckPin = options->Get(Nan::New("sck").ToLocalChecked())->ToUint32()->Value();
  uint32_t mosiPin = options->Get(Nan::New("mosi").ToLocalChecked())->ToUint32()->Value();
  uint32_t misoPin = options->Get(Nan::New("miso").ToLocalChecked())->ToUint32()->Value();
  uint32_t spiMode = options->Get(Nan::New("mode").ToLocalChecked())->ToUint32()->Value();
  uint32_t frequency = options->Get(Nan::New("frequency").ToLocalChecked())->ToUint32()->Value();

  mraa_gpio_context sck = mraa_gpio_init(sckPin);
  mraa_gpio_dir(sck, MRAA_GPIO_OUT);
  mraa_gpio_use_mmaped(sck, 1);

  mraa_gpio_context mosi = mraa_gpio_init(mosiPin);
  mraa_gpio_dir(mosi, MRAA_GPIO_OUT);
  mraa_gpio_use_mmaped(mosi, 1);

  mraa_gpio_context miso = mraa_gpio_init(misoPin);
  mraa_gpio_dir(miso, MRAA_GPIO_IN);
  mraa_gpio_use_mmaped(miso, 1);

  int32_t sleepLoopCount = 0.999 * (1.0 / (double)frequency) * (1000.0 * 1000.0) / 2.0 - 92.5;
  if(sleepLoopCount < 0) {
    sleepLoopCount = 1;
  }

  for(uint32_t i = 0; i < bufferLength; i++) {
    buffer[i] = softSpi_writeByte(sck, mosi, miso, spiMode, sleepLoopCount, buffer[i]);
  }

  mraa_gpio_close(sck);
  mraa_gpio_close(mosi);
  mraa_gpio_close(miso);
}

uint8_t softSpi_writeByte(
  mraa_gpio_context sck,
  mraa_gpio_context mosi,
  mraa_gpio_context miso,
  uint32_t spiMode,
  uint32_t sleepLoopCount,
  uint8_t byte
) {
  uint8_t result = 0;
  for(int i = 0; i < 8; i++) {
    int bit = byte & 0x80;
    result = result << 1;
    result |= softSpi_writeBit(sck, mosi, miso, spiMode, sleepLoopCount, bit);
    byte = byte >> 1;
  }
  return result;
}

uint8_t softSpi_writeBit(
  mraa_gpio_context sck,
  mraa_gpio_context mosi,
  mraa_gpio_context miso,
  uint32_t spiMode,
  uint32_t sleepLoopCount,
  uint8_t bit
) {
  if(spiMode == 3) {
    // CPOL = 1, CPHA = 1, Clock idle low, data is clocked in on rising, edge output data (change) on falling edge
    mraa_gpio_write(sck, 0);
    mraa_gpio_write(mosi, bit ? 1 : 0);
    usleep(sleepLoopCount);
    mraa_gpio_write(sck, 1);
    usleep(sleepLoopCount);
    return mraa_gpio_read(miso);
  } else {
    printf("unhandled spi mode %d\n", spiMode);
    return -1;
  }
}

void usleep(uint32_t sleepLoopCount) {
  #ifdef LOCAL
  printf("usleep %d\n", sleepLoopCount);
  #endif
  volatile uint32_t i;
  for(i = 1; i < sleepLoopCount; i++);
}

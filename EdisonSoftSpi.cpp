#include "softspi.h"

NAN_MODULE_INIT(InitAll) {
  SoftSpi::Init(target);
}

NODE_MODULE(EdisonSoftSpi, InitAll)

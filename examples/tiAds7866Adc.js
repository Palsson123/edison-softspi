// test driver for the TI ADS7866 ADC

var softspi = require('../');
var mraa = require('mraa');

var GPIO15 = 48;
var GPIO19 = 19;
var GPIO20 = 7;
var GPIO109 = 10;
var GPIO114 = 24;
var GPIO115 = 11;

var CSADDR0 = GPIO19;
var CSADDR1 = GPIO20;
var CSADDR2 = GPIO15;
var SPI_SCK = GPIO109;
var SPI_MOSI = GPIO115;
var SPI_MISO = GPIO114;

if(!process.env.LOCAL){
  var cs = [];
  cs[0] = new mraa.Gpio(CSADDR0);
  cs[0].dir(mraa.DIR_OUT);
  cs[1] = new mraa.Gpio(CSADDR1);
  cs[1].dir(mraa.DIR_OUT);
  cs[2] = new mraa.Gpio(CSADDR2);
  cs[2].dir(mraa.DIR_OUT);

  deassertCS();
}

assertCS();
var buf = new Buffer(2);
var opts = {
  sck: SPI_SCK,
  mosi: SPI_MOSI,
  miso: SPI_MISO,
  mode: softspi.MODE3,
  frequency: 10000
};
softspi.transfer(buf, opts);
deassertCS();
console.log(buf.toJSON());

function assertCS() {
  var addr = 0;
  if(!process.env.LOCAL){
    cs[0].write(addr & 0x01);
    cs[1].write(addr & 0x02);
    cs[2].write(addr & 0x04);
  }
}

function deassertCS() {
  var addr = 7;
  if(!process.env.LOCAL){
    cs[0].write(addr & 0x01);
    cs[1].write(addr & 0x02);
    cs[2].write(addr & 0x04);
  }
}

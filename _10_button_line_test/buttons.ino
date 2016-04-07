boolean time_to_trigger( long now, long *lastNow, long callPeriod) { // need a pointer variable to get the address of a value
  if ((now - *lastNow) >= callPeriod) { // if scan period is reached
    *lastNow = now;
    return true;
  }
  else {
    return false;
  }
}


void disarm_cell(byte secId, byte cellId, byte segment[] , byte clk, byte data, byte lt ) { //Assuming that cell is already armed!
  byte index = secId * 5 + cellId;
  byte mask = B10111111;
  segment[index] &= mask;

  for (int i = 0; i < index; i++) { // I believe you cant access adress without accessing conseqtuive values
    segment++; // slide the pointer value
  }

  shiftToSec(segment, clk, data, lt);
}
void disarm_cell(byte t, byte secId, byte cellId, byte segment[] , byte clk, byte data, byte lt ) { //Assuming that cell is already armed!
  byte index = secId * 5 + cellId;
  byte mask = B10111111;
  segment[index] &= mask;
  shiftToSec(secId * 5, segment, clk, data, lt);
}


void arm_cell(byte secId, byte cellId, byte segment[] , byte clk, byte data, byte lt ) {
  byte index = (secId * 5) + cellId;
  byte mask = B01000000;
  segment[index] |= mask;
  for (int i = 0; i < index; i++) { // I believe you cant access adress without accessing conseqtuive values
    segment++; // slide the pointer value
  }
  shiftToSec(segment, clk, data, lt);
}


void arm_cell(byte t, byte secId, byte cellId, byte segment[] , byte clk, byte data, byte lt ) {
  byte index = (secId * 5) + cellId;
  byte mask = B01000000;
  segment[index] |= mask;
  shiftToSec(secId * 5, segment, clk, data, lt); // i still shift to sector therefore i must use sec Id *5
}

void disarm_sector(byte secId, byte segment[] , byte clk, byte data, byte lt) {
  byte index = secId * 5;
  byte mask = B10111111;
  for (byte i = 0; i < 5; i++) {
    segment[index + i] &= mask;
  }

  for (int i = 0; i < index; i++) { // I believe you cant access adress without accessing conseqtuive values
    segment++; // slide the pointer value
  }

  shiftToSec(segment, clk, data, lt);
}

//index SEC*5
void disarm_sector(byte t , byte secId, byte segment[] , byte clk, byte data, byte lt) { //{PSD
  byte index = secId * 5;
  byte mask = B10111111;
  for (byte i = 0; i < 5; i++) {
    segment[index + i] &= mask;
  }

  shiftToSec(index, segment, clk, data, lt); // not using disarm cell for greater efficiency
}

void disarm_segment( byte segment[] , byte clk, byte data, byte lt) {
  for (byte secId = 0; secId < 5; secId++) {
    disarm_sector(secId, segment, clk, data, lt);
  }
}


void disarm_segment(byte t,  byte segment[] , byte clk, byte data, byte lt) {
  for (byte secId = 0; secId < 5; secId++) {
    disarm_sector(1, secId, segment, clk, data, lt);
  }
}


void arm_sector(byte secId, byte segment[] , byte clk, byte data, byte lt) { //???works crappy
  byte index = (secId * 5);
  byte mask = B01000000;
  for (byte i = 0; i < 5; i++) {
    segment[index + i] |= mask;
  }

  for (int i = 0; i < index; i++) { // I believe you cant access adress without accessing conseqtuive values
    segment++; // slide the pointer value
  }

  shiftToSec(segment, clk, data, lt);
}


void arm_sector(byte t , byte secId, byte segment[] , byte clk, byte data, byte lt) { //PSD
  byte index = (secId * 5);
  byte mask = B01000000;
  for (byte i = 0; i < 5; i++) {
    segment[index + i] |= mask;
  }
  shiftToSec(index, segment, clk, data, lt);
}



void arm_segment( byte segment[] , byte clk, byte data, byte lt) {
  for (byte secId = 0; secId < 5; secId++) {
    arm_sector(secId, segment, clk, data, lt);
  }
}



void arm_segment(byte t,  byte segment[] , byte clk, byte data, byte lt) {
  for (byte secId = 0; secId < 5; secId++) {
    arm_sector(1, secId, segment, clk, data, lt);
  }
}

boolean line_is_hot(uint8_t line) {

  if (digitalRead(line) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}


uint8_t button_click (uint8_t line, byte segment[] , byte clk, byte data, byte lt) { // this method can only be called when button has stopped bouncing
  // will return row (sector) and collumn (cell) if the two nibbles of a byte MSN is row and LSN is col

  uint8_t row = 0xFF; // if method returns 0xFF this means that no row was found pressed when checking the grid
  uint8_t col = 0x7F; // 0x7F will be returned if no col was pressed while checking the grid
  boolean prLine = true, crLine = true, pcLine = true , ccLine = true; // these variables must be initialized as true

  disarm_sector(9,1, segment, clk, data, lt);
  crLine = line_is_hot(line);
  if (crLine != prLine) { // will not work elsewise

    arm_cell(9,1, 0, segment , clk, data, lt ); //these must change with different segment
    if (line_is_hot(line)) {
      return 0;
    }
    arm_cell(9,1, 1, segment , clk, data, lt ); //these must change with different segment
    if (line_is_hot(line)) {
      return 1;
    }
    arm_cell(9,1, 2, segment , clk, data, lt ); //these must change with different segment
    if (line_is_hot(line)) {
      return 2;
    }
    arm_cell(9,1, 3, segment , clk, data, lt ); //these must change with different segment
    if (line_is_hot(line)) {
      return 3;
    }
    arm_cell(9,1, 4, segment , clk, data, lt ); //these must change with different segment
    if (line_is_hot(line)) {
      return 4;
    }
    else {
      return 0x7F;
    }
  }
  return 0xFF;
}




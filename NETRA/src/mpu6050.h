#pragma once

struct ImuReading {
  float pitch;
  float yaw;
};

void initImu(int sda, int scl);
ImuReading readImu();

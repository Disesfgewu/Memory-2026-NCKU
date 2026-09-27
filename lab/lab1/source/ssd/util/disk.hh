/*
 * Copyright (C) 2017 CAMELab
 */

#pragma once

#ifndef __UTIL_DISK__
#define __UTIL_DISK__

#include <cinttypes>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace SSDLab {

class Disk {
 protected:
  std::string filename;
  uint64_t diskSize;
  uint32_t sectorSize;

  std::fstream disk;

 public:
  Disk();
  Disk(const Disk &) = delete;
  virtual ~Disk();

  virtual uint64_t open(std::string, uint64_t, uint32_t);
  virtual void close();

  virtual uint16_t read(uint64_t, uint16_t, uint8_t *);
  virtual uint16_t write(uint64_t, uint16_t, uint8_t *);
  virtual uint16_t erase(uint64_t, uint16_t);
};

class CoWDisk : public Disk {
 private:
  std::unordered_map<uint64_t, std::vector<uint8_t>> table;

 public:
  CoWDisk();
  CoWDisk(const CoWDisk &) = delete;
  ~CoWDisk();

  void close() override;

  uint16_t read(uint64_t, uint16_t, uint8_t *) override;
  uint16_t write(uint64_t, uint16_t, uint8_t *) override;
};

class MemDisk : public Disk {
 private:
  std::unordered_map<uint64_t, std::vector<uint8_t>> table;

 public:
  MemDisk();
  MemDisk(const MemDisk &) = delete;
  ~MemDisk();

  uint64_t open(std::string, uint64_t, uint32_t) override;
  void close() override;

  uint16_t read(uint64_t, uint16_t, uint8_t *) override;
  uint16_t write(uint64_t, uint16_t, uint8_t *) override;
  uint16_t erase(uint64_t, uint16_t) override;
};

}  // namespace SSDLab

#endif

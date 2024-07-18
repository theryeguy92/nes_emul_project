#pragma once
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <memory> // Add this include for std::shared_ptr

#include "Mapper_000.h"
#include "Mapper_001.h"
#include "Mapper_002.h"
#include "Mapper_003.h"
#include "Mapper_004.h"
#include "Mapper_066.h"

enum class MIRROR
{
    HORIZONTAL,
    VERTICAL,
    ONE_SCREEN_LO,
    ONE_SCREEN_HI,
    HARDWARE
};

class Cartridge
{
public:
    Cartridge(const std::string& sFileName);
    ~Cartridge();

public:
    bool ImageValid();

private:
    bool bImageValid = false;
    MIRROR hw_mirror = MIRROR::HORIZONTAL;

    uint8_t nMapperID = 0;
    uint8_t nPRGBanks = 0;
    uint8_t nCHRBanks = 0;

    std::vector<uint8_t> vPRGMemory;
    std::vector<uint8_t> vCHRMemory;

    std::shared_ptr<Mapper> pMapper;

public:
    // Communication with Main Bus
    bool cpuRead(uint16_t addr, uint8_t& data);
    bool cpuWrite(uint16_t addr, uint8_t data);

    // Communication with PPU Bus
    bool ppuRead(uint16_t addr, uint8_t& data);
    bool ppuWrite(uint16_t addr, uint8_t data);

    // Permits system reset of mapper to know state
    void reset();

    // Get Mirror configuration
    MIRROR Mirror();

    std::shared_ptr<Mapper> GetMapper();
};

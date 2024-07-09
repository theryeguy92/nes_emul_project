#pragma once
#include "Mapper.h"

class Mapper_067 : public Mapper
{
public:
    Mapper_067(uint8_t prgBanks, uint8_t chrBanks);
    ~Mapper_067();

public:
    bool cpuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool cpuMapWrite(uint16_t addr, uint32_t& mapped_addr, uint8_t data = 0) override;
    bool ppuMapRead(uint16_t addr, uint32_t& mapped_addr) override;
    bool ppuMapWrite(uint16_t addr, uint32_t& mapped_addr) override;
    void reset() override;

private:
    uint8_t prgBankSelect;
    uint8_t chrBankSelect;
};

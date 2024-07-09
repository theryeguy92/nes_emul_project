#include "Cartridge.h"
#include "Mapper_000.h"
#include "Mapper_067.h" // Include the Mapper_067 header file
#include <iostream>

Cartridge::Cartridge(const std::string& sFileName)
{
    // iNES Format Header
    struct sHeader
    {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bImageValid = false;

    std::ifstream ifs;
    ifs.open(sFileName, std::ifstream::binary);
    if (ifs.is_open())
    {
        // Read file header
        ifs.read((char*)&header, sizeof(sHeader));

        // If a "trainer" exists we just need to read past
        // it before we get to the good stuff
        if (header.mapper1 & 0x04)
            ifs.seekg(512, std::ios_base::cur);

        // Determine Mapper ID
        nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
        mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

        // "Discover" File Format
        uint8_t nFileType = 1;

        if (nFileType == 0)
        {
            // Handle file type 0 if needed
        }

        if (nFileType == 1)
        {
            nPRGBanks = header.prg_rom_chunks;
            vPRGMemory.resize(nPRGBanks * 16384);
            ifs.read((char*)vPRGMemory.data(), vPRGMemory.size());

            nCHRBanks = header.chr_rom_chunks;
            if (nCHRBanks == 0)
            {
                // Create CHR RAM
                vCHRMemory.resize(8192);
            }
            else
            {
                // Allocate for ROM
                vCHRMemory.resize(nCHRBanks * 8192);
            }
            ifs.read((char*)vCHRMemory.data(), vCHRMemory.size());
        }

        if (nFileType == 2)
        {
            // Handle file type 2 if needed
        }

        // Load appropriate mapper
        switch (nMapperID)
        {
        case 0:
            pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
            std::cout << "Mapper_000 initialized" << std::endl; // Debug print
            break;
        case 67:
            pMapper = std::make_shared<Mapper_067>(nPRGBanks, nCHRBanks);
            std::cout << "Mapper_067 initialized" << std::endl; // Debug print
            break;
            // Uncomment and implement other cases if needed
            // case 2: pMapper = std::make_shared<Mapper_002>(nPRGBanks, nCHRBanks); break;
            // case 3: pMapper = std::make_shared<Mapper_003>(nPRGBanks, nCHRBanks); break;
            // case 66: pMapper = std::make_shared<Mapper_066>(nPRGBanks, nCHRBanks); break;
        default:
            std::cerr << "Unsupported mapper ID: " << (int)nMapperID << std::endl; // Debug print
            break;
        }

        if (pMapper == nullptr)
        {
            std::cerr << "Failed to initialize mapper" << std::endl; // Debug print
        }
        else
        {
            std::cout << "Mapper initialized successfully" << std::endl; // Debug print
            bImageValid = true;
        }

        ifs.close();
    }
}

Cartridge::~Cartridge()
{
}

bool Cartridge::ImageValid()
{
    return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t& data)
{
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapRead(addr, mapped_addr))
    {
        data = vPRGMemory[mapped_addr];
        return true;
    }
    else
        return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if (pMapper->cpuMapWrite(addr, mapped_addr, data))
    {
        vPRGMemory[mapped_addr] = data;
        return true;
    }
    else
        return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t& data)
{
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapRead(addr, mapped_addr))
    {
        data = vCHRMemory[mapped_addr];
        return true;
    }
    else
        return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data)
{
    uint32_t mapped_addr = 0;
    if (pMapper->ppuMapWrite(addr, mapped_addr))
    {
        vCHRMemory[mapped_addr] = data;
        return true;
    }
    else
        return false;
}

void Cartridge::reset()
{
    // Note: This does not reset the ROM contents,
    // but does reset the mapper.
    if (pMapper != nullptr)
        pMapper->reset();
}

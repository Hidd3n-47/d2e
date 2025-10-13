#pragma once

#include <string>

namespace d2eNet
{

enum class PacketLineType : uint8_t
{
    FLOAT = 0,
    ADD_GAME_OBJECT,
    ADD_COMPONENT,
};

class Packet
{
public:
    [[nodiscard]] inline std::string GetPacketString() const { return mPacketString; }

    inline void AddLineType(const PacketLineType lineType)
    {
        AddStringToPacket(lineType);
    }

    template<typename T>
    inline void AddType()
    {
        AddStringToPacket(PacketLineType::ADD_COMPONENT, typeid(T).name());
    }
private:
    std::string mPacketString;

    inline void AddStringToPacket(const PacketLineType lineType, const std::string& string = "")
    {
        mPacketString += static_cast<char>(static_cast<uint8_t>(lineType))
                      +  static_cast<char>(static_cast<uint8_t>(string.size()));
        mPacketString += string;
    }
};

} // Namespace d2eNet.
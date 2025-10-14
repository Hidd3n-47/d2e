#pragma once

#include <string>

namespace d2eNet
{

enum class PacketLineType : uint8_t
{
    ADD_GAME_OBJECT = 1,
    ADD_COMPONENT,
};

class Packet
{
public:
    class Iterator
    {
    public:
        Iterator(const uint32_t index) : mIndex(index) { /* Empty. */ }

        [[nodiscard]] bool operator==(const Iterator& rhs) const
        {
            return mIndex == rhs.mIndex;
        }
    private:
        uint32_t mIndex;
    };

public:
    Packet() = default;
    inline Packet(const uint8_t* data, const uint16_t size) {
        mBuffer.insert(mBuffer.begin(), data, data + size * sizeof(uint8_t)); }

    [[nodiscard]] inline PacketLineType GetPacketLineType() const { return static_cast<PacketLineType>(mBuffer[0]); }
    [[nodiscard]] inline std::string GetPacketString() const { return std::string{ mBuffer.begin(), mBuffer.end() }; }
    [[nodiscard]] inline void* GetData() const { return (void*)mBuffer.data(); }
    [[nodiscard]] inline uint32_t GetCount() const { return static_cast<uint32_t>(mBuffer.size()); }

    inline void AddLineType(const PacketLineType lineType)
    {
        AddStringToPacket(lineType);
    }

    template<typename T>
    inline void AddType()
    {
        AddStringToPacket(PacketLineType::ADD_COMPONENT, typeid(T).name());
    }

    Iterator Begin() { return Iterator{ 0 }; }
    Iterator End() const { return Iterator{ static_cast<uint32_t>(mBuffer.size()) }; }
private:
    std::vector<uint8_t> mBuffer;

    inline void AddStringToPacket(const PacketLineType lineType, const std::string& string = "")
    {
        mBuffer.push_back(static_cast<uint8_t>(lineType));
        mBuffer.emplace_back(static_cast<uint8_t>(string.length()));
        mBuffer.insert(mBuffer.end(), string.begin(), string.end());
    }
};

} // Namespace d2eNet.
#pragma once

#include <string>

namespace d2eNet
{

enum class PacketLineType : uint8_t
{
    ADD_GAME_OBJECT = 1,
    ADD_COMPONENT,
    SET_COMPONENT_VALUE
};

class Packet
{
public:
friend class Iterator;
    class Iterator
    {
    public:
        Iterator(const Packet* ptr, const uint32_t index) : mPtr(ptr), mIndex(index) { /* Empty. */ }

        [[nodiscard]] inline PacketLineType GetPacketLineType() const { return static_cast<PacketLineType>(mPtr->mBuffer[mIndex]); }
        [[nodiscard]] inline uint8_t GetPacketLineLength() const { return mPtr->mBuffer[mIndex + 1]; }
        [[nodiscard]] inline std::string GetPacketLineString() const
        {
            if (mPtr->mBuffer[mIndex + 1] == 0)
            {
                return {};
            }

            std::string testing;
            for (uint32_t i = mIndex + 2; i <= mIndex + 2 + GetPacketLineLength() - 1; ++i)
            {
                testing += static_cast<char>(mPtr->mBuffer[i]);
            }
            return testing;

            return std::string{
                &mPtr->mBuffer[mIndex + 2],
                &mPtr->mBuffer[mIndex + 2 + GetPacketLineLength() - 1] };
        }

        [[nodiscard]] bool operator==(const Iterator& rhs) const
        {
            return mIndex == rhs.mIndex;
        }

        [[nodiscard]] bool operator!=(const Iterator& rhs) const
        {
            return mIndex != rhs.mIndex;
        }

        Iterator operator++()
        {
            const uint8_t offset = mPtr->mBuffer[mIndex + 1];

            const size_t bufferSize = mPtr->mBuffer.size();
            const uint32_t potentialNextIndex = mIndex + 2 + offset;

            mIndex = potentialNextIndex > bufferSize ? static_cast<uint32_t>(bufferSize) : potentialNextIndex;

            return *this;
        }
    private:
        const Packet* mPtr;
        uint32_t mIndex;
    };

public:
    Packet() = default;
    inline Packet(const uint8_t* data, const uint16_t size) {
        mBuffer.insert(mBuffer.begin(), data, data + size * sizeof(uint8_t)); }

    [[nodiscard]] inline PacketLineType GetPacketLineType() const { return static_cast<PacketLineType>(mBuffer[0]); }
    [[nodiscard]] inline void* GetData() const { return (void*)mBuffer.data(); }
    [[nodiscard]] inline uint32_t GetCount() const { return static_cast<uint32_t>(mBuffer.size()); }

    inline void AddLineType(const PacketLineType lineType, const std::string& string = "")
    {
        AddStringToPacket(lineType, string);
    }

    template<typename T>
    inline void AddType()
    {
        AddStringToPacket(PacketLineType::ADD_COMPONENT, T::GetName());
    }

    Iterator Begin() const { return Iterator{ this, 0 }; }
    Iterator End()   const { return Iterator{ this, static_cast<uint32_t>(mBuffer.size()) }; }
private:
    std::vector<uint8_t> mBuffer;

    inline void AddStringToPacket(const PacketLineType lineType, const std::string& string)
    {
        mBuffer.push_back(static_cast<uint8_t>(lineType));
        mBuffer.emplace_back(static_cast<uint8_t>(string.length()));
        mBuffer.insert(mBuffer.end(), string.begin(), string.end());
    }
};

} // Namespace d2eNet.
#pragma once

#include <string>

namespace d2eNet
{
/**
 * @enum PacketLineType: Enum class that represents the type of packet.
 * - \c ADD_GAME_OBJECT:                 Packet contains information about adding a game object.
 * - \c ADD_COMPONENT:                   Packet contains information about adding a component to a game object.
 * - \c UPDATE_COMPONENT:                Packet contains information about updating values of a preexisting added component.
 * - \c SYNC_GAME_OBJECT_ACROSS_NETWORK: Packet contains information telling the server about an object that values should be synchronised per frame.
 * - \c PLAYER_TWO_JOINED:               Packet containing information telling clients that a second player has joined the game.
 * - \c SERVER_HANDLED_PACKET_CONFIRM:   Packet containing information about a packet who needs to be acknowledged on the server and sent back to the clients.
 * - \c PLAYER_DISCONNECTED:             Packet containing information about a player disconnecting.
 */
enum class PacketLineType : uint8_t
{
    ADD_GAME_OBJECT = 1,
    ADD_COMPONENT,
    UPDATE_COMPONENT,
    SYNC_GAME_OBJECT_ACROSS_NETWORK,
    PLAYER_TWO_JOINED,
    SERVER_HANDLED_PACKET_CONFIRM,
    PLAYER_DISCONNECTED
};

/**
 * @class Packet: A packet is used to send information between a client and a host (either direction).
 * The class is there to assist the user in being able to construct information in a set layout so that the client and host can handle the information.
 * Packets contains information that are normally in the following layout:
 * [uint_8: Packet Type] [uint_8: Length of string] [string]
 * Therefore a packet can be a single line like above, or multiple.
 * @see \c PacketLineType
 */
class Packet
{
public:
    /**
     * @class Iterator: A class that is used to help users iterate over a packet.
     * Since the packet can contain multiple "lines" the iterator takes the received string from the client/host and will break it into
     * processable lines in the formate: [uint_8: Packet Type] [uint_8: Length of string] [string].
     * @example
     * Received packet from client/host:
     * { [type1] [length1] [string1] [type2] [length2] [string2] }
     * The iterator will break this into to lines to be processed.
     * PacketLine1 = { [type1] [length1] [string1] } and PacketLine2 = { [type2] [length2] [string2] }
     * This way the first 'line' [PacketLine1] can be processed, and when the iterator is incremented, we go to the next line [PacketLine2]
     */
    friend class Iterator;
    class Iterator
    {
    public:
        Iterator(const Packet* ptr, const uint32_t index) : mPtr(ptr), mIndex(index) { /* Empty. */ }

        /**
         * @brief Get the line type of the packet line: { this -> [uint_8: Packet Type] } [uint_8: Length of string] [string]
         * @see PacketLineType
         * @return The type of line that the line represents.
         */
        [[nodiscard]] inline PacketLineType GetPacketLineType()   const { return static_cast<PacketLineType>(mPtr->mBuffer[mIndex]); }
        /**
         * @brief Get the length of the line string: [uint_8: Packet Type] { this -> [uint_8: Length of string] } [string]
         * @return The length of the line string.
         */
        [[nodiscard]] inline uint8_t        GetPacketLineLength() const { return mPtr->mBuffer[mIndex + 1]; }
        /**
         * @brief Get the string of the line [uint_8: Packet Type] [uint_8: Length of string] { this -> [string] }
         * @return The line string.
         */
        [[nodiscard]] inline std::string    GetPacketLineString() const
        {
            if (mPtr->mBuffer[mIndex + 1] == 0)
            {
                return {};
            }

            std::string str;
            for (uint32_t i = mIndex + 2; i <= mIndex + 2 + GetPacketLineLength() - 1; ++i)
            {
                str += static_cast<char>(mPtr->mBuffer[i]);
            }
            return str;

            //todo look at this.
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

        /**
         * @brief The pre-increment operator to move the iterator to the next packet 'line'.
         * @return The iterator pointing to the start of the next packet 'line'.
         */
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
    Packet(const bool reliable) : mReliable(reliable) { }
    inline Packet(const uint8_t* data, const uint16_t size) {
        mBuffer.insert(mBuffer.begin(), data, data + static_cast<size_t>(size) * sizeof(uint8_t)); }

    [[nodiscard]] inline void*    GetData()    const { return (void*)mBuffer.data(); }
    [[nodiscard]] inline uint32_t GetCount()   const { return static_cast<uint32_t>(mBuffer.size()); }
    [[nodiscard]] inline bool     IsReliable() const { return mReliable; }

    /**
     * @brief Add a packet line to the packet that stores an ID. This should be used when a game object has been added and a client/host wants to send that information over the connection.
     * @param id The ID that is being sent in the packet, i.e. the ID of the game object.
     */
    inline void AddLineWithId(const uint64_t id)
    {
        const std::string line = std::to_string(id);

        AddStringToPacket(PacketLineType::ADD_GAME_OBJECT, line);
    }

    /**
     * @brief Add a packet line to the packet that informs the host that a game object should be synchronised (i.e. information about the game object sent every-frame).
     * This should be used on game objects that should be shared across all clients.
     * @param id The ID of the game object that is being synchronised.
     */
    inline void AddSyncObject(const uint64_t id)
    {
        const std::string line = std::to_string(id);

        AddStringToPacket(PacketLineType::SYNC_GAME_OBJECT_ACROSS_NETWORK, line);
    }

    /**
     * @brief Add a packet line to the packet that serialized values of the component. This can then be used to update the component values on the receiving end of the connection.
     * @param id The ID of the game object that contains the component.
     * @param component The name of the component the values are serialized from.
     * @param value The serialized string of values of the component.
     */
    inline void UpdateType(const uint64_t id, const std::string& component, const std::string& value)
    {
        // In the format: 'ID | Component | ComponentValues'
        const std::string line = std::to_string(id) + '|' + component + '|' + value;

        AddStringToPacket(PacketLineType::UPDATE_COMPONENT, line);
    }

    /**
     * @brief Add a packet line to the packet that serialized values of the component. This can then be used to update the component values on the receiving end of the connection.
     * @tparam T The component type that the values are serialized from.
     * @param id The ID of the game object that contains the component.
     * @param value The serialized string of values of the component.
     */
    template<typename T>
    inline void UpdateType(const uint64_t id, const std::string& value)
    {
        UpdateType(id, T::GetNameStatic(), value);
    }

    /**
     * @brief Add a packet line to the packet that represents adding a component to a game object.
     * @param id The ID of the game object that the component is being added to.
     * @param component The name of the component that is being added to the game object.
     * @param value The serialized string of values of the component.
     */
    inline void AddType(const uint64_t id, const std::string& component, const std::string& value)
    {
        // In the format: 'ID | Component | ComponentValues'
        const std::string line = std::to_string(id) + '|' + component + '|' + value;

        AddStringToPacket(PacketLineType::ADD_COMPONENT, line);
    }

    /**
     * @brief Add a packet line to the packet that represents adding a component to a game object.
     * @tparam T The component type that has been added to the game object.
     * @param id The ID of the game object that the component is being added to.
     * @param value The serialized string of values of the component.
     */
    template<typename T>
    inline void AddType(const uint64_t id, const std::string& value)
    {
        AddType(id, T::GetNameStatic(), value);
    }

    /**
     * @brief Add a raw string line to the packet.
     * @param lineType The type the packet line represents.
     * @param string The string that is added to the line.
     */
    inline void AddStringToPacket(const PacketLineType lineType, const std::string& string)
    {
        mBuffer.emplace_back(static_cast<uint8_t>(lineType));
        mBuffer.emplace_back(static_cast<uint8_t>(string.length()));
        mBuffer.insert(mBuffer.end(), string.begin(), string.end());
    }

    Iterator Begin() const { return Iterator{ this, 0 }; }
    Iterator End()   const { return Iterator{ this, static_cast<uint32_t>(mBuffer.size()) }; }

private:
    std::vector<uint8_t> mBuffer;
    bool mReliable = true;
};

} // Namespace d2eNet.
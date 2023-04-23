#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <eosio/crypto.hpp>
#include <optional>

using namespace eosio;
using namespace std;

class [[eosio::contract("iot.taiss")]] iot : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `devices`
     *
     * ### params
     *
     * - `{uint64_t} device_id` - (primary key) IoT Device ID
     * - `{name} authority` - IoT Device Authority
     * - `{string} transmitter_signature` - IoT Device Transmitter Signature
     * - `{string} receiver_signature` - IoT Device Receiver Signature
     * - `{vector<float>} location` - IoT Device Location (x, y, z)
     * - `{float} [temperature]` - IoT Device Temperature
     * - `{time_point_sec} [timestamp]` - IoT Device Timestamp
     * - `{uint64_t} nonce` - IoT Device Nonce
     *
     * ### example
     *
     * ```json
     * {
     *     "device_id": 100000,
     *     "transmitter_signature": "aabbccddeeff/2",
     *     "receiver_signature": "aabbccddeeff/3",
     *     "authority": "device.taiss",
     *     "location": [ 45.4035, -71.8938, 0.0 ],
     *     "temperature": 37.5,
     *     "timestamp": "2023-04-19T00:00:00Z",
     *     "nonce": 10
     * }
     * ```
     */
    struct [[eosio::table("devices")]] devices_row {
        uint64_t                    device_id;
        string                      transmitter_signature;
        string                      receiver_signature;
        name                        authority;
        vector<float>               location;
        optional<float>             temperature;
        optional<time_point_sec>    timestamp;
        uint64_t                    nonce;

        uint64_t primary_key() const { return device_id; }
        checksum256 by_transmitter() const { return to_checksum(transmitter_signature); }
        checksum256 by_receiver() const { return to_checksum(receiver_signature); }
    };
    typedef eosio::multi_index< "devices"_n, devices_row,
        indexed_by<"transmitter"_n, const_mem_fun<devices_row, checksum256, &devices_row::by_transmitter>>,
        indexed_by<"receiver"_n, const_mem_fun<devices_row, checksum256, &devices_row::by_receiver>>
    > devices_table;

    static checksum256 to_checksum( const string signature )
    {
        return sha256(signature.c_str(), signature.length());
    }

    [[eosio::action]]
    void setdevice( const optional<uint64_t> device_id, const optional<name> authority, const optional<string> transmitter_signature, const optional<string> receiver_signature );

    [[eosio::action]]
    void deldevice( const uint64_t device_id );

    [[eosio::action]]
    void temperature( const uint64_t device_id, const float temperature );

    [[eosio::action]]
    void location( const uint64_t device_id, const float x, const float y, const optional<float> z );

private:
    bool has_authority( const uint64_t device_id );
    void check_authority( const uint64_t device_id );
    uint64_t get_device_id( const optional<uint64_t> device_id, const optional<string> transmitter_signature, const optional<string> receiver_signature );
};
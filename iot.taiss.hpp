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

    const set<name> TYPES = { "transmitter"_n, "receiver"_n };

    /**
     * ## TABLE `devices`
     *
     * ### params
     *
     * - `{uint64_t} device_id` - (primary key) IoT Device ID
     * - `{name} type` - IoT Device Type (ex: transmitter, receiver)
     * - `{string} signature` - IoT Device Signature
     * - `{name} authority` - IoT Device Authority
     * - `{time_point_sec} [timestamp]` - IoT Device Timestamp
     * - `{uint64_t} nonce` - IoT Device Nonce
     *
     * ### example
     *
     * ```json
     * {
     *     "device_id": 100000,
     *     "type": "transmitter",
     *     "signature": "aabbccddeeff/2",
     *     "authority": "r.1.taiss",
     *     "timestamp": "2023-04-19T00:00:00Z",
     *     "nonce": 10
     * }
     * ```
     */
    struct [[eosio::table("devices")]] devices_row {
        uint64_t                    device_id; // primary key
        string                      signature;
        name                        type;
        name                        authority;
        optional<time_point_sec>    timestamp;
        uint64_t                    nonce;

        uint64_t primary_key() const { return device_id; }
        checksum256 by_signature() const { return to_checksum(signature); }
    };
    typedef eosio::multi_index< "devices"_n, devices_row,
        indexed_by<"by.signature"_n, const_mem_fun<devices_row, checksum256, &devices_row::by_signature>>
    > devices_table;

    static checksum256 to_checksum( const string signature )
    {
        return sha256(signature.c_str(), signature.length());
    }

    [[eosio::action]]
    void setdevice( const string signature, const name type, const name authority );

    [[eosio::action]]
    void deldevice( const string signature );

    [[eosio::action]]
    void temperature( const string transmitter, const string receiver, const float temperature );

    [[eosio::action]]
    void location( const string transmitter, const string receiver, const vector<float> location );

private:
    bool has_authority( const uint64_t device_id );
    void check_authority( const uint64_t device_id );
    uint64_t get_device_id( const string signature );
    void authenticate_device( const string signature );
};
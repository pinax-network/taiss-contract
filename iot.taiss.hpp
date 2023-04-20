#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
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
     * - `{vector<float>} location` - IoT Device Location (x, y, z)
     * - `{float} temperature` - IoT Device Temperature
     * - `{time_point_sec} timestamp` - IoT Device Timestamp
     * - `{uint64_t} nonce` - IoT Device Nonce
     *
     * ### example
     *
     * ```json
     * {
     *     "device_id": 2199024546082,
     *     "authority": "device.taiss",
     *     "location": [ 45.4035, -71.8938, 0.0 ],
     *     "temperature": 37.5,
     *     "timestamp": "2023-04-19T00:00:00Z",
     *     "nonce": 10
     * }
     * ```
     */
    struct [[eosio::table("devices")]] devices_row {
        uint64_t            device_id;
        name                authority;
        vector<float>       location;
        float               temperature;
        time_point_sec      timestamp;
        uint64_t            nonce;

        uint64_t primary_key() const { return device_id; }
    };
    typedef eosio::multi_index< "devices"_n, devices_row> devices_table;

    [[eosio::action]]
    void setdevice( const uint64_t device_id, const name authority );

    [[eosio::action]]
    void deldevice( const uint64_t device_id );

    [[eosio::action]]
    void temperature( const uint64_t device_id, const float temperature );

    [[eosio::action]]
    void location( const uint64_t device_id, const float x, const float y, const optional<float> z );

private:
    bool has_authority( const uint64_t device_id );
    void check_authority( const uint64_t device_id );
};
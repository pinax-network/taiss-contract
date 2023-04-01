#include <eosio/eosio.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>

using namespace eosio;
using namespace std;

class [[eosio::contract("iot.taiss")]] iot : public eosio::contract {
public:
    using contract::contract;

    /**
     * ## TABLE `devices`
     * 
     * - scope: `{name} account`
     * 
     * ### params
     *
     * - `{uint64_t} device_id` - (primary key) IoT Device ID
     * - `{set<name>} owners` - (secondary key) List of owners
     *
     * ### example
     *
     * ```json
     * {
     *     "device_id": 2199024546082,
     *     "owners": ["device.taiss"]
     * }
     * ```
     */
    struct [[eosio::table("devices")]] devices_row {
        uint64_t            device_id;
        set<name>           owners;

        uint64_t primary_key() const { return device_id; }
    };
    typedef eosio::multi_index< "devices"_n, devices_row> devices_table;

    [[eosio::action]]
    void setdevice( const uint64_t device_id, const set<name> owners );

    [[eosio::action]]
    void deldevice( const uint64_t device_id );

    [[eosio::action]]
    void data( const uint64_t device_id, const float temparature, const float humidity, const float pressure );

    [[eosio::action]]
    void status( const uint64_t device_id, const float battery, const bool connected );

    [[eosio::action]]
    void action( const uint64_t device_id, const string type, const string state );

private:
    bool has_authority( const uint64_t device_id );
    void check_authority( const uint64_t device_id );
};
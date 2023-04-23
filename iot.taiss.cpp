#include "iot.taiss.hpp"

[[eosio::action]]
void iot::setdevice( const optional<uint64_t> device_id, const optional<name> authority, const optional<string> transmitter_signature, const optional<string> receiver_signature )
{
    require_auth( get_self() );
    devices_table _devices( get_self(), get_self().value );

    // validation
    if ( authority ) check( is_account( *authority ), "authority account does not exist" );

    uint64_t set_device_id = get_device_id( device_id, transmitter_signature, receiver_signature );

    // insert or update
    auto insert = [&]( auto & row ) {
        row.device_id = set_device_id;
        if (receiver_signature) row.receiver_signature = *receiver_signature;
        if (transmitter_signature) row.transmitter_signature = *transmitter_signature;
        if (authority) row.authority = *authority;
    };
    auto devices_itr = _devices.find( set_device_id );
    if ( devices_itr == _devices.end() ) _devices.emplace( get_self(), insert );
    else _devices.modify( devices_itr, get_self(), insert );
}

uint64_t iot::get_device_id( const optional<uint64_t> device_id, const optional<string> transmitter_signature, const optional<string> receiver_signature )
{
    if ( device_id ) return *device_id;
    devices_table _devices( get_self(), get_self().value );
    if ( transmitter_signature ) {
        auto idx = _devices.get_index<"transmitter"_n>();
        auto it = idx.find(to_checksum(*transmitter_signature));
        return it->device_id;
    }
    if ( receiver_signature ) {
        auto idx = _devices.get_index<"receiver"_n>();
        auto it = idx.find(to_checksum(*receiver_signature));
        return it->device_id;
    }
    if ( _devices.begin() == _devices.end() ) return 100'000; // 100K start of default device_id
    return _devices.available_primary_key();
}

[[eosio::action]]
void iot::deldevice( const uint64_t device_id )
{
    require_auth( get_self() );
    devices_table _devices( get_self(), get_self().value );

    // validation
    auto devices_itr = _devices.find( device_id );
    check( devices_itr != _devices.end(), "device not found" );

    // delete
    _devices.erase( devices_itr );
}

[[eosio::action]]
void iot::temperature( const uint64_t device_id, const float temperature )
{
    check_authority( device_id );
    auto devices = devices_table( get_self(), get_self().value );
    auto &itr = devices.get( device_id, "device not found" );
    devices.modify( itr, get_self(), [&]( auto & row ) {
        row.temperature = temperature;
        row.timestamp = current_time_point();
        row.nonce += 1;
    });
}

[[eosio::action]]
void iot::location( const uint64_t device_id, const float x, const float y, const optional<float> z )
{
    check_authority( device_id );
    auto devices = devices_table( get_self(), get_self().value );
    auto &itr = devices.get( device_id, "device not found" );
    devices.modify( itr, get_self(), [&]( auto & row ) {
        row.location = { x, y };
        if ( z ) row.location.push_back( *z );
        row.timestamp = current_time_point();
        row.nonce += 1;
    });
}

bool iot::has_authority( const uint64_t device_id )
{
    devices_table _devices( get_self(), get_self().value );
    auto itr = _devices.get( device_id, "device not found" );
    return has_auth(itr.authority);
}

void iot::check_authority( const uint64_t device_id )
{
    check( has_authority(device_id), "device missing required authority" );
}

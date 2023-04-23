#include "iot.taiss.hpp"

[[eosio::action]]
void iot::setdevice( const string signature, const name type, const name authority )
{
    require_auth( get_self() );
    devices_table _devices( get_self(), get_self().value );

    // validation
    if ( authority ) check( is_account( authority ), "authority account does not exist" );
    check( TYPES.find( type ) != TYPES.end(), "invalid device type (ex: transmitter, receiver)" );

    // primary key device_id
    const uint64_t device_id = get_device_id( signature );

    // insert or update
    auto insert = [&]( auto & row ) {
        row.device_id = device_id;
        row.type = type;
        row.signature = signature;
        row.authority = authority;
    };
    auto devices_itr = _devices.find( device_id );
    if ( devices_itr == _devices.end() ) _devices.emplace( get_self(), insert );
    else _devices.modify( devices_itr, get_self(), insert );
}

uint64_t iot::get_device_id( const string signature )
{
    devices_table _devices( get_self(), get_self().value );

    // no devices exists
    if ( _devices.begin() == _devices.end() ) return 100'000;

    // Lookup device_id by signature using secondary index
    auto idx = _devices.get_index<"by.signature"_n>();
    auto it = idx.find(to_checksum(signature));

    // no signature found
    if (it == idx.end() ) return _devices.available_primary_key();

    // signature exists
    return it->device_id;
}

[[eosio::action]]
void iot::deldevice( const string signature )
{
    require_auth( get_self() );
    devices_table _devices( get_self(), get_self().value );

    // validation
    const uint64_t device_id = get_device_id( signature );
    auto devices_itr = _devices.find( device_id );
    check( devices_itr != _devices.end(), "device signature not found" );

    // delete
    _devices.erase( devices_itr );
}

[[eosio::action]]
void iot::temperature( const string transmitter, const string receiver, const float temperature )
{
    authenticate_device( transmitter );
    authenticate_device( receiver );
}

[[eosio::action]]
void iot::location( const string transmitter, const string receiver, const vector<float> location )
{
    authenticate_device( transmitter );
    authenticate_device( receiver );
}

bool iot::has_authority( const uint64_t device_id )
{
    devices_table _devices( get_self(), get_self().value );
    auto itr = _devices.get( device_id, "device signature not found" );
    return has_auth(itr.authority);
}

void iot::check_authority( const uint64_t device_id )
{
    check( has_authority(device_id), "device signature missing required authority" );
}

void iot::authenticate_device( const string signature )
{
    auto devices = devices_table( get_self(), get_self().value );

    // authority
    const uint64_t device_id = get_device_id( signature );
    check_authority( device_id );

    // update device
    auto &itr = devices.get( device_id, "device signature not found" );
    devices.modify( itr, get_self(), [&]( auto & row ) {
        row.timestamp = current_time_point();
        row.nonce += 1;
    });
}
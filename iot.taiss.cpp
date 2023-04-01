#include "iot.taiss.hpp"

[[eosio::action]]
void iot::setdevice( const uint64_t device_id, const name authority )
{
    require_auth( get_self() );
    devices_table _devices( get_self(), get_self().value );

    // validation
    check( is_account( authority ), "authority account does not exist" );

    // insert or update
    auto insert = [&]( auto & row ) {
        row.device_id = device_id;
        row.authority = authority;
    };
    auto devices_itr = _devices.find( device_id );
    if ( devices_itr == _devices.end() ) _devices.emplace( get_self(), insert );
    else _devices.modify( devices_itr, get_self(), insert );
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
void iot::data( const uint64_t device_id, const float temparature, const float humidity, const float pressure )
{
    check_authority( device_id );
}


[[eosio::action]]
void iot::status( const uint64_t device_id, const float battery, const bool connected )
{
    check_authority( device_id );
}

[[eosio::action]]
void iot::action( const uint64_t device_id, const string type, const string state )
{
    check_authority( device_id );
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

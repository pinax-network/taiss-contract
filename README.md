# `TAISS` Project - **IoT** smart contract
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/pinax-network/taiss-contract/blob/main/LICENSE-MIT)
[![Antelope CDT](https://github.com/pinax-network/taiss-contract/actions/workflows/release.yml/badge.svg)](https://github.com/pinax-network/taiss-contract/actions/workflows/release.yml)
[![Blanc++ Vert](https://github.com/pinax-network/taiss-contract/actions/workflows/ci.yml/badge.svg)](https://github.com/pinax-network/taiss-contract/actions/workflows/ci.yml)

> IoT device interactions to on-chain events.

## Related

- [TAISS Substreams](https://github.com/pinax-network/taiss-substreams)

## Events

| Event | Description |
| --- | --- |
| **Device Authority** | where the device is registered on the blockchain with a public key and a signature. |
| **Control Actions** | where the device transmitter & receiver send temperature and location data to the blockchain. |

## Diagram

```mermaid
graph TD;
  actions[TAISS Control Actions]
  device[TAISS Device Authority]
  device --> setdevice
  actions --> temperature
  actions --> location

  eos[EOS Blockchain] --> newaccount
  newaccount --> account[EOS Account]
  authority --> account[account]
  account --> public_key[Public Key]

  setdevice[setdevice]
  setdevice --> transmitter
  setdevice --> receiver
  setdevice --> authority[authority]

  temperature[temperature]
  temperature --> temp[Temp C.]

  location[location]
  location --> geometry[location=x,y,z]
```

## Payload `JSON` Format

Here are some JSON examples of payloads for each of these actions:

#### Temperature

```json
{
  "transmitter": "aabbccddeeff/2",
  "receiver": "aabbccddeeff/3",
  "temperature": 25.5
}
```

#### Location

```json
{
  "transmitter": "aabbccddeeff/2",
  "receiver": "aabbccddeeff/3",
  "location": [45.4035, -71.8938, 0]
}
```

## TABLE `devices`

### params

- `{uint64_t} device_id` - (primary key) IoT Device ID
- `{name} type` - IoT Device Type (ex: transmitter, receiver)
- `{string} signature` - IoT Device Signature
- `{name} authority` - IoT Device Authority
- `{time_point_sec} [timestamp]` - IoT Device Timestamp
- `{uint64_t} nonce` - IoT Device Nonce

### example

```json
{
    "device_id": 100000,
    "type": "transmitter",
    "signature": "aabbccddeeff/2",
    "authority": "device.taiss",
    "timestamp": "2023-04-19T00:00:00Z",
    "nonce": 10
}
```

## Actions

```bash
# Setup Device
$ cleos push action iot.taiss setdevice '["aabbccddeeff/2", "transmitter", device.taiss]' -p iot.taiss
$ cleos push action iot.taiss setdevice '["aabbccddeeff/3", "receiver", device.taiss]' -p iot.taiss

# Temperature
$ cleos push action iot.taiss temperature '["aabbccddeeff/2", "aabbccddeeff/3", 25.5]' -p device.taiss

# Location
$ cleos push action iot.taiss location '["aabbccddeeff/2", "aabbccddeeff/3", [45.4035, -71.8938, 0.0]]' -p device.taiss

# Delete Device
$ cleos push action iot.taiss deldevice '["aabbccddeeff/2"]' -p iot.taiss
```

## Decentralized Identifiers

- [EPC](https://en.wikipedia.org/wiki/Electronic_Product_Code)
- [MAC-48](https://en.wikipedia.org/wiki/MAC_address)
- [DID](https://www.w3.org/TR/did-core)
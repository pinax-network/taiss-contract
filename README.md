# `TAISS` Project - **IoT** smart contract
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/pinax-network/taiss-contract/blob/main/LICENSE-MIT)
[![Antelope CDT](https://github.com/pinax-network/taiss-contract/actions/workflows/release.yml/badge.svg)](https://github.com/pinax-network/taiss-contract/actions/workflows/release.yml)
[![Blanc++ Vert](https://github.com/pinax-network/taiss-contract/actions/workflows/ci.yml/badge.svg)](https://github.com/pinax-network/taiss-contract/actions/workflows/ci.yml)

> IoT device interactions to on-chain events.

## Events

| Event | Description |
| --- | --- |
| **Data Collection** | where the device collects data from its sensors and sends it to a server or cloud for further analysis.
| **Status Updates** | where the device sends information about its current status or state, such as battery life or connectivity.
| **Control Actions** | where the device receives a command from a user or server to perform an action, such as turning on a light or unlocking a door.

## Payload `JSON` Format

Here are some JSON examples of payloads for each of these actions:

#### Temperature

```json
{
  "device_id": 901536379396317224,
  "temperature": 25.5
}
```

#### Location

```json
{
  "device_id": 901536379396317224,
  "x": 45.4035,
  "y": -71.8938,
  "z": 0
}
```

#### Status Updates

```json
{
  "device_id": 901536379396317224,
  "battery": 0.5,
  "connected": true
}
```

#### Control Actions

```json
{
  "device_id": 901536379396317224,
  "type": "light",
  "state": "on"
}
```

## Tables

## TABLE `devices`

### params

- `{uint64_t} device_id` - (primary key) IoT Device ID
- `{name} authority` - IoT Device Authority

### example

```json
{
  "device_id": 2199024546082,
  "authority": "device.taiss"
}
```

## Actions

```bash
# Setup Device
$ cleos push action iot.taiss setdevice '[901536379396317224, device.taiss]' -p iot.taiss
$ cleos push action iot.taiss deldevice '[901536379396317224]' -p iot.taiss

# Temperature
$ cleos push action iot.taiss temperature '[901536379396317224, 25.5]' -p device.taiss

# Location
$ cleos push action iot.taiss location '[901536379396317224, 45.4035, -71.8938, 0.0]' -p device.taiss

# Status Updates
$ cleos push action iot.taiss status '[901536379396317224, 0.5, true]' -p device.taiss

# Control Actions
$ cleos push action iot.taiss action '[901536379396317224, "light", "on"]' -p device.taiss
```

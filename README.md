# `TAISS` Project **IoT** Smart Contract
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/pinax-network/iot.taiss/blob/main/LICENSE-MIT)
[![Antelope CDT](https://github.com/pinax-network/iot.taiss/actions/workflows/release.yml/badge.svg)](https://github.com/pinax-network/iot.taiss/actions/workflows/release.yml)
[![Blanc++ Vert](https://github.com/pinax-network/iot.taiss/actions/workflows/ci.yml/badge.svg)](https://github.com/pinax-network/iot.taiss/actions/workflows/ci.yml)

> IoT device interactions to on-chain events.

## Events

| Event | Description |
| --- | --- |
| **Data Collection** | where the device collects data from its sensors and sends it to a server or cloud for further analysis.
| **Status Updates** | where the device sends information about its current status or state, such as battery life or connectivity.
| **Control Actions** | where the device receives a command from a user or server to perform an action, such as turning on a light or unlocking a door.

## Payload `JSON` Format

Here are some JSON examples of payloads for each of these actions:

#### Data Collection

```json
{
  "device_id": 901536379396317224,
  "data": {
    "temperature": 25.5,
    "humidity": 60.0,
    "pressure": 1013.0
  }
}
```

#### Status Updates

```json
{
  "device_id": 901536379396317224,
  "status": {
    "battery": 0.5,
    "connected": true
  }
}
```

#### Control Actions

```json
{
  "device_id": 901536379396317224,
  "action": {
    "type": "light",
    "state": "on"
  }
}
```

## Actions

```bash
# Setup Device
$ cleos push action iot.taiss setdevice '[90153637939631722, [device.taiss]]' -p iot.taiss
$ cleos push action iot.taiss deldevice '[90153637939631722]' -p iot.taiss

# Data Collection
$ cleos push action iot.taiss data '[901536379396317224, 25.5, 60.0, 1013.0]' -p device.taiss

# Status Updates
$ cleos push action iot.taiss status '[901536379396317224, 0.5, true]' -p device.taiss

# Control Actions
$ cleos push action iot.taiss action '[901536379396317224, "light", "on"]' -p device.taiss
```

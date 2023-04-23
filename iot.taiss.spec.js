import { TimePointSec, Name } from "@greymass/eosio";
import { Blockchain } from "@proton/vert"
import { it, describe, beforeEach } from "node:test";
import assert from 'node:assert';

// Vert EOS VM
const blockchain = new Blockchain()

// contracts
const contract = blockchain.createContract('iot.taiss', 'iot.taiss', true);

blockchain.createAccounts('device.taiss');

// one-time setup
beforeEach(async () => {
  blockchain.setTime(TimePointSec.from("2023-03-31T00:00:00.000"));
});

function get_device(device_id) {
  const scope = Name.from('iot.taiss').value.value;
  return contract.tables.devices(scope).getTableRow(BigInt(String(device_id)));
}

function set_device(device) {
  return contract.actions.setdevice([device.signature, device.type, device.authority]).send();
}

describe('iot.taiss', () => {
  const authority = 'device.taiss';
  const transmitter = {
    signature:'aabbccddeeff/2',
    type: "transmitter",
    authority,
    device_id: 100000,
  }
  const receiver = {
    signature: 'aabbccddeeff/3',
    type: "receiver",
    authority,
    device_id: 100001,
  }

  it("setdevice", async () => {
    await set_device(transmitter);
    await set_device(receiver);
    const device = get_device(transmitter.device_id);
    assert.equal(transmitter.authority, device.authority);
    assert.equal(transmitter.signature, device.signature);
    assert.equal(transmitter.type, device.type);
    assert.equal(transmitter.device_id, device.device_id);
  });

  it("temperature", async () => {
    await contract.actions.temperature([transmitter.signature, receiver.signature, 25.5]).send(authority);
    await contract.actions.temperature([transmitter.signature, receiver.signature, 23.0]).send(authority);
    await contract.actions.temperature([transmitter.signature, receiver.signature, 15.3]).send(authority);
  });

  it("location", async () => {
    await contract.actions.location([transmitter.signature, receiver.signature, [45.4035, -71.8938, 0.0]]).send(authority);
    await contract.actions.location([transmitter.signature, receiver.signature, [45.4035, -71.8938]]).send(authority);
  });

  it("error: account does not exists", async () => {
    const action = contract.actions.setdevice(['aabbccddeeff/4', "transmitter", "invalid"]).send();
    await expectToThrow(action, /authority account does not exist/);
  });

  it("error: invalid type", async () => {
    const action = contract.actions.setdevice(['aabbccddeeff/4', "foobar", "device.taiss"]).send();
    await expectToThrow(action, /invalid device type \(ex: transmitter, receiver\)/);
  });
});

/**
 * Expect a promise to throw an error with a specific message.
 * @param promise - The promise to await.
 * @param {string} errorMsg - The error message that we expect to see.
 */
const expectToThrow = async (promise, errorMsg) => {
  try {
    await promise
    assert.fail('Expected promise to throw an error');
  } catch (e) {
    if ( errorMsg ) assert.match(e.message, errorMsg);
    else assert.fail('Expected promise to throw an error');
  }
}

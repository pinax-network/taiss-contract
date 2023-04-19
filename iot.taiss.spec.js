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
  return contract.tables.devices(scope).getTableRow(BigInt(device_id));
}

describe('iot.taiss', () => {
  const device_id = "901536379396317224";
  const authority = "device.taiss";

  it("setdevice", async () => {
    await contract.actions.setdevice([device_id, authority]).send();
    assert.deepEqual(get_device(device_id), {
      device_id,
      authority,
    })
  });

  it("temperature", async () => {
    await contract.actions.temperature([device_id, 25.5]).send(authority);
    await contract.actions.temperature([device_id, 23.0]).send(authority);
    await contract.actions.temperature([device_id, 15.3]).send(authority);
  });

  it("location", async () => {
    await contract.actions.location([device_id, 45.4035, -71.8938, 0.0]).send(authority);
    await contract.actions.location([device_id, 45.4035, -71.8938, null]).send(authority);
  });

  it("error: account does not exists", async () => {
    const action = contract.actions.setdevice([device_id, "invalid"]).send();
    await expectToThrow(action, /authority account does not exist/);
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

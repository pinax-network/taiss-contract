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
  return contract.tables.users(scope).getTableRow(BigInt(device_id));
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

  it("data", async () => {
    await contract.actions.data([device_id, 25.5, 60.0, 1013.0]).send();
    await contract.actions.data([device_id, 23.0, 50.0, 1041.0]).send();
    await contract.actions.data([device_id, 15.3, 70.0, 900.0]).send();
  });

  it("error: account does not exists", async () => {
    const action = contract.actions.setdevice([device_id, "invalid"]).send();
    await expectToThrow(action, /owner account does not exist/);
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

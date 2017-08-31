const child_process = require("child_process")
const chai = require('chai')

class BinaryManager {
    constructor() {

    }
    startBinary() {
        this.binary = child_process.spawn("../../bin/src/AquaLampServer.exe");
        this.binary.on('close', (code) => {
            if (code) {
                chai.assert.fail(0, code, "Binary crashed!");
            }
        });
    }

    stopBinary() {
        this.binary.kill();
    }
}

exports.BinaryManager = BinaryManager;

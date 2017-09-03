const child_process = require("child_process")
const chai = require('chai')
const fs = require('fs')

class BinaryManager {
    constructor() {

    }
    startBinary() {
        this.log = fs.createWriteStream('../../bin/src/target.log');
        this.binary = child_process.spawn("../../bin/src/AquaLampServer.exe");

        this.binary.stdout.pipe(this.log);
        this.binary.stderr.pipe(this.log);


        this.binary.on('uncaughtException', function (err) {
            console.error((err && err.stack) ? err.stack : err);
        });
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

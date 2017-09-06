const child_process = require("child_process");
const chai = require('chai');
const fs = require('fs');
const os = require('os');

if (os.platform == "win32") {
    var settings = require("../settings_windows.json");
}

class BinaryManager {
    constructor() {

    }

    startBinary() {
        this.binary = child_process.spawn(settings.binary.path);
        this.binary.on('close', (code) => {
            if (code) {
                chai.assert.fail(0, code, "Binary crashed!");
            }
        });

        this.binary.stdout.on('data', function (data) {
            console.log(data);
        });
        this.binary.stderr.on('data', function (data) {
            console.log(data);
        });
    }

    stopBinary() {
        this.binary.kill();
    }
}

exports.BinaryManager = BinaryManager;

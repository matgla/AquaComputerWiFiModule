const child_process = require("child_process")

class BinaryManager {
    constructor() {

    }
    startBinary() {
        this.binary = child_process.spawn("../../../build/src/AquaLampServer.exe");
    }

    stopBinary() {
        console.log(process.platform);
        if ("win32" === process.platform) {
            console.log("my kill");
            child_process.spawn("taskkill", ["/pid", this.binary.pid, "/f", "/t"]);
        } else {
            this.binary.kill();
        }
    }
}

exports.BinaryManager = BinaryManager;
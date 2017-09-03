"use strict";

var expect = require("chai").expect;
var sleep = require("sleep").sleep;
var net = require("net");
var SerialPort = require("serialport");
var fifo = require("fifo");

var BinaryManager = require("../framework/binaryManager").BinaryManager;

function parseData(data) {
    console.log("mam komplet");
    var buf = new Buffer(data);
    console.log(buf.toString("ascii"));
    var object = JSON.parse(buf);

    console.log(object.id);

    send(buf.toString("ascii"));
}

var queue = fifo();

function send(data) {
    queue.push(data);
}

describe("TEst", function () {
    it("Empty", function (done) {
        var port = new SerialPort("COM4", { baudRate: 9600 }, function (err) {
            if (err) {
                return console.log("Error while opening port: ", err.message);
            }
        });

        var transmissionOngoing = false;
        var receivedLength = false;
        var lengthToBeReceived = 0;
        const LENGTH_SIZE = 8;
        var length = 0;
        var buffer = [];
        port.on('data', function (data) {
            for (var byte = 0; byte < data.length; byte++) {
                if (data[byte] == 0x10 && !transmissionOngoing) {
                    port.write([0x20]);
                    transmissionOngoing = true;
                    lengthToBeReceived = 8;
                    receivedLength = false;
                    length = 0;
                    continue;
                }

                if (data[byte] == 0x20 && !transmissionOngoing) {
                    var data = queue.pop();

                    var buf = [];

                    for (var i = 0; i < data.length; i++) {
                        buf.push(data.length >> i * 8);
                    }

                    port.write(buf);
                    port.write(data);
                }

                if (!receivedLength) {
                    length |= data[byte] << 8 * (LENGTH_SIZE - lengthToBeReceived);
                    --lengthToBeReceived;
                    if (lengthToBeReceived == 0) {
                        receivedLength = true;
                        console.log("Bytes to be received:", length.toString());
                    }
                    continue;
                }

                if (length) {
                    buffer.push(data[byte]);
                    --length;
                    if (length == 0) {
                        transmissionOngoing = false;
                        parseData(buffer);
                    }
                }
            }
        });

        port.on('readable', function () {
            console.log('Data:', port.read());
        });
        // The open event is always emitted
        port.on('open', function () {
            console.log("Opened");
        });

        var bm = new BinaryManager();

        bm.startBinary();
        setTimeout(function () {
            bm.stopBinary();
            done();
        }, 2000);
        //bm.stopBinary();
    })
})

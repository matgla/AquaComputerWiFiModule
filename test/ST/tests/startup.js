"use strict";

var expect = require("chai").expect;
var sleep = require("sleep").sleep;
var net = require("net");
var SerialPort = require("serialport");

var BinaryManager = require("../framework/binaryManager").BinaryManager;

describe("Startup", function () {
    it("Successful", function (done) {
        // var port = new SerialPort("COM2", { baudRate: 9600 }, function (err) {
        //     if (err) {
        //         return console.log("Error while opening port: ", err.message);
        //     }
        // });

        // port.on('data', function (data) {
        //     console.log("data: ", data.toString("utf8"));
        // });

        // port.on('readable', function () {
        //     console.log('Data:', port.read());
        // });
        // // The open event is always emitted 
        // port.on('open', function () {
        //     console.log("Opened");
        // });

        var bm = new BinaryManager();

        bm.startBinary();
        setTimeout(function () {
            bm.stopBinary();
            done();
        }, 100);
        //bm.stopBinary();
    })
})

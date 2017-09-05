"use strict";

var expect = require("chai").expect;
var sleep = require("sleep").sleep;
var net = require("net");

var BinaryManager = require("../framework/binaryManager").BinaryManager;
var Serial = require("../framework/serial").Serial;

describe("TEst", function () {
    it("Empty", function (done) {
        var serial = new Serial();

        var bm = new BinaryManager();

        bm.startBinary();
        setTimeout(function () {
            bm.stopBinary();
            serial.close();
            done();
        }, 2000);
        //bm.stopBinary();
    })
})

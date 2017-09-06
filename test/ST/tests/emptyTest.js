"use strict";

var expect = require("chai").expect;
var sleep = require("sleep").sleep;
var net = require("net");
var Promise = require("bluebird");

var BinaryManager = require("../framework/binaryManager").BinaryManager;
var Serial = require("../framework/serial").Serial;

describe("TEst", function () {
    var serial = new Serial();
    var bm = new BinaryManager();

    afterEach(() => {
        serial.close();
        bm.stopBinary();
    })

    it("Empty", function (done) {
        serial.expect({ "id": "handshake" }, () => { done() });

        setTimeout(() => { done() }, 1500);
        bm.startBinary();
    })
})

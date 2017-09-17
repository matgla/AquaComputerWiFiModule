"use strict";

var expect = require("chai").expect;
var sleep = require("sleep").sleep;
var net = require("net");
var Promise = require("bluebird");
const chai = require("chai");

var BinaryManager = require("../framework/binaryManager").BinaryManager;
var Serial = require("../framework/serial").Serial;

describe("DetectionShould", function () {
    var serial = new Serial();
    var bm = new BinaryManager();

    beforeEach(() => {
        setTimeout(() => { chai.assert.fail(0, 1, "Test timeouted!"); done() }, 1500);
    })

    afterEach(() => {
        serial.close();
        bm.stopBinary();
    })

    it("DetectMcu", function (done) {
        serial.expect({ "id": "handshake" }, () => {
            serial.send({ "id": "handshake" });
            serial.send({ "id": "get_info" })
        });

        serial.expect({ id: "module_info" }, () => { done(); }).setComparator((expected, received) => {
            return expected.id == received.id;
        });

        bm.startBinary();
    })
})

"use strict";

var expect = require("chai").expect;
var sleep = require("sleep").sleep;

var BinaryManager = require("../framework/binaryManager").BinaryManager;

describe("Test", function () {
    it("Empty", function () {
        var bm = new BinaryManager();
        bm.startBinary();
        sleep(1);
        bm.stopBinary();
    })
})
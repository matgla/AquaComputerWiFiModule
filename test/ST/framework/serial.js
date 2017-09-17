"use strict";

var SerialPort = require("serialport");
var log4js = require("log4js");
var fifo = require("fifo")
const chai = require('chai');
const _ = require("underscore");
const Promise = require("bluebird");
const os = require("os")

if (os.platform() == "win32") {
    var settings = require("../settings_windows.json");
}
var messages = require("./messages.js");

var logger = log4js.getLogger("serial");
logger.level = settings.logger.level;

class Expectation {
    constructor(msg, fn, inSequence, comparator) {
        this.message = msg;
        this.callback = fn;
        this.inSequence = inSequence;
        this.timesToBeCalled = 1;
        this.comparator = comparator;
    }

    setComparator(comparator) {
        this.comparator = comparator;
    }

    times(times_) {
        this.timesToBeCalled = times_;
    }
}

class Serial {
    constructor() {
        this.serial = new SerialPort(settings.serial.port, { baudRate: settings.serial.baudrate }, function (err) {
            if (err) {
                return console.log("Error while opening port: ", err.message);
            }
        });

        this.transmission = {};
        this.transmission.ongoing = false;
        this.transmission.isMessageLengthKnown = false;
        this.transmission.length = 0;
        this.transmission.receivedBytes = 0;
        this.transmission.buffer = [];
        this.messageQueue = fifo();

        var self = this;
        this.serial.on('data', function (data) {
            self.onData(data);
        });

        this.serial.on('open', function () {
            logger.info("Serial port " + settings.serial.port + " opened with baudrate: " + settings.serial.baudrate.toString());
        });

        this.expectations = [];

    }

    parseData() {
        var buffer = new Buffer(this.transmission.buffer);
        logger.trace("Message buffer: ", buffer.toString("ascii"));
        var msg = JSON.parse(buffer);
        logger.debug("Received message:", msg.id);

        chai.assert(this.expectations.length != 0, "Unexpected call: " + JSON.stringify(msg) + ". Any expectations was set.");

        if (this.expectations[0].comparator(this.expectations[0].message, msg)) {
            if (this.expectations[0].timesToBeCalled == 1) {
                var expectation = this.expectations.shift();
                expectation.callback(msg);
            } else {
                this.expectations[0].timesToBeCalled--;
                var expectation = this.expectations[0];
                expectation.callback(msg);
            }
        }
        else if (index = _.findIndex_(this.expectations[0], function (expectation) {
            return expectation.comparator(expectation.message, msg) && expectation.inSequence === false;
        })) {
            if (expectation.times == 1) {
                this.expectations.splice(index, 1);
                expectation.callback(msg);
            } else {
                this.expectations[index].timesToBeCalled--;
                expectation.callback(msg);
            }
        }
        else {
            chai.assert.fail(false, true, "Unexpected call: " + JSON.stringify(msg));
        }
    }

    onData(data) {
        for (var byte = 0; byte < data.length; byte++) {
            if (data[byte] == messages.TransmissionMessages.Start && !this.transmission.ongoing) {
                this.transmission.ongoing = true;
                this.transmission.isMessageLengthKnown = false;
                this.transmission.length = 0;
                this.transmission.receivedBytes = 0;
                this.transmission.buffer = [];

                this.serial.write([messages.TransmissionMessages.Ack]);
                continue;
            }

            if (data[byte] == messages.TransmissionMessages.Ack && !this.transmission.ongoing) {
                var message = this.messageQueue.pop();

                var lengthBuffer = [];

                for (var i = 0; i < 4; i++) {
                    lengthBuffer.push((message.length >>> i * 8) & 0xff);
                }

                for (var i = 0; i < 4; i++) {
                    lengthBuffer.push(0);
                }

                logger.trace("sending data: " + lengthBuffer.toString());
                this.serial.write(lengthBuffer);
                this.serial.write(message);
            }

            if (!this.transmission.isMessageLengthKnown) {
                this.transmission.length |= data[byte] << 8 * this.transmission.receivedBytes;
                ++this.transmission.receivedBytes;

                if (this.transmission.receivedBytes == 8) {
                    this.transmission.isMessageLengthKnown = true;
                    logger.trace("Bytes to be received: ", this.transmission.length.toString());
                }
                continue;
            }

            if (this.transmission.length) {
                this.transmission.buffer.push(data[byte]);
                --this.transmission.length;
                ++this.transmission.receivedBytes;
                if (this.transmission.length == 0) {
                    this.transmission.ongoing = false;
                    this.parseData();
                    this.write();
                }
            }
        }
    }

    expect(msg, fn = function (data) { }, inSequence = true, comparator = _.isEqual) {
        var expectation = new Expectation(msg, fn, inSequence, comparator);

        this.expectations.push(expectation);
        return expectation;

    }

    verify() {
        chai.assert(this.expectations.length == 0, "Unresolved expectations: " + JSON.stringify(this.expectations));
    }

    close() {
        this.verify();
        this.serial.close();
    }

    send(msg) {
        this.messageQueue.push(JSON.stringify(msg));
        this.write();
    }

    write() {
        if (!this.transmission.ongoing) {
            if (this.messageQueue.length > 0) {
                this.serial.write([messages.TransmissionMessages.Start]);
            }
        }
    }
}

exports.Serial = Serial;

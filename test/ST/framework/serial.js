"use strict";

var SerialPort = require("serialport");
var log4js = require("log4js");
var fifo = require("fifo")

var settings = require("../settings_windows.json")
var messages = require("./messages.js");

var logger = log4js.getLogger("serial");
logger.level = settings.logger.level;

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

        this.expectation_sequence = [];

    }

    parseData() {
        var buffer = new Buffer(this.transmission.buffer);
        logger.trace("Message buffer: ", buffer.toString("ascii"));

        var msg = JSON.parse(buffer);

        logger.debug("Received message:", msg.id);
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

                logger.trace("sending data: " + buf.toString());
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
                }
            }
        }
    }

    expect(msg) {
        this.expectation_sequence.push(msg);
    }

    verify() {
        // check if all expected messages arrived
    }

    close() {
        this.serial.close();
    }
}

exports.Serial = Serial;

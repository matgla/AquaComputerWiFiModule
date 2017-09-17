"use strict";
var log4js = require("log4js");
var settings = require("../settings_windows.json");

log4js.configure({
    appenders: { default: { type: 'file', filename: 'test.log', append: false } },
    categories: {
        default: {
            appenders: ["default"],
            level: "debug"
        }
    }
});

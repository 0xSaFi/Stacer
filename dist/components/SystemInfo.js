"use strict";

Object.defineProperty(exports, "__esModule", {
  value: true
});

var _systeminformation = require("systeminformation");

var _systeminformation2 = _interopRequireDefault(_systeminformation);

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

exports.default = {
  template: "<div class=\"system-info\">SYSTEM INFO\n                <ul> <li v-for=\"info in information\"> {{ info }} </li> </ul>\n              </div>",
  data: function data() {
    return {
      information: []
    };
  },
  created: function created() {
    var _this = this;

    _systeminformation2.default.osInfo(function (o) {
      _this.information.push("Hostname: " + o.hostname);
      _this.information.push("Platform: " + o.platform + " " + o.arch);
      _this.information.push("Distribution: " + o.distro + " " + o.release);
      _this.information.push("Kernel Release: " + o.kernel);
      _systeminformation2.default.cpu(function (c) {
        _this.information.push("Cpu Model: " + c.manufacturer + " " + c.brand);
        _this.information.push("Cpu Speed: " + c.speed + "GHz");
        _this.information.push("Cpu Cores: " + c.cores);
      });
    });
  }
};
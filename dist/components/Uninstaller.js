'use strict';

Object.defineProperty(exports, "__esModule", {
    value: true
});

var _child_process = require('child_process');

var _sudoPrompt = require('sudo-prompt');

var _sudoPrompt2 = _interopRequireDefault(_sudoPrompt);

var _config = require('../config');

var _helpers = require('../helpers');

function _interopRequireDefault(obj) { return obj && obj.__esModule ? obj : { default: obj }; }

function _toConsumableArray(arr) { if (Array.isArray(arr)) { for (var i = 0, arr2 = Array(arr.length); i < arr.length; i++) { arr2[i] = arr[i]; } return arr2; } else { return Array.from(arr); } }

exports.default = {
    template: '<div id="uninstaller-table">\n\t\t\t\t<div id="installed-packages-title">\n\t\t\t\t\t<span style="padding:0;">System Installed Packages ({{ filteredPackages.length }})</span>\n\t\t\t\t\t<input type="text" id="packages-search" v-model="searchString" placeholder="Search..." />\n\t\t\t\t</div>\n\t\t\t\t<div class="tdl-content scroll">\n\t\t\t\t\t<ul>\n\t\t\t\t\t\t<li v-for="package in filteredPackages" > {{ package }}\n\t\t\t\t\t\t\t<a :name="package" @click="removePackage"></a>\n\t\t\t\t\t\t</li>\n\t\t\t\t\t</ul>\n\t\t\t\t</div>\n\t\t\t</div>',
    data: function data() {
        return {
            packagesList: [],
            searchString: '',
            isBusy: false
        };
    },
    created: function created() {
        var _this = this;

        var packages = (0, _child_process.spawn)('bash', ['-c', _config.commands.getInstalledPackages]);

        packages.stdout.on('data', function (data) {
            var _packagesList;

            _this.packagesList.splice(0, _this.packagesList.length);
            data = data.toString().split('\n').filter(function (s) {
                return s != '';
            });

            (_packagesList = _this.packagesList).push.apply(_packagesList, _toConsumableArray(data));
        });
    },

    methods: {
        removePackage: function removePackage(e) {
            var _this2 = this;

            if (!this.isBusy) {
                this.isBusy = true;
                e.target.className += 'loader';
                var packageName = e.target.name;

                _sudoPrompt2.default.exec((0, _helpers.command)(_config.commands.removePackage + packageName), { name: 'Stacer' }, function (error, stdout, stderr) {
                    if (stderr) {
                        e.target.className = '';
                        (0, _helpers.showMessage)('Operation not successful.', 'error');
                    } else {
                        _this2.searchString = e.target.className = '';
                        var i = _this2.packagesList.indexOf(packageName);
                        if (i != -1) _this2.packagesList.splice(i, 1);

                        (0, _helpers.showMessage)(packageName + ' package uninstalled.', 'success');
                    }
                    _this2.isBusy = false;
                });
            } else {
                (0, _helpers.showMessage)('Another process continues.', 'error');
            }
        }
    },
    computed: {
        filteredPackages: function filteredPackages() {
            var tempPackagesList = this.packagesList,
                searchString = this.searchString;

            if (!searchString) return tempPackagesList;

            searchString = searchString.toString().trim().toLowerCase();

            return tempPackagesList.filter(function (item) {
                return item.trim().toLowerCase().indexOf(searchString) !== -1;
            });
        }
    }
};
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

exports.default = {
	template: '<div id="system-service-table">\n\t\t\t\t<div id="system-service-title">\n\t\t\t\t\t<span style="padding:0;">System Services ({{ filteredServices.length }})</span>\n\t\t\t\t\t<input type="text" id="system-service-search" v-model="searchString" placeholder="Search..." />\n\t\t\t\t</div>\n\t\t\t\t<div class="tdl-content scroll">\n\t\t\t\t\t<span class="fl w100 empty-list" v-show="! filteredServices.length" >\n\t\t\t\t\t\tNo service found.\n\t\t\t\t\t</span>\n\t\t\t\t\t<ul v-show="filteredServices.length">\n\t\t\t\t\t\t<li v-for="service in filteredServices" >\n\t\t\t\t\t\t\t{{ service.name }}\n\t\t\t\t\t\t\t<input type="checkbox" class="switch" :id="service.name" :checked="service.isRun" @change="statusChange" />\n\t\t\t\t\t\t\t<label :for="service.name"></label>\n\t\t\t\t\t\t</li>\n\t\t\t\t\t</ul>\n\t\t\t\t</div>\n\t\t\t</div>',
	data: function data() {
		return {
			servicesList: [],
			searchString: '',
			isBusy: false
		};
	},
	created: function created() {
		var _this = this;

		var services = (0, _child_process.spawn)('bash', ['-c', _config.commands.getAllService]);

		services.stdout.on('data', function (data) {
			data = data.toString().split('\n').filter(function (s) {
				return s != '';
			});

			data.forEach(function (service) {
				var serviceName = service.substring(1);
				var isRun = service.substring(0, 1) == '+' ? 'checked' : '';
				_this.servicesList.push({ name: serviceName, isRun: isRun });
			});
		});
	},

	methods: {
		statusChange: function statusChange(e) {
			var _this2 = this;

			var serviceName = e.target.id;
			var status = e.target.checked ? 'start' : 'stop';

			if (!this.isBusy) {
				this.isBusy = true;
				_sudoPrompt2.default.exec((0, _helpers.command)('service ' + serviceName + ' ' + status), { name: 'Stacer' }, function (error, stdout, stderr) {
					if (stderr) {
						e.target.checked = !status;
						(0, _helpers.showMessage)('Operation not successful.', 'error');
					} else {
						(0, _helpers.showMessage)(serviceName + ' service ' + status + (e.target.checked ? 'ed' : 'ped'), 'success');
					}

					_this2.isBusy = false;
				});
			} else {
				(0, _helpers.showMessage)('Another process continues.', 'error');
			}
		}
	},
	computed: {
		filteredServices: function filteredServices() {
			var tempServicesList = this.servicesList,
			    searchString = this.searchString;

			if (!searchString) return tempServicesList;

			searchString = searchString.toString().trim().toLowerCase();

			return tempServicesList.filter(function (item) {
				return item.name.toString().trim().toLowerCase().indexOf(searchString) !== -1;
			});
		}
	}
};
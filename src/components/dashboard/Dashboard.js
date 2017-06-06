import {
	shell
} from 'electron'
import si from 'systeminformation'
//Components
import SystemInfo from './SystemInfo'
import UpBar from './UpBar'
import DownBar from './DownBar'
import CpuBar from './CpuBar'
import MemoryBar from './MemoryBar'
import DiskBar from './DiskBar'

export default {
	template: `<transition name="slide-fade">
					<div class="content">
						<slot></slot>
						<cpu-bar/>
						<memory-bar/>
						<disk-bar/>

						<down-bar/>
						<system-info/>
						<up-bar/>

						<!--Update Check-->
						<div class="update-check" v-show="update_check">
							<span>{{ lang('updateMsg') }}</span>
							<button @click="download_update">
								{{ lang('downloadUpdate') }}
							</button>
						</div>
						
					</div>
				</transition>`,
	data() {
		return ({
			update_check: false
		})
	},
	components: {
		'system-info': SystemInfo,
		'up-bar': UpBar,
		'down-bar': DownBar,
		'cpu-bar': CpuBar,
		'memory-bar': MemoryBar,
		'disk-bar': DiskBar
	},
	methods: {
		// open the link on browser
		download_update() {
			try {
				shell.openExternal('https://github.com/oguzhaninan/Stacer/releases/latest')
			} catch (err) {

			}
		}
	},
	created() {
		// Update check
		try {
			$.getJSON('https://api.github.com/repos/oguzhaninan/Stacer/releases/latest', data => {
				let currentVersion = require('../../../package.json').version.toString()
				let releaseVersion = data.tag_name.substr(1).toString()

				this.update_check = (currentVersion != releaseVersion)
			})
		} catch (error) {
			console.log(error)
		}
	}
}
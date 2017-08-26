import helpers from '../../utils/helpers'
import si from 'systeminformation'
import Chartkick from 'chartkick'
const seconds_max = 30;

export default {
	template: `<div>
					<h4>{{ lang('memHistory') }} <slot></slot></h4>
			   		<div id="memory-chart" :style="'height:' + fheight"></div>
				</div>`,
	props: ['fheight'],
	data() {
		return ({
			memoryValues: [],
			memoryData: [],
			seconds: Array.from(new Array(seconds_max),(val,index)=>index+1)
		})
	},
	mounted() {
		si.mem(ram => {
			let totalMem = helpers.prettyMemSize(ram.total)
			let totalSwap = helpers.prettyMemSize(ram.swaptotal)

			for (var i = 0; i < 2; i++)
				this.memoryValues.push((new Array(seconds_max)).fill(0))

			let memoryChart = new Chartkick.LineChart('memory-chart', this.memoryData, {
				colors: ['#2ecc71', '#e74c3c', '#3498db', '#f1c40f', '#9b59b6', '#34495e', '#1abc9c', '#e67e22'],
				min: 0,
				max: Math.max(totalMem, totalSwap),
				legend: true,
				points: false
			})

			setInterval(() => {
				si.mem(ram => {
					let usedMem = helpers.prettyMemSize(ram.total - ram.available)
					let usedSwap = helpers.prettyMemSize(ram.swapused)

					this.memoryValues.forEach((m, i) => this.memoryValues[i].splice(0, 1))

					this.memoryValues[0].push(usedMem)
					this.memoryValues[1].push(usedSwap)

					this.memoryData = []

					this.memoryData.push({
						name: lang('memory'),
						data: this.memoryValues[0].map((d, i) => [this.seconds[i], d])
					})

					this.memoryData.push({
						name: lang('swap').toString().toUpperCase(),
						data: this.memoryValues[1].map((d, i) => [this.seconds[i], d])
					})

					memoryChart.updateData(this.memoryData)
				})
			}, 1000)
		})
	}
}

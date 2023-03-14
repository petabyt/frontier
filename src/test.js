let E_LABEL = 1;
let E_CONTAINER = 2;

let COLOR_YELLOW = 0xded49e;
let COLOR_BLACK = 0;
let COLOR_WHITE = 0xffffff;
let COLOR_GREY = 0x626262;

let ui = {
	reset: ffi("void ui_reset()"),
	drawContainer: ffi("void ui_draw_container(int, int, int, int, int)"),
	endContainer: ffi("void ui_end_container()"),
	drawText: ffi("void ui_draw_text(char *, int)"),

	screen: [],

	addLabel: function(c, text) {
		let e = {type: E_LABEL, text: text, color: COLOR_BLACK};
		if (c === null) {
			this.screen[this.screen.length] = e;
		} else {
			c.children[c.children.length] = e;
		}

		return e;
	},

	addContainer: function(x, y, w, h, color) {
		let e = {type: E_CONTAINER, x: x, y: y, width: w, height: h, color: color, children: []};
		this.screen[this.screen.length] = e;
		return e;
	},

	renderContainer: function(e) {
		for (let i = 0; i < e.length; i++) {
			if (e[i].type === E_CONTAINER) {
				this.drawContainer(e[i].x, e[i].y, e[i].width, e[i].height, e[i].color);
				this.renderContainer(e[i].children);
				this.endContainer();
			} else if (e[i].type === E_LABEL) {
				this.drawText(e[i].text, e[i].color);
			}
		}
	},

	render: function() {
		bmp.clear(COLOR_YELLOW);
		this.reset();
		this.renderContainer(this.screen);
		bmp.render();
	}
};

let clicks = 0;
let framesLbl = null;
function setupScreen() {
	let c = ui.addContainer(0, 0, 0, 50, COLOR_WHITE);
	ui.addLabel(c, "OS uilib");
	let c2 = ui.addContainer(0, 0, 0, 0, COLOR_GREY);
	framesLbl = ui.addLabel(c2, "asd");
}

setupScreen();

function renderFrame() {
	ui.render();
	framesLbl.text = "mJS Render speed test: " + String(clicks);
	clicks++;
}

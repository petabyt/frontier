let clicks = 0;

let E_LABEL = 1;

let COLOR_YELLOW = 0xded49e;
let COLOR_BLACK = 0;

let elements = [
	{type: E_LABEL, x: 10, y: 10, text: "Hello, World", color: COLOR_BLACK},
];

function addLabel(text, x, y) {
	elements[elements.length] = {type: E_LABEL, x: x, y: y, text: text, color: COLOR_BLACK};
}

addLabel("FOo bar", 10, 50);

function renderFrame() {
	bmp.clear(COLOR_YELLOW);

	elements[0].text = "Clicked " + String(clicks) + " times";
	
	for (let i = 0; i < elements.length; i++) {
		if (elements[i].type === E_LABEL) {
			bmp.text(elements[i].x, elements[i].y, elements[i].text, elements[i].color);
		}
	}

	bmp.text(io.mouse_x(), io.mouse_y(), "Move to pointer", 0xfff);
	
	bmp.render();
}

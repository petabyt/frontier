let io = {
	print: ffi("void uart_str(char *)"),
};

let bmp = {
	text: ffi("void font_print_string(int, int, char *)"),
	render: ffi("void bmp_apply()")
};

function String(number) {
	return JSON.stringify(number);
}

for (let i = 0; i < 10; i++) {
	bmp.text(10, 10 + (i * 10), "Hello World " + String(i) + "\n");
}

bmp.render();

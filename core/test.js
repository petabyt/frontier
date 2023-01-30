let io = {
	print: ffi("void uart_str(char *)"),
};

function String(number) {
	return JSON.stringify(number);
}

for (let i = 0; i < 10; i++) {
	io.print("Hello " + String(i) + "\n");
}

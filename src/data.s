.section rodata

.align 4
.global all_js
all_js:
.incbin "lib.js"
.incbin "test.js"

.global all_js_size
.align 4
all_js_size:
.int all_js_size - all_js

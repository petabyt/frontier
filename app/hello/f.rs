// Rust bindings fo Frontier OS
macro_rules! cstr {
    ($data:literal) => {
        &concat!($data, "\0").as_bytes()[0] as *const u8
    }
}

#[allow(dead_code)]
pub mod c {
	extern {
		pub fn puts(str: *const u8);
		pub fn calc() -> u32;
		fn printf(fmt: *const u8, ...) -> u32;
	}
}

#[no_mangle]
pub fn panic() {
	unsafe {
		c::puts(cstr!("RUST PANIC"));
	}

	loop {}
}

pub fn str_to_u8_ptr(input_str: &str, buffer: &mut [u8]) -> *const u8 {
    let input_bytes = input_str.as_bytes();
    let buffer_len = buffer.len();
    if input_bytes.len() + 1 > buffer_len {
        panic!("buffer is too small to hold string and null terminator");
    }
    for i in 0..input_bytes.len() {
        buffer[i] = input_bytes[i];
    }
    buffer[input_bytes.len()] = 0;
    buffer.as_ptr()
}

pub fn puts(str: &str) {
    let mut buffer = [0u8; 256];
    let ptr = str_to_u8_ptr(str, &mut buffer);
	unsafe {
		c::puts(ptr);
	}
}

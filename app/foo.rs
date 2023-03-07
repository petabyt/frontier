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
fn panic() {
	unsafe {
		c::puts(cstr!("RUST PANIC"));
	}

	loop {}
}

fn puts(str: *const u8) {
	unsafe {
		c::puts(str);
	}
}

#[no_mangle]
pub fn main() -> u32 {
	puts(cstr!("Hello ELF Rust World"));

	return 0;
}

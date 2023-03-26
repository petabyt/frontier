mod f;

#[no_mangle]
pub fn main() -> u32 {
	f::puts("Hello rust world");

	let mut i = 0;
	while i < 10 {
		f::puts("foo");
		i = i + 1;
	}

return 123;
}

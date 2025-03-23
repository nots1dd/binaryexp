use libc::{exit, fflush, printf, signal, SIGSEGV};
use std::ffi::CStr;
use std::io::{self, Write};

static mut FLAG: [u8; 32] = *b"crypto{hidden_flag_hereeeeeeeee}";

extern "C" fn sigsegv_handler(_sig: i32) {
    unsafe {
        // Copy FLAG to a local buffer before creating CStr
        let flag_copy = FLAG; // This avoids shared references to mutable statics
        let flag_str = CStr::from_ptr(flag_copy.as_ptr() as *const i8);

        println!("\n[!] Segmentation fault detected!");
        println!("{}", flag_str.to_string_lossy());
        fflush(std::ptr::null_mut()); // Flush all output streams
        exit(1);
    }
}

fn main() {
    unsafe {
        signal(SIGSEGV, sigsegv_handler as usize);
    }

    const BUFSIZE: usize = 128;
    let mut input = String::with_capacity(BUFSIZE);

    print!("Enter your name: ");
    io::stdout().flush().unwrap();
    io::stdin().read_line(&mut input).unwrap();

    unsafe {
        printf(b"Hello, \0".as_ptr() as *const i8);
        printf(input.as_ptr() as *const i8); // Potential vulnerability
        printf(b"\n\0".as_ptr() as *const i8);
    }
}

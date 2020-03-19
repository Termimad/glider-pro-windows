mod apple_double;
mod mac_roman;
mod macbinary;
mod rsrcfork;
mod utils;

use crate::apple_double::AppleDouble;
use crate::macbinary::MacBinary;
use crate::rsrcfork::ResourceFork;
use std::env;
use std::fs::File;
use std::io::{self, BufReader, BufWriter, Cursor, Read, Seek, SeekFrom, Write};

fn extract_resource_bytes(mut reader: impl Read + Seek) -> io::Result<Option<Vec<u8>>> {
    reader.seek(SeekFrom::Start(0))?;
    if let Ok(Some(data)) = AppleDouble::read_from(&mut reader) {
        return Ok(Some(data.rsrc));
    }
    reader.seek(SeekFrom::Start(0))?;
    if let Ok(Some(data)) = MacBinary::read_from(&mut reader) {
        return Ok(Some(data.rsrc));
    }
    let mut bytes = Vec::new();
    reader.read_to_end(&mut bytes).map(|_| Some(bytes))
}

fn derez_resfork(_resfork: ResourceFork, _writer: impl Write) {
    unimplemented!("derez_resfork");
}

fn dump_resfork(_resfork: ResourceFork, _writer: impl Write) {
    unimplemented!("dump_resfork");
}

fn convert_resfork(_resfork: ResourceFork, _writer: impl Write) {
    unimplemented!("convert_resfork");
}

fn main() {
    let arguments = env::args().skip(1).collect::<Vec<_>>();
    if arguments.len() < 3 {
        print_usage();
        return;
    }
    let command = &arguments[0];
    let input_name = &arguments[1];
    let output_name = &arguments[2];
    let input_file = match File::open(input_name) {
        Ok(file) => BufReader::new(file),
        Err(e) => {
            eprintln!("error: could not open input file: {}", e);
            return;
        }
    };
    let output_file = match File::create(output_name) {
        Ok(file) => BufWriter::new(file),
        Err(e) => {
            eprintln!("error: could not open output file: {}", e);
            return;
        }
    };
    let resfork_bytes = match extract_resource_bytes(input_file) {
        Ok(Some(bytes)) => bytes,
        Ok(None) => {
            eprintln!("error: could not determine file format");
            return;
        }
        Err(e) => {
            eprintln!("error: input reading failed: {}", e);
            return;
        }
    };
    let resfork_cursor = Cursor::new(resfork_bytes);
    let resfork = match ResourceFork::read_from(resfork_cursor) {
        Ok(fork) => fork,
        Err(e) => {
            eprintln!("error: could not parse resource fork: {}", e);
            return;
        }
    };
    match command.as_str() {
        "derez" => derez_resfork(resfork, output_file),
        "dump" => dump_resfork(resfork, output_file),
        "convert" => convert_resfork(resfork, output_file),
        _ => {
            eprintln!("error: unknown command");
            print_usage();
            return;
        }
    }
}

fn print_usage() {
    eprintln!("usage: resfork-extract <command> <input> [output]");
    eprintln!();
    eprintln!("    <command>  One of 'derez', 'dump', or 'convert'.");
    eprintln!("    <input>    Input file's name.");
    eprintln!("    <output>   Output file's name. Defaults to stdout if omitted.");
}

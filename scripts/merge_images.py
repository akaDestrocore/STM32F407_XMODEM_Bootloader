#!/usr/bin/env python3
import argparse
import os
import struct
import binascii
import sys

# Constants matching the C implementation
IMAGE_MAGIC_LOADER = 0xDEADC0DE
IMAGE_MAGIC_UPDATER = 0xFEEDFACE
IMAGE_MAGIC_APP = 0xC0FFEE00
IMAGE_VERSION_CURRENT = 0x0100

IMAGE_TYPE_LOADER = 1
IMAGE_TYPE_UPDATER = 2
IMAGE_TYPE_APP = 3

HEADER_SIZE = 0x200

# Memory map offsets from base address 0x08000000
LOADER_ADDR = 0x08002000
UPDATER_ADDR = 0x0800F000
APP_ADDR = 0x0801C000

# CRC32 calculation using the same algorithm as STM32 hardware CRC
def calculate_crc32(data):
    crc = 0xFFFFFFFF
    
    for i in range(0, len(data), 4):
        word = 0
        for j in range(min(4, len(data) - i)):
            word |= data[i + j] << (j * 8)

        crc ^= word
        for _ in range(32):
            if crc & 0x80000000:
                crc = (crc << 1) ^ 0x04C11DB7
            else:
                crc = (crc << 1)
            crc &= 0xFFFFFFFF

    return crc

def is_header_present(data):
    """Check if the binary data already contains a header by looking for magic numbers"""
    if len(data) < 4:
        return False
    
    magic = int.from_bytes(data[0:4], byteorder='little')
    return magic in [IMAGE_MAGIC_LOADER, IMAGE_MAGIC_UPDATER, IMAGE_MAGIC_APP]

def extract_header_fields(header_data):
    """Extract fields from binary header data"""
    if len(header_data) < HEADER_SIZE:
        return None
    
    # Unpack header fields from binary data - the struct size is 24 bytes
    header_fields = struct.unpack('<LHBBBBHLLL', header_data[0:24])
    
    header = {
        'magic': header_fields[0],
        'image_hdr_version': header_fields[1],
        'image_type': header_fields[2],
        'version_major': header_fields[3],
        'version_minor': header_fields[4],
        'version_patch': header_fields[5],
        '_padding': header_fields[6],
        'vector_addr': header_fields[7],
        'crc': header_fields[8],
        'data_size': header_fields[9]
    }
    
    return header

def create_updated_header(header_dict, image_data, base_addr):
    """Create binary header based on header dict and image data"""
    # Calculate CRC on the actual data
    crc = calculate_crc32(image_data)
    data_size = len(image_data)
    
    # Update header fields
    header_dict['crc'] = crc
    header_dict['data_size'] = data_size
    header_dict['vector_addr'] = base_addr + HEADER_SIZE
    
    # Create binary header
    header = struct.pack('<LHBBBBHLLL',
                         header_dict['magic'],
                         header_dict['image_hdr_version'],
                         header_dict['image_type'],
                         header_dict['version_major'],
                         header_dict['version_minor'],
                         header_dict['version_patch'],
                         header_dict['_padding'],
                         header_dict['vector_addr'],
                         header_dict['crc'],
                         header_dict['data_size'])
    
    # Pad the header to HEADER_SIZE bytes
    header += b'\x00' * (HEADER_SIZE - len(header))
    
    return header

def create_new_header(image_type, magic, version, vector_addr, data):
    """Create a binary header structure from scratch"""
    version_major, version_minor, version_patch = version
    
    # Calculate CRC on the actual data
    crc = calculate_crc32(data)
    data_size = len(data)
    
    # Create header structure
    header = struct.pack('<LHBBBBHLLL',
                         magic,                    # image_magic (4 bytes)
                         IMAGE_VERSION_CURRENT,    # image_hdr_version (2 bytes)
                         image_type,               # image_type (1 byte)
                         version_major,            # version_major (1 byte)
                         version_minor,            # version_minor (1 byte)
                         version_patch,            # version_patch (1 byte)
                         0,                        # _padding (2 bytes)
                         vector_addr,              # vector_addr (4 bytes)
                         crc,                      # crc (4 bytes)
                         data_size)                # data_size (4 bytes)
    
    # Pad the header to HEADER_SIZE bytes
    header += b'\x00' * (HEADER_SIZE - len(header))
    
    print(f"Header created for {image_type_to_str(image_type)}:")
    print(f"  - Magic: 0x{magic:08X}")
    print(f"  - Version: {version_major}.{version_minor}.{version_patch}")
    print(f"  - Vector Address: 0x{vector_addr:08X}")
    print(f"  - Data Size: {data_size} bytes")
    print(f"  - CRC32: 0x{crc:08X}")
    
    return header

def image_type_to_str(image_type):
    if image_type == IMAGE_TYPE_LOADER:
        return "Loader"
    elif image_type == IMAGE_TYPE_UPDATER:
        return "Updater"
    elif image_type == IMAGE_TYPE_APP:
        return "Application"
    else:
        return "Unknown"

def patch_binary(filename, image_type, version, base_addr):
    """Patch a binary with a proper header or update existing header"""
    # Read the binary data
    with open(filename, 'rb') as f:
        binary_data = f.read()
    
    # Determine base address based on image type if not provided
    if base_addr is None:
        if image_type == IMAGE_TYPE_LOADER:
            base_addr = LOADER_ADDR
        elif image_type == IMAGE_TYPE_UPDATER:
            base_addr = UPDATER_ADDR
        elif image_type == IMAGE_TYPE_APP:
            base_addr = APP_ADDR
        else:
            raise ValueError(f"Invalid image type: {image_type}")
    
    # Get the magic number for this image type
    if image_type == IMAGE_TYPE_LOADER:
        magic = IMAGE_MAGIC_LOADER
    elif image_type == IMAGE_TYPE_UPDATER:
        magic = IMAGE_MAGIC_UPDATER
    elif image_type == IMAGE_TYPE_APP:
        magic = IMAGE_MAGIC_APP
    else:
        raise ValueError(f"Invalid image type: {image_type}")
    
    # Check if binary already has a header
    if is_header_present(binary_data) and len(binary_data) >= HEADER_SIZE:
        print(f"Detected existing header in {filename}, updating...")
        
        # Extract the existing header and image data
        existing_header_data = binary_data[:HEADER_SIZE]
        image_data = binary_data[HEADER_SIZE:]
        
        # Parse header fields
        header_dict = extract_header_fields(existing_header_data)
        
        if header_dict:
            # Update header with new CRC, size and vector address
            updated_header = create_updated_header(header_dict, image_data, base_addr)
            print(f"Updated existing header for {image_type_to_str(image_type)}:")
            print(f"  - Magic: 0x{header_dict['magic']:08X}")
            print(f"  - Version: {header_dict['version_major']}.{header_dict['version_minor']}.{header_dict['version_patch']}")
            print(f"  - Vector Address: 0x{header_dict['vector_addr']:08X}")
            print(f"  - Data Size: {header_dict['data_size']} bytes")
            print(f"  - CRC32: 0x{header_dict['crc']:08X}")
        else:
            # Couldn't parse header, create a new one
            print(f"Couldn't parse existing header, creating new one...")
            vector_addr = base_addr + HEADER_SIZE
            updated_header = create_new_header(image_type, magic, version, vector_addr, image_data)
    else:
        # No header found, create a new one
        print(f"No header found in {filename}, creating new one...")
        vector_addr = base_addr + HEADER_SIZE
        image_data = binary_data
        updated_header = create_new_header(image_type, magic, version, vector_addr, image_data)
    
    # Write the patched binary (header + data)
    output_filename = os.path.splitext(filename)[0] + "_patched.bin"
    with open(output_filename, 'wb') as f:
        f.write(updated_header)
        f.write(image_data)
    
    print(f"Patched binary written to {output_filename}")
    return output_filename

def merge_binaries(boot_filename, loader_filename, updater_filename, app_filename, output_filename="merged_firmware.bin"):
    """Merge multiple binaries into a single flash image"""
    # Memory map offsets from base address 0x08000000
    LOADER_OFFSET = 0x2000 
    UPDATER_OFFSET = 0xF000
    APP_OFFSET = 0x1C000  

    # Get file sizes
    boot_size = os.stat(boot_filename).st_size
    loader_size = os.stat(loader_filename).st_size
    updater_size = os.stat(updater_filename).st_size
    app_size = os.stat(app_filename).st_size

    # Verify sizes
    if boot_size > LOADER_OFFSET:
        raise Exception(f"Bootloader is too big! Size: {boot_size}, Max: {LOADER_OFFSET}")
    if loader_size > (UPDATER_OFFSET - LOADER_OFFSET):
        raise Exception(f"Loader is too big! Size: {loader_size}, Max: {UPDATER_OFFSET - LOADER_OFFSET}")
    if updater_size > (APP_OFFSET - UPDATER_OFFSET):
        raise Exception(f"Updater is too big! Size: {updater_size}, Max: {APP_OFFSET - UPDATER_OFFSET}")

    print(f"Merging binaries:")
    print(f"Boot: {boot_filename} ({boot_size} bytes) at offset 0x{0:X}")
    print(f"Loader: {loader_filename} ({loader_size} bytes) at offset 0x{LOADER_OFFSET:X}")
    print(f"Updater: {updater_filename} ({updater_size} bytes) at offset 0x{UPDATER_OFFSET:X}")
    print(f"App: {app_filename} ({app_size} bytes) at offset 0x{APP_OFFSET:X}")
    print(f"Output: {output_filename}")

    # Read all binaries
    with open(boot_filename, "rb") as f:
        boot_data = f.read()
    with open(loader_filename, "rb") as f:
        loader_data = f.read()
    with open(updater_filename, "rb") as f:
        updater_data = f.read()
    with open(app_filename, "rb") as f:
        app_data = f.read()

    # Create merged binary
    output_data = bytearray(APP_OFFSET + len(app_data))
    
    # Copy components at their offsets
    output_data[0:len(boot_data)] = boot_data
    output_data[LOADER_OFFSET:LOADER_OFFSET+len(loader_data)] = loader_data
    output_data[UPDATER_OFFSET:UPDATER_OFFSET+len(updater_data)] = updater_data
    output_data[APP_OFFSET:APP_OFFSET+len(app_data)] = app_data

    with open(output_filename, "wb") as f:
        f.write(output_data)

    print(f"Created merged binary: {output_filename}")
    print(f"Total size: {os.stat(output_filename).st_size} bytes")
    return output_filename

def main():
    parser = argparse.ArgumentParser(description="STM32F4 Bootloader Image Management Tool")
    subparsers = parser.add_subparsers(dest="command", help="Command to execute")
    
    # Patch command
    patch_parser = subparsers.add_parser("patch", help="Patch a binary with header information")
    patch_parser.add_argument("filename", help="Binary file to patch")
    patch_parser.add_argument("--type", type=int, required=True, choices=[1, 2, 3], 
                             help="Image type: 1=Loader, 2=Updater, 3=Application")
    patch_parser.add_argument("--version", required=True, help="Version in format 'major.minor.patch'")
    patch_parser.add_argument("--base-addr", type=lambda x: int(x, 0), 
                             help="Base address override (default: determined by type)")
    
    # Merge command
    merge_parser = subparsers.add_parser("merge", help="Merge multiple binaries into a single image")
    merge_parser.add_argument("boot", help="Boot binary file")
    merge_parser.add_argument("loader", help="Loader binary file")
    merge_parser.add_argument("updater", help="Updater binary file")
    merge_parser.add_argument("app", help="Application binary file")
    merge_parser.add_argument("--output", help="Output filename (default: merged_firmware.bin)")
    
    # Build command - patch and merge in one step
    build_parser = subparsers.add_parser("build", help="Patch all binaries and merge into a single image")
    build_parser.add_argument("boot", help="Boot binary file")
    build_parser.add_argument("loader", help="Loader binary file")
    build_parser.add_argument("updater", help="Updater binary file")
    build_parser.add_argument("app", help="Application binary file")
    build_parser.add_argument("--loader-version", default="1.0.0", help="Loader version (default: 1.0.0)")
    build_parser.add_argument("--updater-version", default="1.0.0", help="Updater version (default: 1.0.0)")
    build_parser.add_argument("--app-version", default="1.0.0", help="Application version (default: 1.0.0)")
    build_parser.add_argument("--output", help="Output filename (default: merged_firmware.bin)")
    
    args = parser.parse_args()
    
    if args.command == "patch":
        version_parts = args.version.split('.')
        if len(version_parts) != 3:
            parser.error("Version must be in format 'major.minor.patch'")
        
        try:
            version = tuple(map(int, version_parts))
        except ValueError:
            parser.error("Version components must be integers")
            
        patch_binary(args.filename, args.type, version, args.base_addr)
        
    elif args.command == "merge":
        output = merge_binaries(args.boot, args.loader, args.updater, args.app)
        if args.output and args.output != "merged_firmware.bin":
            os.rename("merged_firmware.bin", args.output)
            print(f"Renamed output to {args.output}")
            
    elif args.command == "build":
        # Parse versions
        def parse_version(version_str):
            parts = version_str.split('.')
            if len(parts) != 3:
                parser.error(f"Version '{version_str}' must be in format 'major.minor.patch'")
            try:
                return tuple(map(int, parts))
            except ValueError:
                parser.error(f"Version components in '{version_str}' must be integers")
        
        loader_version = parse_version(args.loader_version)
        updater_version = parse_version(args.updater_version)
        app_version = parse_version(args.app_version)
        
        # Patch binaries (boot doesn't need patching)
        print("\n=== Patching Loader ===")
        loader_patched = patch_binary(args.loader, IMAGE_TYPE_LOADER, loader_version, LOADER_ADDR)
        
        print("\n=== Patching Updater ===")
        updater_patched = patch_binary(args.updater, IMAGE_TYPE_UPDATER, updater_version, UPDATER_ADDR)
        
        print("\n=== Patching Application ===")
        app_patched = patch_binary(args.app, IMAGE_TYPE_APP, app_version, APP_ADDR)
        
        # Merge patched binaries
        print("\n=== Merging Binaries ===")
        output = merge_binaries(args.boot, loader_patched, updater_patched, app_patched)
        
        if args.output and args.output != "merged_firmware.bin":
            os.rename("merged_firmware.bin", args.output)
            print(f"Renamed output to {args.output}")
    else:
        parser.print_help()
        sys.exit(1)

if __name__ == "__main__":
    main()
#!/usr/bin/env python3
import argparse
import os
import struct
import binascii
import sys
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

DEFAULT_KEY = bytes([
    0x57, 0xE3, 0x05, 0x34, 0xDB, 0x19, 0x4B, 0x25, 
    0x09, 0x13, 0xB9, 0x64, 0x3A, 0x42, 0xE6, 0x9B 
])

DEFAULT_AAD = bytes([
    0x66, 0x66, 0x30, 0x36, 0x62, 0x35, 0x63, 0x79, 
    0x62, 0x65, 0x72, 0x70, 0x75, 0x6e, 0x6b, 0x32
])

def encrypt_firmware(input_file, output_file, key=DEFAULT_KEY, aad=DEFAULT_AAD):
    """Encrypt a firmware binary using AES-GCM"""
    print(f"Reading firmware from {input_file}")
    with open(input_file, "rb") as f:
        firmware_data = f.read()
    
    print(f"Firmware size: {len(firmware_data)} bytes")
    
    # Generate random 12-byte IV key
    nonce = get_random_bytes(12)
    print(f"Generated nonce: {binascii.hexlify(nonce).decode()}")
    
    # Create AES-GCM cipher
    cipher = AES.new(key, AES.MODE_GCM, nonce=nonce)
    
    # Add header (AAD)
    cipher.update(aad)
    
    # Encrypt the firmware
    ciphertext, tag = cipher.encrypt_and_digest(firmware_data)
    
    print(f"Encryption complete, authentication tag: {binascii.hexlify(tag).decode()}")
    
    # Prepare header for XMODEM:  nonce + size + encrypted_data + tag
    size_bytes = struct.pack(">I", len(firmware_data))
    
    # Write encrypted firmware with header
    with open(output_file, "wb") as f:
        f.write(nonce)          # 12 bytes
        f.write(size_bytes)     # 4 bytes
        f.write(ciphertext)     # variable length
        f.write(tag)            # 16 bytes
    
    total_size = len(nonce) + len(size_bytes) + len(ciphertext) + len(tag)
    print(f"Encrypted firmware written to {output_file} ({total_size} bytes)")
    print(f"  - Header size: {len(nonce) + len(size_bytes)} bytes")
    print(f"  - Encrypted data: {len(ciphertext)} bytes")
    print(f"  - Authentication tag: {len(tag)} bytes")
    
    # Calculate how many XMODEM packets will be needed
    packet_size = 128
    packet_count = (total_size + packet_size - 1) // packet_size
    print(f"Will require {packet_count} XMODEM packets for transmission")
    
    return True

def decrypt_firmware(input_file, output_file, key=DEFAULT_KEY, aad=DEFAULT_AAD):
    print(f"Reading encrypted firmware from {input_file}")
    with open(input_file, "rb") as f:
        data = f.read()
    
    if len(data) < 32:
        print("Error: Encrypted file is too small")
        return False
    
    # Extract components
    nonce = data[:12]
    size_bytes = data[12:16]
    original_size = struct.unpack(">I", size_bytes)[0]
    
    # Calculate sizes
    tag = data[-16:]
    ciphertext = data[16:-16]
    
    print(f"Encrypted firmware size: {len(data)} bytes")
    print(f"  - Nonce: {binascii.hexlify(nonce).decode()}")
    print(f"  - Original size: {original_size} bytes")
    print(f"  - Encrypted data: {len(ciphertext)} bytes")
    print(f"  - Authentication tag: {binascii.hexlify(tag).decode()}")
    
    try:
        cipher = AES.new(key, AES.MODE_GCM, nonce=nonce)
        cipher.update(aad)
        
        # Decrypt and verify
        plaintext = cipher.decrypt_and_verify(ciphertext, tag)
        
        # Verify size
        if len(plaintext) != original_size:
            print(f"Warning: Decrypted size ({len(plaintext)}) doesn't match header size ({original_size})")
        
        # Write decrypted firmware
        with open(output_file, "wb") as f:
            f.write(plaintext)
        
        print(f"Decryption successful, firmware written to {output_file}")
        return True
    
    except Exception as e:
        print(f"Decryption failed: {str(e)}")
        return False

def main():
    parser = argparse.ArgumentParser(description="Encrypt/Decrypt firmware for secure bootloader")
    parser.add_argument("--key", help="AES-128 key in hex format (32 characters)", default=None)
    parser.add_argument("--aad", help="Additional Authenticated Data in hex format", default=None)
    
    subparsers = parser.add_subparsers(dest="command", help="Command to execute")
    
    # Encrypt command
    encrypt_parser = subparsers.add_parser("encrypt", help="Encrypt a firmware binary")
    encrypt_parser.add_argument("input", help="Input firmware binary file")
    encrypt_parser.add_argument("output", help="Output encrypted firmware file")
    
    # Decrypt command
    decrypt_parser = subparsers.add_parser("decrypt", help="Decrypt an encrypted firmware binary")
    decrypt_parser.add_argument("input", help="Input encrypted firmware file")
    decrypt_parser.add_argument("output", help="Output decrypted firmware file")
    
    args = parser.parse_args()
    
    # Get key from arguments or use default
    key = DEFAULT_KEY
    if args.key:
        try:
            if len(args.key) != 32:
                print("Error: Key must be 16 bytes (32 hex characters)")
                return 1
            key = bytes.fromhex(args.key)
        except ValueError:
            print("Error: Invalid key format. Must be hex string")
            return 1
    
    # Get header from arguments or use default
    aad = DEFAULT_AAD
    if args.aad:
        try:
            aad = bytes.fromhex(args.aad)
        except ValueError:
            print("Error: Invalid AAD format. Must be hex string")
            return 1
    
    if args.command == "encrypt":
        if not os.path.exists(args.input):
            print(f"Error: Input file {args.input} does not exist")
            return 1
        
        if encrypt_firmware(args.input, args.output, key, aad):
            return 0
        else:
            return 1
    
    elif args.command == "decrypt":
        if not os.path.exists(args.input):
            print(f"Error: Input file {args.input} does not exist")
            return 1
        
        if decrypt_firmware(args.input, args.output, key, aad):
            return 0
        else:
            return 1
    
    else:
        parser.print_help()
        return 1

if __name__ == "__main__":
    sys.exit(main())
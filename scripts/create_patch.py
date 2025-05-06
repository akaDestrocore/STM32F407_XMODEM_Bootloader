import os
import sys
import argparse
import subprocess
import shutil

HEADER_SIZE = 0x200

def create_patch(old_firmware, new_firmware, output_patch, encrypt=False):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    
    print(f"Creating patch from {old_firmware} to {new_firmware}")
    
    # Create temp dir
    temp_dir = os.path.join(script_dir, "temp")
    os.makedirs(temp_dir, exist_ok=True)
    
    # Create paths for temp files
    old_no_header = os.path.join(temp_dir, "old_no_header.bin")
    new_no_header = os.path.join(temp_dir, "new_no_header.bin")
    patch_file = os.path.join(temp_dir, "patch.bin")
    temp_final = os.path.join(temp_dir, "final_patch.bin")
    
    try:
        # Extract header
        with open(new_firmware, "rb") as f:
            new_header = f.read(HEADER_SIZE)
        
        # Set the patch flag even tho it should be set already (for CRC to pass)
        header_bytes = bytearray(new_header)
        header_bytes[7] = 1  # Set is_patch flag
        new_header = bytes(header_bytes)
        
        # Create headerless binaries
        with open(old_firmware, "rb") as f:
            f.seek(HEADER_SIZE)
            with open(old_no_header, "wb") as out_f:
                out_f.write(f.read())
        
        with open(new_firmware, "rb") as f:
            f.seek(HEADER_SIZE)
            with open(new_no_header, "wb") as out_f:
                out_f.write(f.read())
        
        # Print sizes for verification
        old_size = os.path.getsize(old_no_header)
        new_size = os.path.getsize(new_no_header)
        print(f"Old firmware (no header): {old_size} bytes")
        print(f"New firmware (no header): {new_size} bytes")
        
        # Run jdiff to create the patch
        original_dir = os.getcwd()
        os.chdir(script_dir)
        jdiff_cmd = f"./jdiff -j {old_no_header} {new_no_header} {patch_file}"
        print(f"Running: {jdiff_cmd}")

        try:
            result = os.system(jdiff_cmd)
            print(f"jdiff completed with return code: {result}")
        except Exception as e:
            print(f"Exception while running jdiff: {e}")
        
        # Return to original directory
        os.chdir(original_dir)
        
        # Check if the patch file was created successfully
        if not os.path.exists(patch_file):
            print(f"Error: Output patch file {patch_file} not found")
            return False
        
        patch_size = os.path.getsize(patch_file)
        if patch_size == 0:
            print(f"Error: Output patch file {patch_file} is empty")
            return False
        
        print(f"Patch file created successfully: {patch_size} bytes")
        
        # Combine header with patch data
        with open(patch_file, "rb") as f:
            patch_data = f.read()
        
        # Write final patch (header + patch data)
        with open(temp_final, "wb") as f:
            f.write(new_header)
            f.write(patch_data)
        
        # Encrypt if requested
        if encrypt:
            encrypt_script = os.path.join(script_dir, 'encrypt_firmware.py')
            
            if not os.path.exists(encrypt_script):
                print(f"Error: Encryption script not found at {encrypt_script}")
                return False
            
            print(f"Encrypting patch...")
            try:
                # Run encryption script
                cmd = [sys.executable, encrypt_script, 'encrypt', temp_final, output_patch]
                result = subprocess.run(cmd, capture_output=True, text=True)
                
                if result.returncode != 0:
                    print(f"Encryption failed with return code {result.returncode}")
                    if result.stdout:
                        print(f"Output: {result.stdout}")
                    if result.stderr:
                        print(f"Error: {result.stderr}")
                    return False
                
                print("Encryption completed successfully")
            except Exception as e:
                print(f"Exception during encryption: {e}")
                return False
        else:
            # Just copy the file if no encryption
            shutil.copy2(temp_final, output_patch)
        
        # Print final information
        final_size = os.path.getsize(output_patch)
        print(f"Successfully created patch file: {output_patch}")
        print(f"Final patch size: {final_size} bytes")
        
        return True
    
    except Exception as e:
        print(f"Error creating patch: {e}")
        import traceback
        traceback.print_exc()
        return False
    
    finally:
        # Clean up temporary files
        for file in [old_no_header, new_no_header, patch_file, temp_final]:
            if os.path.exists(file):
                try:
                    os.remove(file)
                except Exception as e:
                    print(f"Warning: Failed to remove temporary file {file}: {e}")

def main():
    parser = argparse.ArgumentParser(description='Create delta patch between firmware versions')
    parser.add_argument('old_firmware', help='Old firmware binary file')
    parser.add_argument('new_firmware', help='New firmware binary file')
    parser.add_argument('output_patch', help='Output patch file')
    parser.add_argument('-e', '--encrypt', action='store_true', 
                        help='Encrypt the patch after creation')
    parser.add_argument('-b', '--build-dir', action='store_true',
                        help='Place output in build directory automatically')
    
    args = parser.parse_args()
    
    # Check if input files exist
    if not os.path.isfile(args.old_firmware):
        print(f"Error: Old firmware file not found: {args.old_firmware}")
        return 1
    
    if not os.path.isfile(args.new_firmware):
        print(f"Error: New firmware file not found: {args.new_firmware}")
        return 1
    
    # Determine output path
    output_path = args.output_patch
    
    # If build directory option is enabled - place in build directory
    if args.build_dir:
        # Get the filename from the output path
        output_filename = os.path.basename(output_path)
        # Place it in the build dir
        output_path = os.path.join("build", output_filename)
        print(f"Output will be placed in build directory: {output_path}")
    
    # Create output directory if it doesn't exist
    output_dir = os.path.dirname(output_path)
    if output_dir and not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)
    
    # Create patch
    if create_patch(args.old_firmware, args.new_firmware, output_path, args.encrypt):
        print("Patch creation completed successfully")
        return 0
    else:
        print("Patch creation failed")
        return 1

if __name__ == "__main__":
    sys.exit(main())
import argparse

def patch_binary_payload(bin_filename, skip_header=False):
    if skip_header:
        print(f"Skipping header processing for {bin_filename}")
        return
        
    VERSION_SIZE = 4
    
    with open(bin_filename, "rb") as f:
        data = f.read()
        
    print(f"Binary '{bin_filename}': header = {data[:VERSION_SIZE].hex()}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter,
    )
    parser.add_argument("bin", action="store", help="Binary file to patch")
    parser.add_argument("--skip-header", action="store_true", help="Skip header processing (for bootloader)")
    args = parser.parse_args()

    patch_binary_payload(args.bin, args.skip_header)
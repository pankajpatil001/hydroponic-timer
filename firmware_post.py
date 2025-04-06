# firmware_post.py
import shutil
from pathlib import Path

Import("env")

def copy_firmware(source, target, env):
    # Path to the generated .bin file
    build_dir = Path(env.subst("$BUILD_DIR"))
    firmware_path = build_dir / "firmware.bin"
    
    # Destination directory
    dest_dir = Path(env.subst("$PROJECT_DIR")) / "firmwares"
    dest_dir.mkdir(exist_ok=True)

    # Copy the .bin file
    shutil.copy(firmware_path, dest_dir / f"{env['PIOENV']}.bin")
    print(f"Copied firmware to: {dest_dir / env['PIOENV']}.bin")

# Hook into post build
env.AddPostAction("$BUILD_DIR/firmware.bin", copy_firmware)

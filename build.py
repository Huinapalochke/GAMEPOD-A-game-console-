import subprocess
from pathlib import Path
from os import system
import re
import sys

system("cls")

ROOT = Path(__file__).parent
GAMES_DIR = ROOT / "Games"
OUTPUT = ROOT / "GeneratedGames.cpp"

if not GAMES_DIR.exists():
    print("[!] Games folder not found!")
    sys.exit(1)

game_files = sorted(GAMES_DIR.rglob("*.cpp"))

if not game_files:
    print("[!] No games found!")
    sys.exit(1)

print("=== GAMEPOD BUILD ===")

generated = []

for file in game_files:
    print("[+] " + str(file))

for file in game_files:
    code = file.read_text(encoding="utf-8")

    if '#include "gamepod_api.h"' not in code.lower():
        print(f"[!] API is not connected in <{file.name}>!")
        sys.exit(1)

    print(f"[.] API connected in <{file.name}>!")
    print

    if not re.search(r"\bvoid\s+RegisterGame\s*\(\s*\)", code, re.IGNORECASE):
        print(f"[!] Registration function not found in <{file.name}>!")
        sys.exit(1)

    print(f"[+] Registration function found in <{file.name}>!")
    print

    unique_name = "RegisterGame_" + re.sub(
        r"\W", "_", file.stem
    )

    code = re.sub(
        r"\bvoid\s+RegisterGame\s*\(\s*\)",
        f"void {unique_name}()",
        code,
        count=1,
        flags=re.IGNORECASE
    )

    generated.append((file, unique_name, code))
    print(f"[+] Registered: {unique_name}")

for file, unique_name, code in generated:
    generated_file = ROOT / f"Generated_{file.stem}.cpp"
    generated_file.write_text(code, encoding="utf-8")

with OUTPUT.open("w", encoding="utf-8") as f:
    f.write('#include "GamePod_API.h"\n\n')

    for file, unique_name, code in generated:
        f.write(f"void {unique_name}();\n")

    f.write("\nvoid RegisterAllGames() {\n")

    for file, unique_name, code in generated:
        f.write(f"    {unique_name}();\n")

    f.write("}\n")

print(f"\n[+] Generated: {OUTPUT.name}")
print(f"[+] Games found: {len(generated)}")
print("[+] Done!")

print()
print("Now you need to upload the code Using the Arduino IDE")
print("Import a file named as GAMEPOD.ino (the main one)")

input()
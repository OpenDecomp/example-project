#!/usr/bin/env python3
"""Rebuilds orig/<version>/obj/ and config/<version>/build.sha1 from example-only/original/.

Only this example has a script like this: its game is made up, so its "original" objects are
compiled from the made-up original source. Real projects never have the original source and
never commit anything from their game files.

    python3 example-only/make_targets.py [--cross powerpc-linux-gnu-]
"""

import argparse
import hashlib
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT))
import configure  # noqa: E402

ORIGINAL = "example-only/original"


def run(cmd: list[str]) -> None:
    subprocess.run(cmd, cwd=ROOT, check=True)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--cross", default=configure.DEFAULT_CROSS)
    cross = parser.parse_args().cross

    for version in configure.VERSIONS:
        objects = []
        for unit in configure.UNITS:
            obj = configure.orig_path(version, unit)
            (ROOT / obj).parent.mkdir(parents=True, exist_ok=True)
            src = f"{ORIGINAL}/src/{unit.name}.c"
            run([f"{cross}gcc", *configure.cflags(version), "-c", src, "-o", obj])
            objects.append(obj)
        with tempfile.TemporaryDirectory() as tmp:
            elf, binary = Path(tmp) / "game.elf", Path(tmp) / "game.bin"
            run([f"{cross}ld", "-T", "tools/link.ld", "-o", str(elf), *objects])
            run([f"{cross}objcopy", "-O", "binary", str(elf), str(binary)])
            digest = hashlib.sha1(binary.read_bytes()).hexdigest()
        sha1_file = ROOT / "config" / version / "build.sha1"
        sha1_file.parent.mkdir(parents=True, exist_ok=True)
        sha1_file.write_text(f"{digest}  build/{version}/game.bin\n", newline="\n")
        print(f"{version}: {len(objects)} objects, game.bin {digest}")


if __name__ == "__main__":
    main()

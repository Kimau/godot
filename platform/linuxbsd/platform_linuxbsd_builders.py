"""Functions used to generate source files during build time"""

import subprocess


def make_debug_linuxbsd(target, source, env):
    dst = str(target[0])
    # Use the (possibly cross) binutils resolved in detect.py; the host's
    # bare objcopy/strip can't read a cross-compiled ELF. Fall back to the
    # host tools for native builds.
    objcopy = env.get("OBJCOPY", "objcopy")
    strip = env.get("STRIP", "strip")
    subprocess.run([objcopy, "--only-keep-debug", dst, f"{dst}.debugsymbols"], check=True)
    subprocess.run([strip, "--strip-debug", "--strip-unneeded", dst], check=True)
    subprocess.run([objcopy, f"--add-gnu-debuglink={dst}.debugsymbols", dst], check=True)

#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProgressCategory,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "RM7P4F",  # Europe
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--objdiff",
    metavar="BINARY | DIR",
    type=Path,
    help="path to objdiff-cli binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--ninja",
    metavar="BINARY",
    type=Path,
    help="path to ninja binary (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
parser.add_argument(
    "--warn",
    dest="warn",
    type=str,
    choices=["all", "off", "error"],
    help="how to handle warnings",
)
parser.add_argument(
    "--no-progress",
    dest="progress",
    action="store_false",
    help="disable progress calculation",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.objdiff_path = args.objdiff
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
config.ninja_path = args.ninja
config.progress = args.progress
if not is_windows():
    config.wrapper = args.wrapper
# Don't build asm unless we're --non-matching
if not config.non_matching:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-2"
config.compilers_tag = "20251118"
config.dtk_tag = "v1.8.4"
config.objdiff_tag = "v3.6.1"
config.sjiswrap_tag = "v1.2.2"
config.wibo_tag = "1.0.3"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym BUILD_VERSION={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
]
if args.debug:
    config.ldflags.append("-g")  # Or -gdwarf-2 for Wii linkers
if args.map:
    config.ldflags.append("-mapunused")
    # config.ldflags.append("-listclosure") # For Wii linkers

# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Optional numeric ID for decomp.me preset
# Can be overridden in libraries or objects
config.scratch_preset_id = None

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract off",  # Proven on game code, see notes/COMPILER.md
    "-str reuse",
    "-enc SJIS",  # Shift-JIS sources (Wii-era Nintendo encoding); same objects as -multibyte for current code
    "-i include",
    f"-i build/{config.version}/include",
    "-i libs/Runtime/include",
    "-i libs/MSL_C/include",
    "-i libs/RVL_SDK/include",
    "-i libs/MetroTRK/include",
    f"-DBUILD_VERSION={version_num}",
    f"-DVERSION_{config.version}",
]

# Debug flags
if args.debug:
    # Or -sym dwarf-2 for Wii compilers
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Warning flags
if args.warn == "all":
    cflags_base.append("-W all")
elif args.warn == "off":
    cflags_base.append("-W off")
elif args.warn == "error":
    cflags_base.append("-W error")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

# MSL_C flags (from mscharged-decomp: SDK flags with -ipa file)
cflags_msl = [
    *cflags_base,
    "-inline auto",
    "-ipa file",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-sdata 0",
    "-sdata2 0",
]

# Working hypothesis: game code matches GC 3.0a3 to 3.0a5.2 identically;
# 3.0a5.2 is chosen (SDK build tag 0x4199_60831). See notes/COMPILER.md.
config.linker_version = "GC/3.0a5.2"


# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_base,
        "progress_category": "sdk",
        "objects": objects,
    }


# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rel,
        "progress_category": "game",
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching


# Object is only matching for specific versions
def MatchingFor(*versions):
    return config.version in versions


config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "RVL_SDK",
        # SDK compiler not identified yet (see notes/COMPILER.md); all objects are NonMatching.
        "mw_version": config.linker_version,
        "cflags": cflags_base,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "RVL_SDK/base/PPCArch.c"),
            Object(NonMatching, "RVL_SDK/os/OS.c"),
            Object(NonMatching, "RVL_SDK/os/OSAlarm.c"),
            Object(NonMatching, "RVL_SDK/os/OSAlloc.c"),
            Object(NonMatching, "RVL_SDK/os/OSArena.c"),
            Object(NonMatching, "RVL_SDK/os/OSAudioSystem.c"),
            Object(NonMatching, "RVL_SDK/os/OSCache.c"),
            Object(NonMatching, "RVL_SDK/os/OSContext.c"),
            Object(NonMatching, "RVL_SDK/os/OSError.c"),
            Object(NonMatching, "RVL_SDK/os/OSExec.c"),
            Object(NonMatching, "RVL_SDK/os/OSFatal.c"),
            Object(NonMatching, "RVL_SDK/os/OSFont.c"),
            Object(NonMatching, "RVL_SDK/os/OSInterrupt.c"),
            Object(NonMatching, "RVL_SDK/os/OSLink.c"),
            Object(NonMatching, "RVL_SDK/os/OSMessage.c"),
            Object(NonMatching, "RVL_SDK/os/OSMemory.c"),
            Object(NonMatching, "RVL_SDK/os/OSMutex.c"),
            Object(NonMatching, "RVL_SDK/os/OSReboot.c"),
            Object(NonMatching, "RVL_SDK/os/OSReset.c"),
            Object(NonMatching, "RVL_SDK/os/OSRtc.c"),
            Object(NonMatching, "RVL_SDK/os/OSSync.c"),
            Object(NonMatching, "RVL_SDK/os/OSThread.c"),
            Object(NonMatching, "RVL_SDK/os/OSTime.c"),
            Object(NonMatching, "RVL_SDK/os/OSUtf.c"),
            Object(NonMatching, "RVL_SDK/os/OSIpc.c"),
            Object(NonMatching, "RVL_SDK/os/OSPlayRecord.c"),
            Object(NonMatching, "RVL_SDK/os/OSStateFlags.c"),
            Object(NonMatching, "RVL_SDK/os/OSNet.c"),
            Object(NonMatching, "RVL_SDK/os/OSNandbootInfo.c"),
            Object(NonMatching, "RVL_SDK/os/OSPlayTime.c"),
            Object(NonMatching, "RVL_SDK/os/__ppc_eabi_init.cpp"),
            Object(NonMatching, "RVL_SDK/exi/EXIBios.c"),
            Object(NonMatching, "RVL_SDK/exi/EXIUart.c"),
            Object(NonMatching, "RVL_SDK/exi/EXICommon.c"),
            Object(NonMatching, "RVL_SDK/si/SIBios.c"),
            Object(NonMatching, "RVL_SDK/si/SISamplingRate.c"),
            Object(NonMatching, "RVL_SDK/db/db.c"),
            Object(NonMatching, "RVL_SDK/vi/vi.c"),
            Object(NonMatching, "RVL_SDK/vi/i2c.c"),
            Object(NonMatching, "RVL_SDK/vi/vi3in1.c"),
            Object(NonMatching, "RVL_SDK/gx/GXInit.c"),
            Object(NonMatching, "RVL_SDK/gx/GXFifo.c"),
            Object(NonMatching, "RVL_SDK/gx/GXAttr.c"),
            Object(NonMatching, "RVL_SDK/gx/GXMisc.c"),
            Object(NonMatching, "RVL_SDK/gx/GXGeometry.c"),
            Object(NonMatching, "RVL_SDK/gx/GXTexture.c"),
            Object(NonMatching, "RVL_SDK/gx/GXBump.c"),
            Object(NonMatching, "RVL_SDK/gx/GXTev.c"),
            Object(NonMatching, "RVL_SDK/gx/GXPixel.c"),
            Object(NonMatching, "RVL_SDK/gx/GXTransform.c"),
            Object(NonMatching, "RVL_SDK/gx/GXPerf.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvdfs.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvd.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvdqueue.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvderror.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvdidutils.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvdFatal.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvdDeviceError.c"),
            Object(NonMatching, "RVL_SDK/dvd/dvd_broadway.c"),
            Object(NonMatching, "RVL_SDK/ax/AXCL.c"),
            Object(NonMatching, "RVL_SDK/ax/AXVPB.c"),
            Object(NonMatching, "RVL_SDK/axfx/AXFXReverbHi.c"),
            Object(NonMatching, "RVL_SDK/axfx/AXFXHooks.c"),
            Object(NonMatching, "RVL_SDK/mem/mem_heapCommon.c"),
            Object(NonMatching, "RVL_SDK/mem/mem_list.c"),
            Object(NonMatching, "RVL_SDK/dsp/dsp_task.c"),
            Object(NonMatching, "RVL_SDK/nand/nand.c"),
            Object(NonMatching, "RVL_SDK/nand/NANDOpenClose.c"),
            Object(NonMatching, "RVL_SDK/nand/NANDCore.c"),
            Object(NonMatching, "RVL_SDK/nand/NANDSecret.c"),
            Object(NonMatching, "RVL_SDK/nand/NANDCheck.c"),
            Object(NonMatching, "RVL_SDK/nand/NANDLogging.c"),
            Object(NonMatching, "RVL_SDK/sc/scsystem.c"),
            Object(NonMatching, "RVL_SDK/sc/scapi.c"),
            Object(NonMatching, "RVL_SDK/sc/scapi_prdinfo.c"),
            Object(NonMatching, "RVL_SDK/arc/arc.c"),
            Object(NonMatching, "RVL_SDK/ipc/ipcMain.c"),
            Object(NonMatching, "RVL_SDK/ipc/ipcclt.c"),
            Object(NonMatching, "RVL_SDK/ipc/memory.c"),
            Object(NonMatching, "RVL_SDK/ipc/ipcProfile.c"),
            Object(NonMatching, "RVL_SDK/fs/fs.c"),
            Object(NonMatching, "RVL_SDK/pad/Pad.c"),
            Object(NonMatching, "RVL_SDK/wpad/WPAD.c"),
            Object(NonMatching, "RVL_SDK/wpad/WPADHIDParser.c"),
            Object(NonMatching, "RVL_SDK/wpad/WPADEncrypt.c"),
            Object(NonMatching, "RVL_SDK/euart/euart.c"),
            Object(NonMatching, "RVL_SDK/usb/usb.c"),
            Object(NonMatching, "RVL_SDK/wud/WUD.c"),
            Object(NonMatching, "RVL_SDK/wud/WUDHidHost.c"),
            Object(NonMatching, "RVL_SDK/bte/gki_buffer.c"),
            Object(NonMatching, "RVL_SDK/bte/gki_time.c"),
            Object(NonMatching, "RVL_SDK/bte/gki_ppc.c"),
            Object(NonMatching, "RVL_SDK/bte/hcisu_h2.c"),
            Object(NonMatching, "RVL_SDK/bte/uusb_ppc.c"),
            Object(NonMatching, "RVL_SDK/bte/bte_hcisu.c"),
            Object(NonMatching, "RVL_SDK/bte/bte_init.c"),
            Object(NonMatching, "RVL_SDK/bte/bte_logmsg.c"),
            Object(NonMatching, "RVL_SDK/bte/bte_main.c"),
            Object(NonMatching, "RVL_SDK/bte/btu_task1.c"),
            Object(NonMatching, "RVL_SDK/bte/bd.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_sys_conn.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_sys_main.c"),
            Object(NonMatching, "RVL_SDK/bte/ptim.c"),
            Object(NonMatching, "RVL_SDK/bte/utl.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_dm_act.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_dm_api.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_dm_main.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_dm_pm.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_hh_act.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_hh_api.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_hh_main.c"),
            Object(NonMatching, "RVL_SDK/bte/bta_hh_utils.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_acl.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_dev.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_devctl.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_discovery.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_inq.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_main.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_pm.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_sco.c"),
            Object(NonMatching, "RVL_SDK/bte/btm_sec.c"),
            Object(NonMatching, "RVL_SDK/bte/btu_hcif.c"),
            Object(NonMatching, "RVL_SDK/bte/btu_init.c"),
            Object(NonMatching, "RVL_SDK/bte/wbt_ext.c"),
            Object(NonMatching, "RVL_SDK/bte/gap_api.c"),
            Object(NonMatching, "RVL_SDK/bte/gap_conn.c"),
            Object(NonMatching, "RVL_SDK/bte/gap_utils.c"),
            Object(NonMatching, "RVL_SDK/bte/hcicmds.c"),
            Object(NonMatching, "RVL_SDK/bte/hidd_api.c"),
            Object(NonMatching, "RVL_SDK/bte/hidd_conn.c"),
            Object(NonMatching, "RVL_SDK/bte/hidd_mgmt.c"),
            Object(NonMatching, "RVL_SDK/bte/hidd_pm.c"),
            Object(NonMatching, "RVL_SDK/bte/hidh_api.c"),
            Object(NonMatching, "RVL_SDK/bte/hidh_conn.c"),
            Object(NonMatching, "RVL_SDK/bte/l2c_api.c"),
            Object(NonMatching, "RVL_SDK/bte/l2c_csm.c"),
            Object(NonMatching, "RVL_SDK/bte/l2c_link.c"),
            Object(NonMatching, "RVL_SDK/bte/l2c_main.c"),
            Object(NonMatching, "RVL_SDK/bte/l2c_utils.c"),
            Object(NonMatching, "RVL_SDK/bte/port_api.c"),
            Object(NonMatching, "RVL_SDK/bte/port_rfc.c"),
            Object(NonMatching, "RVL_SDK/bte/port_utils.c"),
            Object(NonMatching, "RVL_SDK/bte/rfc_l2cap_if.c"),
            Object(NonMatching, "RVL_SDK/bte/rfc_mx_fsm.c"),
            Object(NonMatching, "RVL_SDK/bte/rfc_port_fsm.c"),
            Object(NonMatching, "RVL_SDK/bte/rfc_port_if.c"),
            Object(NonMatching, "RVL_SDK/bte/rfc_ts_frames.c"),
            Object(NonMatching, "RVL_SDK/bte/rfc_utils.c"),
            Object(NonMatching, "RVL_SDK/bte/sdp_api.c"),
            Object(NonMatching, "RVL_SDK/bte/sdp_db.c"),
            Object(NonMatching, "RVL_SDK/bte/sdp_discovery.c"),
            Object(NonMatching, "RVL_SDK/bte/sdp_main.c"),
            Object(NonMatching, "RVL_SDK/bte/sdp_server.c"),
            Object(NonMatching, "RVL_SDK/bte/sdp_utils.c"),
            Object(NonMatching, "RVL_SDK/tpl/TPL.c"),
            Object(NonMatching, "RVL_SDK/dwc/dwc_common.c"),
            Object(NonMatching, "RVL_SDK/gamespy/darray.c"),
            Object(NonMatching, "RVL_SDK/gamespy/hashtable.c"),
            Object(NonMatching, "RVL_SDK/gamespy/GP/gpiInfo.c"),
            Object(NonMatching, "RVL_SDK/gamespy/GP/gpiUnique.c"),
            Object(NonMatching, "RVL_SDK/gamespy/GP/gpiUtility.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Auth.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Buffer.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Callback.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Connection.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Main.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Message.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Socket.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gt2/gt2Utility.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gstats/gbucket.c"),
            Object(NonMatching, "RVL_SDK/gamespy/gstats/gstats.c"),
            Object(NonMatching, "RVL_SDK/gamespy/natneg/NATify.c"),
            Object(NonMatching, "RVL_SDK/gamespy/natneg/natneg.c"),
            Object(NonMatching, "RVL_SDK/gamespy/serverbrowsing/sb_crypt.c"),
            Object(NonMatching, "RVL_SDK/gamespy/serverbrowsing/sb_queryengine.c"),
            Object(NonMatching, "RVL_SDK/gamespy/serverbrowsing/sb_server.c"),
            Object(NonMatching, "RVL_SDK/gamespy/serverbrowsing/sb_serverbrowsing.c"),
        ],
    },
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",  # str | List[str]
        "objects": [
            Object(Matching, "Runtime.PPCEABI.H/__mem.c"),
            Object(Matching, "Runtime.PPCEABI.H/__va_arg.c"),
            Object(Matching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(Matching, "Runtime.PPCEABI.H/NMWException.cpp", extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "Runtime.PPCEABI.H/ptmf.c"),
            Object(Matching, "Runtime.PPCEABI.H/runtime.c"),
            Object(Matching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
            Object(Matching, "Runtime.PPCEABI.H/Gecko_ExceptionPPC.cpp"),
            Object(Matching, "Runtime.PPCEABI.H/GCN_mem_alloc.c"),
        ],
    },
    {
        "lib": "MSL_C",
        # Per file, proven by byte comparison (see notes/COMPILER.md): printf.c needs 3.0a5.2,
        # six files need 3.0a5 (set on their objects), the others match with both.
        "mw_version": "GC/3.0a5.2",
        "cflags": cflags_msl,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "MSL_C/alloc.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/ansi_files.c", extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "MSL_C/ansi_fp.c", extra_cflags=["-Cpp_exceptions on", "-rostr", "-use_lmw_stmw on"], mw_version="GC/3.0a5"),
            Object(NonMatching, "MSL_C/arith.c"),
            Object(Matching, "MSL_C/buffer_io.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/char_io.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/direct_io.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/file_io.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/FILE_POS.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/mbstring.c", extra_cflags=["-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/mem.c"),
            Object(Matching, "MSL_C/mem_funcs.c"),
            Object(Matching, "MSL_C/math_api.c"),
            Object(Matching, "MSL_C/misc_io.c"),
            Object(Matching, "MSL_C/printf.c", extra_cflags=["-Cpp_exceptions on", "-str reuse,pool,readonly", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/qsort.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/rand.c"),
            Object(Matching, "MSL_C/scanf.c", extra_cflags=["-Cpp_exceptions on", "-str reuse,pool,readonly", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/signal.c", extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "MSL_C/string.c", extra_cflags=["-Cpp_exceptions on", "-str reuse,pool,readonly"]),
            Object(Matching, "MSL_C/strtold.c", extra_cflags=["-Cpp_exceptions on", "-str reuse,pool,readonly", "-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/strtoul.c", extra_cflags=["-Cpp_exceptions on", "-use_lmw_stmw on"], mw_version="GC/3.0a5"),
            Object(Matching, "MSL_C/wstring.c"),
            Object(Matching, "MSL_C/wchar_io.c"),
            Object(Matching, "MSL_C/uart_console_io_gcn.c", extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "MSL_C/abort_exit_ppc_eabi.c", extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "MSL_C/math_sun.c", cflags=cflags_runtime, extra_cflags=["-Cpp_exceptions on"]),
            Object(Matching, "MSL_C/extras.c"),
            Object(Matching, "MSL_C/e_acos.c"),
            Object(Matching, "MSL_C/e_asin.c", mw_version="GC/3.0a5"),
            Object(Matching, "MSL_C/e_atan2.c"),
            Object(Matching, "MSL_C/e_fmod.c"),
            Object(Matching, "MSL_C/e_log.c"),
            Object(Matching, "MSL_C/e_log10.c"),
            Object(Matching, "MSL_C/e_pow.c", mw_version="GC/3.0a5"),
            Object(Matching, "MSL_C/e_rem_pio2.c", mw_version="GC/3.0a5"),
            Object(Matching, "MSL_C/k_cos.c"),
            Object(Matching, "MSL_C/k_rem_pio2.c", extra_cflags=["-use_lmw_stmw on"]),
            Object(Matching, "MSL_C/k_sin.c"),
            Object(Matching, "MSL_C/k_tan.c", mw_version="GC/3.0a5"),
            Object(Matching, "MSL_C/s_atan.c", mw_version="GC/3.0a5"),
            Object(Matching, "MSL_C/s_ceil.c"),
            Object(Matching, "MSL_C/s_copysign.c"),
            Object(Matching, "MSL_C/s_cos.c"),
            Object(Matching, "MSL_C/s_floor.c"),
            Object(Matching, "MSL_C/s_frexp.c"),
            Object(Matching, "MSL_C/s_ldexp.c"),
            Object(Matching, "MSL_C/s_sin.c"),
            Object(Matching, "MSL_C/s_tan.c"),
            Object(Matching, "MSL_C/w_acos.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/w_asin.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/w_atan2.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/w_fmod.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/w_log.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/w_log10.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/w_pow.c", extra_cflags=["-D_IEEE_LIBM"]),
            Object(Matching, "MSL_C/e_sqrt.c"),
            Object(Matching, "MSL_C/math_ppc.c"),
            Object(Matching, "MSL_C/w_sqrt.c", extra_cflags=["-D_IEEE_LIBM"]),
        ],
    },
    {
        "lib": "MetroTRK",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "progress_category": "sdk",
        "objects": [
            Object(NonMatching, "MetroTRK/mainloop.c"),
            Object(NonMatching, "MetroTRK/nubevent.c"),
            Object(NonMatching, "MetroTRK/nubinit.c"),
            Object(NonMatching, "MetroTRK/msg.c"),
            Object(NonMatching, "MetroTRK/msgbuf.c"),
            Object(NonMatching, "MetroTRK/serpoll.c"),
            Object(NonMatching, "MetroTRK/usr_put.c"),
            Object(NonMatching, "MetroTRK/dispatch.c"),
            Object(NonMatching, "MetroTRK/msghndlr.c"),
            Object(NonMatching, "MetroTRK/support.c"),
            Object(NonMatching, "MetroTRK/mutex_TRK.c"),
            Object(NonMatching, "MetroTRK/notify.c"),
            Object(NonMatching, "MetroTRK/flush_cache.c"),
            Object(NonMatching, "MetroTRK/mem_TRK.c"),
            Object(NonMatching, "MetroTRK/string_TRK.c"),
            Object(NonMatching, "MetroTRK/targimpl.c"),
            Object(NonMatching, "MetroTRK/targsupp.c"),
            Object(NonMatching, "MetroTRK/mpc_7xx_603e.c"),
            Object(NonMatching, "MetroTRK/mslsupp.c"),
            Object(NonMatching, "MetroTRK/__exception.s"),
            Object(NonMatching, "MetroTRK/dolphin_trk.c"),
            Object(NonMatching, "MetroTRK/main_TRK.c"),
            Object(NonMatching, "MetroTRK/dolphin_trk_glue.c"),
            Object(NonMatching, "MetroTRK/targcont.c"),
            Object(NonMatching, "MetroTRK/target_options.c"),
            Object(NonMatching, "MetroTRK/UDP_Stubs.c"),
            Object(NonMatching, "MetroTRK/cc_gdev.c"),
            Object(NonMatching, "MetroTRK/CircleBuffer.c"),
            Object(NonMatching, "MetroTRK/MWCriticalSection_gc.c"),
        ],
    },
]


# Optional callback to adjust link order. This can be used to add, remove, or reorder objects.
# This is called once per module, with the module ID and the current link order.
#
# For example, this adds "dummy.c" to the end of the DOL link order if configured with --non-matching.
# "dummy.c" *must* be configured as a Matching (or Equivalent) object in order to be linked.
def link_order_callback(module_id: int, objects: List[str]) -> List[str]:
    # Don't modify the link order for matching builds
    if not config.non_matching:
        return objects
    if module_id == 0:  # DOL
        return objects + ["dummy.c"]
    return objects


# Uncomment to enable the link order callback.
# config.link_order_callback = link_order_callback


# Optional extra categories for progress tracking
# Adjust as desired for your project
config.progress_categories = [
    ProgressCategory("game", "Game Code"),
    ProgressCategory("sdk", "SDK Code"),
]
config.progress_each_module = args.verbose
# Optional extra arguments to `objdiff-cli report generate`
config.progress_report_args = [
    # Marks relocations as mismatching if the target value is different
    # Default is "functionRelocDiffs=none", which is most lenient
    # "--config functionRelocDiffs=data_value",
]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress information
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)

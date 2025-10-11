#!/usr/bin/env python3

"""
pip install psutil py-cpuinfo
"""

import json, shutil, subprocess, sys, platform, psutil, cpuinfo
from datetime import datetime


def bytes_to_gb(n):
    return round(n / (1024**3), 2)


def run_cmd(cmd, timeout=10):
    try:
        p = subprocess.run(
            cmd,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            timeout=timeout,
            shell=False
        )
        out = (p.stdout or "") + ("\n" + p.stderr if p.stderr else "")
        out = out.strip()
        if p.returncode != 0 and not out:
            return None
        return out
    except (FileNotFoundError, subprocess.TimeoutExpired):
        return None


def which(name):
    return shutil.which(name) is not None


def probe_version(binary, args=["--version"], take_first_line=True):
    if not which(binary):
        return None
    if binary.lower() == "cl" and args == ["--version"]:
        raw = run_cmd(["cl"])
    else:
        raw = run_cmd([binary, *args])
    if not raw:
        return None
    return raw.splitlines()[0] if take_first_line else raw


def os_info():
    return {
        "system": platform.system(),
        "release": platform.release(),
        "version": platform.version(),
        "platform": platform.platform(),
        "machine": platform.machine(),
        "python": sys.version.split()[0]
    }


def cpu_info():
    ci = cpuinfo.get_cpu_info()
    return {
        "brand": ci.get("brand_raw"),
        "arch": ci.get("arch_string_raw"),
        "bits": ci.get("bits"),
        "hz_advertised": ci.get("hz_advertised_friendly"),
        "hz_actual": ci.get("hz_actual_friendly"),
        "cores_physical": psutil.cpu_count(logical=False),
        "cores_logical": psutil.cpu_count(logical=True)
    }


def ram_info():
    vm = psutil.virtual_memory()
    sm = getattr(psutil, "swap_memory", lambda: None)()
    return {
        "total_gb": bytes_to_gb(vm.total),
        "available_gb": bytes_to_gb(vm.available),
        "swap_gb": bytes_to_gb(sm.total) if sm else None
    }


def disks_info():
    items = []
    for part in psutil.disk_partitions(all=False):
        try:
            usage = psutil.disk_usage(part.mountpoint)
        except Exception:
            continue
        items.append({
            "device": part.device,
            "mountpoint": part.mountpoint,
            "fstype": part.fstype,
            "total_gb": bytes_to_gb(usage.total),
            "used_gb": bytes_to_gb(usage.used),
            "free_gb": bytes_to_gb(usage.free)
        })
    return items


def main():
    report = {
        "generated_at": datetime.utcnow().isoformat() + "Z",
        "os": os_info(),
        "cpu": cpu_info(),
        "memory": ram_info(),
        "disks": disks_info()
    }
    print(json.dumps(report, ensure_ascii=False, indent=2))


if __name__ == "__main__":
    main()


#!/usr/bin/env bash

# proc_inspector.sh
# Usage: ./proc_inspector.sh <PID>

PID="$1"

if [ -z "$PID" ]; then
    echo "Usage: $0 <PID>"
    exit 1
fi

if [ ! -d "/proc/$PID" ]; then
    echo "Process $PID does not exist."
    exit 1
fi

echo "PID: $PID"

echo "Command: $(tr '\0' ' ' < "/proc/$PID/cmdline")"

echo "Status:"
grep -E '^(Name|State|PPid|Uid|Gid|Threads|VmSize|VmRSS):' \
    "/proc/$PID/status"

echo "Executable: $(readlink "/proc/$PID/exe")"

echo "Open file descriptors: $(ls "/proc/$PID/fd" 2>/dev/null | wc -l)"

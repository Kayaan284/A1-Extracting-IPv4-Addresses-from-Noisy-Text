"""Check exact sample output, termination, failure reset, and EOF behavior."""
import subprocess
import sys

exe = sys.argv[1]
prompt = "Enter a string (or 'END' to quit): "
cases = [
    ("connecting to 192.168.1.1 now", "192.168.1.1", 3232235777, "none"),
    ("server=10.0.0.255:8080end", "10.0.0.255", 167772415, "8080"),
    ("192a168.1.1.1", "168.1.1.1", 2818638081, "none"),
    ("192.168.1.1.", None, 0, "none"),
    ("Connection from 192.168.1.1 refused", "192.168.1.1", 3232235777, "none"),
    ("192.168.01.1", None, 0, "none"),
    ("1.2.3.4:99999", None, 0, "none"),
    ("12.34.56", None, 0, "none"),
    ("no number here", None, 0, "none"),
]
expected = ""
for _, address, number, port in cases:
    expected += prompt
    expected += (f"Extracted IPv4 address: {address} (decimal value: {number}, port: {port})\n"
                 if address else "Invalid input: no valid IPv4 address found\n")
expected += prompt + "Program terminated.\n"

def run(text):
    return subprocess.run([exe], input=text, text=True, capture_output=True, check=True).stdout

assert run("\n".join(row[0] for row in cases) + "\nEND\n1.2.3.4\n") == expected
assert run("end\nEND\n") == prompt + "Invalid input: no valid IPv4 address found\n" + prompt + "Program terminated.\n"
assert run("") == prompt + "Program terminated.\n"
print("Passed 3 exact-output CLI checks.")

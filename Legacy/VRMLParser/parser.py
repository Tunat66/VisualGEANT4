from wrlparser import parse
import sys

file = sys.argv[1]
with open(file) as f:
    l = "".join(f.readlines())
    f.close()

scene = parse(l)
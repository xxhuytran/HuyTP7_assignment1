# Sinh du lieu mau: python3 tools/gen_data.py
import random

random.seed(42)
SENSORS = [("T1", "TEMP"), ("T2", "TEMP"), ("V1", "VOLT"), ("V2", "VOLT"), ("S1", "SPEED"), ("S2", "SPEED")]


def value(kind, prev):
    if kind == "TEMP":
        return round(random.uniform(20, 110), 1)
    if kind == "VOLT":
        return round(random.uniform(8, 17), 2)
    base = prev if prev is not None else 60
    return round(min(300, max(0, base + random.uniform(-60, 60))), 1)


def gen(path, n, bad_every=0):
    prev = {}
    ts = 1000
    with open(path, "w") as f:
        for i in range(n):
            sid, kind = random.choice(SENSORS)
            ts += random.randint(0, 20)  # co khi trung timestamp
            v = value(kind, prev.get(sid))
            prev[sid] = v
            line = "%d;%s;%s;%s" % (ts, sid, kind, v)
            if bad_every and i % bad_every == bad_every - 1:
                line = random.choice(["garbage", "%d;%s;%s;abc" % (ts, sid, kind),
                                      "%d;%s;TEMP;999" % (ts, sid), "%d;%s;FOO;1" % (ts, sid)])
            f.write(line + "\n")


gen("data/sensor_log.txt", 200, 25)
gen("data/large.txt", 120000, 1000)
open("data/empty.txt", "w").close()
with open("data/alarms.txt", "w") as f:
    f.write("1000;T1;TEMP;110\n2000;V1;VOLT;8.5\n3000;S1;SPEED;40\n4000;S1;SPEED;100\n")
with open("data/errors.txt", "w") as f:
    f.write("1000;T1;TEMP;50\nbad line\n2000;T1;TEMP;abc\n3000;T1;TEMP;200\n4000;T1;TEMP;60\n5000;T1;FOO;1\n")

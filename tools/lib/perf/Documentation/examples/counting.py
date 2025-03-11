#!/usr/bin/python3

import sys
sys.path.append('../../')
from libperf import *

# software ids
PERF_COUNT_SW_CPU_CLOCK = 0
PERF_COUNT_SW_TASK_CLOCK = 1

# Perf event types
PERF_TYPE_HARDWARE = 0
PERF_TYPE_SOFTWARE = 1
PERF_TYPE_TRACEPOINT = 2
PERF_TYPE_HW_CACHE = 3

# perf_event_attr_read format
PERF_FORMAT_TOTAL_TIME_ENABLED = 1 << 0
PERF_FORMAT_TOTAL_TIME_RUNNING = 1 << 1
PERF_FORMAT_ID = 1 << 2
PERF_FORMAT_GROUP = 1 << 3
PERF_FORMAT_LOST = 1 << 4

# Perf sample identifier
PERF_SAMPLE_IDENTIFIER = 1 << 16

def get_attr(config):
    attr = perf_event_attr()
    attr.type = PERF_TYPE_SOFTWARE
    attr.config = config
    attr.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING
    attr.disabled = 1
    attr.size = 136
    attr.sample_type = PERF_SAMPLE_IDENTIFIER
    return attr

libperf_init(None)
threads = perf_thread_map__new_dummy()
assert(threads)
perf_thread_map__set_pid(threads, 0, 0)

evlist = perf_evlist__new()
assert(evlist)

attr1 = get_attr(PERF_COUNT_SW_CPU_CLOCK)
evsel = perf_evsel__new(attr1)
assert(evsel)
perf_evlist__add(evlist, evsel)

attr2 = get_attr(PERF_COUNT_SW_TASK_CLOCK)
evsel = perf_evsel__new(attr2)
assert(evsel)
perf_evlist__add(evlist, evsel)

perf_evlist__set_maps(evlist, None, threads)
rc = perf_evlist__open(evlist)
if rc != 0:
    print("failed to open evsel: ", rc)

perf_evlist__enable(evlist)

count = 100000
while count >= 0:
    count-=1

perf_evlist__disable(evlist)
c = perf_counts_values()
for sel  in evlist:
		perf_evsel__read(sel, 0, 0, c);
		print("count %lu, enabled %lu, run %lu" %(c.val, c.ena, c.run))

perf_evlist__close(evlist);
perf_evlist__delete(evlist);
perf_thread_map__put(threads);

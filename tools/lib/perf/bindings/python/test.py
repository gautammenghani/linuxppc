#!/usr/bin/env python3

import sys

sys.path.append("/home/gautam/work/github_repos/linux_upstream/tools/lib/perf/bindings/python/raw_pylibperf_wrap.so")

from raw_pylibperf import *
from enum import Enum
import ctypes as ct
import ctypes.util

libc = ct.CDLL(ctypes.util.find_library("libc"))

libperf_init(libc.vprintf)
threads = perf_thread_map__new_dummy()
assert(threads)

perf_thread_map__set_pid(threads, 0, 0)
evlist = perf_evlist__new()
assert(evlist)

# create perf_event_attr
attr1 = perf_event_attr()
attr1.type        = PERF_TYPE_SOFTWARE
attr1.config      = PERF_COUNT_SW_CPU_CLOCK
attr1.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING
attr1.disabled    = 1
attr1.size=136
evsel1 = perf_evsel__new(attr1)
assert(evsel1)
perf_evlist__add(evlist, evsel1);

attr2 = perf_event_attr()
attr2.type        = PERF_TYPE_SOFTWARE
attr2.config      = PERF_COUNT_SW_TASK_CLOCK
attr2.read_format = PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING
attr2.disabled    = 1
attr2.size=136
evsel1 = perf_evsel__new(attr1)
evsel2 = perf_evsel__new(attr2)
assert(evsel2)
perf_evlist__add(evlist, evsel2)
perf_evlist__set_maps(evlist, None, threads)
err = perf_evlist__open(evlist)
assert(err == 0)
for i in range(10000):
        continue
perf_evlist__disable(evlist);
counts = perf_counts_values()
for evsel in evlist:
        perf_evsel__read(evsel, 0, 0, counts)
        print(f"count {counts.val}, enabled {counts.ena}, run {counts.run}\n")
perf_evlist__close(evlist);
perf_evlist__delete(evlist);
perf_thread_map__put(threads);
'''
if (err) {
        fprintf(stderr, "failed to open evsel\n");
        goto out_evlist;
}
perf_evlist__enable(evlist);
while (count--);
perf_evlist__disable(evlist);
perf_evlist__for_each_evsel(evlist, evsel) {
        perf_evsel__read(evsel, 0, 0, &counts);
        fprintf(stdout, "count %llu, enabled %llu, run %llu\n",
                        counts.val, counts.ena, counts.run);
}
perf_evlist__close(evlist);
out_evlist:
perf_evlist__delete(evlist);
out_threads:
perf_thread_map__put(threads);
return err;
'''

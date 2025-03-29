%module raw_pylibperf
%{
#include <linux/perf_event.h>
#include <perf/evlist.h>
#include <perf/evsel.h>
#include <perf/cpumap.h>
#include <perf/threadmap.h>
#include <perf/mmap.h>
#include <perf/core.h>
#include <perf/event.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
%}

%include <stdint.i>

typedef int pid_t;
struct perf_event_attr {

	uint32_t			type;

	uint32_t			size;

	uint64_t			config;

	union {
		uint64_t		sample_period;
		uint64_t		sample_freq;
	};

	uint64_t			sample_type;
	uint64_t			read_format;

	uint64_t			disabled       :  1,
				inherit	       :  1,
				pinned	       :  1,
				exclusive      :  1,
				exclude_user   :  1,
				exclude_kernel :  1,
				exclude_hv     :  1,
				exclude_idle   :  1,
				mmap           :  1,
				comm	       :  1,
				freq           :  1,
				inherit_stat   :  1,
				enable_on_exec :  1,
				task           :  1,
				watermark      :  1,
				precise_ip     :  2,
				mmap_data      :  1,
				sample_id_all  :  1,

				exclude_host   :  1,
				exclude_guest  :  1,

				exclude_callchain_kernel : 1,
				exclude_callchain_user   : 1,
				mmap2          :  1,
				comm_exec      :  1,
				use_clockid    :  1,
				context_switch :  1,
				write_backward :  1,
				namespaces     :  1,
				ksymbol        :  1,
				bpf_event      :  1,
				aux_output     :  1,
				cgroup         :  1,
				text_poke      :  1,
				build_id       :  1,
				inherit_thread :  1,
				remove_on_exec :  1,
				sigtrap        :  1,
				__reserved_1   : 26;

	union {
		uint32_t		wakeup_events;
		uint32_t		wakeup_watermark;
	};

	uint32_t			bp_type;
	union {
		uint64_t		bp_addr;
		uint64_t		kprobe_func;
		uint64_t		uprobe_path;
		uint64_t		config1;
	};
	union {
		uint64_t		bp_len;
		uint64_t		kprobe_addr;
		uint64_t		probe_offset;
		uint64_t		config2;
	};
	uint64_t	branch_sample_type;

	uint64_t	sample_regs_user;

	uint32_t	sample_stack_user;

	int32_t	clockid;

	uint64_t	sample_regs_intr;

	uint32_t	aux_watermark;
	uint16_t	sample_max_stack;
	uint16_t	__reserved_2;
	uint32_t	aux_sample_size;

	uint64_t	sig_data;
	uint64_t	config3;
};

enum perf_type_id {
	PERF_TYPE_HARDWARE			= 0,
	PERF_TYPE_SOFTWARE			= 1,
	PERF_TYPE_TRACEPOINT			= 2,
	PERF_TYPE_HW_CACHE			= 3,
	PERF_TYPE_RAW				= 4,
	PERF_TYPE_BREAKPOINT			= 5,

	PERF_TYPE_MAX,				/* non-ABI */
};
enum perf_sw_ids {
	PERF_COUNT_SW_CPU_CLOCK			= 0,
	PERF_COUNT_SW_TASK_CLOCK		= 1,
	PERF_COUNT_SW_PAGE_FAULTS		= 2,
	PERF_COUNT_SW_CONTEXT_SWITCHES		= 3,
	PERF_COUNT_SW_CPU_MIGRATIONS		= 4,
	PERF_COUNT_SW_PAGE_FAULTS_MIN		= 5,
	PERF_COUNT_SW_PAGE_FAULTS_MAJ		= 6,
	PERF_COUNT_SW_ALIGNMENT_FAULTS		= 7,
	PERF_COUNT_SW_EMULATION_FAULTS		= 8,
	PERF_COUNT_SW_DUMMY			= 9,
	PERF_COUNT_SW_BPF_OUTPUT		= 10,
	PERF_COUNT_SW_CGROUP_SWITCHES		= 11,

	PERF_COUNT_SW_MAX,			/* non-ABI */
};

enum perf_event_read_format {
	PERF_FORMAT_TOTAL_TIME_ENABLED		= 1U << 0,
	PERF_FORMAT_TOTAL_TIME_RUNNING		= 1U << 1,
	PERF_FORMAT_ID				= 1U << 2,
	PERF_FORMAT_GROUP			= 1U << 3,
	PERF_FORMAT_LOST			= 1U << 4,

	PERF_FORMAT_MAX = 1U << 5,		/* non-ABI */
};

struct perf_counts_values {
	union {
		struct {
			uint64_t val;
			uint64_t ena;
			uint64_t run;
			uint64_t id;
			uint64_t lost;
		};
		uint64_t values[5];
	};
};

// TODO: This is not the same as libperf defn
void libperf_init(...);
struct perf_thread_map *perf_thread_map__new_dummy(void);
void perf_thread_map__set_pid(struct perf_thread_map *map, int idx, pid_t pid);
struct perf_evlist *perf_evlist__new(void);
struct perf_evsel *perf_evsel__new(struct perf_event_attr *attr);
void perf_evlist__add(struct perf_evlist *evlist,struct perf_evsel *evsel);
void perf_evlist__set_maps(struct perf_evlist *evlist,
				       struct perf_cpu_map *cpus,
				       struct perf_thread_map *threads);
int perf_evlist__open(struct perf_evlist *evlist);
void perf_evlist__close(struct perf_evlist *evlist);
void perf_evlist__enable(struct perf_evlist *evlist);
void perf_evlist__disable(struct perf_evlist *evlist);


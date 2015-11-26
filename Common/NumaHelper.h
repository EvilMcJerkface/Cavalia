#pragma once
#ifndef __COMMON_NUMA_HELPER_H__
#define __COMMON_NUMA_HELPER_H__

#include <iostream>
#include <vector>
#if defined(__linux__)
#include <numa.h>
#endif

#if defined(__linux__)
struct NumaTopology{
	NumaTopology(){
		// number of cores in the machine.
		max_core_count_ = numa_num_task_cpus();
		// number of numa nodes in the machine.
		max_node_count_ = numa_num_task_nodes();
		// number of cores in each numa node.
		core_per_node_ = max_core_count_ / max_node_count_;

		cores_.resize(max_node_count_);

		bitmask *bm = numa_bitmask_alloc(max_core_count_);
		for (size_t i = 0; i < max_node_count_; ++i) {
			numa_node_to_cpus(i, bm);
			for (size_t j = 0; j < max_core_count_; ++j) {
				if (numa_bitmask_isbitset(bm, j)) {
					cores_[i].push_back(j);
				}
			}
		}
		numa_bitmask_free(bm);
	}

	void Print(){
		std::cout << "max core count = " << max_core_count_ << std::endl;
		std::cout << "max node count = " << max_node_count_ << std::endl;
		std::cout << "core per node = " << core_per_node_ << std::endl;
	}

	size_t max_core_count_;
	size_t max_node_count_;
	size_t core_per_node_;
	std::vector<std::vector<size_t>> cores_;
};
#endif

static size_t GetNumaNodeId(const size_t &core_id){
#if defined(__linux__)
	return numa_node_of_cpu(core_id);
#else
	return 0;
#endif
}

#endif
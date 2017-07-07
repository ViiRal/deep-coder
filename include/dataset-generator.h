#pragma once
#include "parameters.h"

#include <vector>
#include <utility>
#include <unordered_map>
#include "dsl/type.h"
#include "example-generator.h"

using ProgramType = std::pair<std::vector<dsl::Type>, dsl::Type>;

namespace std {
    template <>
    struct hash<std::vector<dsl::Type>> {
        size_t operator()(const std::vector<dsl::Type> &input) const {
            size_t retval = 0;
            auto hash_for_type = hash<dsl::Type>();
            auto hash_for_size_t = hash<size_t>();
            for (auto i = 0; i < input.size(); i++) {
                retval ^= hash_for_type(input.at(i)) ^ hash_for_size_t(i);
            }

            return retval;
        }
    };

    template <>
    struct hash<ProgramType> {
        size_t operator ()(const ProgramType &p) const {
            return hash<decltype(p.first)>()(p.first) ^ hash<decltype(p.second)>()(p.second);
        }
    };

}

struct Dataset {
    std::unordered_map<ProgramType, std::vector<std::pair<dsl::Program, std::vector<Example>>>> programs;
    size_t size;

    Dataset();
    void insert(const dsl::Program &p, const std::vector<Example> &examples);
};

bool has_unused_variable(const dsl::Program &p);

std::experimental::optional<Dataset> generate_dataset(size_t min_length, size_t max_length, size_t dataset_size, size_t example_per_program = EXAMPLE_NUM);
#ifndef MUNICIPALITY_PROCESSOR_H
#define MUNICIPALITY_PROCESSOR_H

#include "Municipality.h"
#include <vector>
#include <functional>

class MunicipalityProcessor {
public:
    static std::vector<Municipality> filterMunicipalities(
        const std::vector<Municipality>& municipalities,
        std::function<bool(const Municipality&)> predicate);
};

#endif

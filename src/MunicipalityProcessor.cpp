#include "MunicipalityProcessor.h"
#include <algorithm>

std::vector<Municipality> MunicipalityProcessor:: filterMunicipalities(
    const std::vector<Municipality>& municipalities,
    std::function<bool(const Municipality&)> predicate) {

    std::vector<Municipality> result;
    std::copy_if(municipalities.begin(), municipalities.end(), std::back_inserter(result), predicate);
    return result;
}

#include <casacore/tables/Tables/TableDesc.h>
#include <casacore/tables/Tables/SetupNewTab.h>
#include <casacore/tables/Tables/ScaColDesc.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/tables/Tables/ArrColDesc.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/DataMan/Adios2StMan.h>
#include <casacore/tables/DataMan/AdiosStMan.h>
#include <casacore/tables/DataMan/Hdf5StMan.h>
#include <casacore/casa/namespace.h>
#include <mpi.h>
#include <cmath>
#include "json.hpp"

using namespace std;
using namespace nlohmann;

size_t cell_size_min = 10;
size_t cell_size_max = 10;
size_t columns_min = 5;
size_t columns_max = 5;
size_t rows_min = 5;
size_t rows_max = 5;

template<class T>
void GenData(Array<T> &arr, IPosition pos, uInt row){
    arr = row + 1;
}




#include <casacore/tables/Tables/TableDesc.h>
#include <casacore/tables/Tables/SetupNewTab.h>
#include <casacore/tables/Tables/ScaColDesc.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/tables/Tables/ArrColDesc.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/DataMan/Adios2StMan.h>
#include <casacore/casa/namespace.h>
#include <mpi.h>

#include "common.h"
#include "json.hpp"

using namespace std;
using namespace nlohmann;

int main(int argc, char **argv){

    int mpiRank, mpiSize;
    int mpi_provided_mode;
    MPI_Init_thread(&argc,&argv, MPI_THREAD_MULTIPLE, &mpi_provided_mode);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    std::string filename = "test.table";
    int cell_size = 64;
    int rows = 100;
    int columns = 1;
    std::string stman_type = "Adios2StMan";

    if(argc > 1)
        cell_size = atoi(argv[1]);

    if(argc > 2)
        rows = atoi(argv[2]);

    if(argc > 3)
        columns = atoi(argv[3]);

    if(argc > 4)
        stman_type = argv[4];

    if(argc > 5)
        filename = argv[5];

    auto start_time = std::chrono::system_clock::now();
    auto end_time = std::chrono::system_clock::now();

    {
        DataManager *stman;
        if(stman_type == "Adios2StMan")
        {
            stman = new Adios2StMan(MPI_COMM_WORLD);
        }

        IPosition array_pos = IPosition(1,cell_size);

        TableDesc td("", "1", TableDesc::Scratch);
        td.addColumn (ArrayColumnDesc<Float>("array_Float", array_pos, ColumnDesc::FixedShape));

        SetupNewTable newtab(filename, td, Table::New);
        newtab.bindAll(*stman);
        Table tab(MPI_COMM_WORLD, newtab, rows);

        ArrayColumn<Float> array_Float (tab, "array_Float");

        Array<Float> arr_Float(array_pos);

        GenData(arr_Float, array_pos, mpiRank);

        start_time = std::chrono::system_clock::now();
        for(int i=mpiRank; i<rows; i+=mpiSize)
        {
            array_Float.put(i, arr_Float);
        }
    }
    end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double totalsize = sizeof(float) * cell_size * rows * columns;
    double rate = totalsize / duration.count();

    json j;
    j["rows"] = rows;
    j["columns"] = columns;
    j["cell_number_floats"] = cell_size;
    j["total_bytes"] = totalsize;
    j["manager"] = stman_type;
    j["time"] = duration.count();
    j["rate"] = rate;

    cout << j.dump(4) << endl;

    MPI_Finalize();

    return 0;
}



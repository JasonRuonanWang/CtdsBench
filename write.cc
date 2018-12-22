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

#include <cmath>

using namespace std;
using namespace nlohmann;

size_t arraysize_min = 10;
size_t arraysize_max = 30;
size_t rows_min = 5;
size_t rows_max = 20;
size_t columns_range = 1;

int main(int argc, char **argv){

    int mpiRank, mpiSize;
    int mpi_provided_mode;
    MPI_Init_thread(&argc,&argv, MPI_THREAD_MULTIPLE, &mpi_provided_mode);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    if(argc < 6)
    {
        cout << "./write cell_size rows columns manager if_delete_file" << endl;
    }

    size_t cell_size = atoi(argv[1]);
    size_t rows = atoi(argv[2]);
    size_t columns = atoi(argv[3]);
    std::string stman_type = argv[4];
    bool if_delete = atoi(argv[5]);

    auto start_time = std::chrono::system_clock::now();
    auto end_time = std::chrono::system_clock::now();

    double totalsize = sizeof(float) * cell_size * rows * columns;
    json j;
    j["rows"] = rows;
    j["columns"] = columns;
    j["cell_number_floats"] = cell_size;
    j["total_bytes"] = totalsize;
    j["manager"] = stman_type;
    j["mpi_size"] = mpiSize;

    hash<string> hash_fn;
    string filename = "/lustre/atlas/scratch/wangj/csc303/data/" + to_string(hash_fn(j.dump()));

    {
        DataManager *stman;
        if(stman_type == "Adios2StMan")
        {
            stman = new Adios2StMan(MPI_COMM_WORLD);
        }

        IPosition array_pos = IPosition(1,cell_size);

        TableDesc td("", "1", TableDesc::Scratch);

        for(size_t i=0; i<columns; ++i)
        {
            string column_name = "column" + to_string(i);
            td.addColumn (ArrayColumnDesc<Float>(column_name, array_pos, ColumnDesc::FixedShape));
        }


        SetupNewTable newtab(filename, td, Table::New);
        newtab.bindAll(*stman);
        Table tab(MPI_COMM_WORLD, newtab, rows);

        vector<ArrayColumn<Float>> colvec;
        for(size_t i=0; i<columns; ++i)
        {
            string column_name = "column" + to_string(i);
            colvec.emplace_back(tab, column_name);
        }

        Array<Float> arr_Float(array_pos);

        GenData(arr_Float, array_pos, mpiRank);

        start_time = std::chrono::system_clock::now();
        for(int i=mpiRank; i<rows; i+=mpiSize)
        {
            for(auto &j : colvec)
            {
                j.put(i, arr_Float);
            }
        }
    }
    end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double rate = totalsize / duration.count();

    j["time"] = duration.count();
    j["rate"] = rate;

    if(mpiRank == 0)
    {
        cout << j.dump() << endl;
        if(if_delete)
        {
            string cmd = "rm -rf " + filename;
            system(cmd.c_str());
        }
    }

    MPI_Finalize();

    return 0;
}



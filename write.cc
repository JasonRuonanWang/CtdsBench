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

#include "common.h"
#include "json.hpp"

#include <cmath>

using namespace std;
using namespace nlohmann;

size_t cell_size_min = 10;
size_t cell_size_max = 27;
size_t columns_min = 1;
size_t columns_max = 1;
size_t rows_min = 5;
size_t rows_max = 10;

int main(int argc, char **argv){

    int mpiRank, mpiSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    string stman_type = argv[1];

    string filename = argv[2];

    if(stman_type == "AdiosStMan")
    {
        columns_max = 1;
    }

    srand (static_cast <unsigned> (time(0)));

    float cell_size_exp;
    if(cell_size_max == cell_size_min)
    {
        cell_size_exp = cell_size_min;
    }
    else
    {
        cell_size_exp = cell_size_min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(cell_size_max - cell_size_min)) );
    }
    size_t cell_size = pow(2, cell_size_exp);

    float rows_exp;
    if(rows_max == rows_min)
    {
        rows_exp = rows_min;
    }
    else
    {
        rows_exp = rows_min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(rows_max - rows_min)) );
    }
    size_t rows = pow(2, rows_exp) * mpiSize;

    size_t columns = rand() % columns_max + columns_min;

    auto start_time = std::chrono::system_clock::now();
    auto end_time = std::chrono::system_clock::now();

    double totalsize = sizeof(float) * cell_size * rows * columns;
    if(mpiRank ==0)
    {
        cout << "rows = " << rows << ", columns = " << columns << ", size = "  << cell_size << ", stman = " << stman_type << ", filename = " << filename << endl;
    }
    json j;
    j["rows"] = rows;
    j["columns"] = columns;
    j["cell_number_floats"] = cell_size;
    j["total_bytes"] = totalsize;
    j["manager"] = stman_type;
    j["mpi_size"] = mpiSize;

    DataManager *stman;
    {
        if(stman_type == "Adios2StMan")
        {
            adios2::Params engineParams = { {"Threads", "4"}, {"MaxBufferSize","1Gb"}};
            vector<adios2::Params> transportParams;
            string engineType = "BPFile";
            stman = new Adios2StMan(MPI_COMM_WORLD, engineType, engineParams, transportParams);
        }
        else if(stman_type == "Hdf5StMan")
        {
            stman = new Hdf5StMan(MPI_COMM_WORLD);
        }
        else if(stman_type == "AdiosStMan")
        {
            stman = new AdiosStMan("POSIX", "", 1000, 1);
        }
        else
        {
            cout << "unknown stman" << endl;
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
    delete stman;

    end_time = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    double rate = totalsize / duration.count();

    j["time"] = duration.count();
    j["rate"] = rate;

    if(mpiRank == 0)
    {
        cout << j.dump() << endl;
    }

    MPI_Finalize();

    return 0;
}



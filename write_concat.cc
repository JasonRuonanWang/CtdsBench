#include "common.h"

int main(int argc, char **argv){

    int mpiRank, mpiSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    string filename = argv[1];
    filename = filename + "." + to_string(mpiRank);

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
    size_t rows = pow(2, rows_exp);

    size_t columns = rand() % columns_max + columns_min;

    auto start_time = std::chrono::system_clock::now();
    auto end_time = std::chrono::system_clock::now();

    double totalsize = sizeof(float) * cell_size * rows * columns * mpiSize;
    if(mpiRank ==0)
    {
        cout << "rows = " << rows << ", columns = " << columns << ", size = "  << cell_size << ", stman = concat, filename = " << filename << endl;
    }
    json j;
    j["rows"] = rows;
    j["columns"] = columns;
    j["cell_number_floats"] = cell_size;
    j["total_bytes"] = totalsize;
    j["manager"] = "concat";
    j["mpi_size"] = mpiSize;

    MPI_Barrier(MPI_COMM_WORLD);
    {
        IPosition array_pos = IPosition(1,cell_size);
        TableDesc td("", "1", TableDesc::Scratch);
        for(size_t i=0; i<columns; ++i)
        {
            string column_name = "column" + to_string(i);
            td.addColumn (ArrayColumnDesc<Float>(column_name, array_pos, ColumnDesc::FixedShape));
        }
        SetupNewTable newtab(filename, td, Table::New);
        Table tab(MPI_COMM_SELF, newtab, rows);

        vector<ArrayColumn<Float>> colvec;
        for(size_t i=0; i<columns; ++i)
        {
            string column_name = "column" + to_string(i);
            colvec.emplace_back(tab, column_name);
        }

        Array<Float> arr_Float(array_pos);

        GenData(arr_Float, array_pos, mpiRank);

        start_time = std::chrono::system_clock::now();
        for(int i=0; i<rows; i++)
        {
            for(auto &j : colvec)
            {
                j.put(i, arr_Float);
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

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



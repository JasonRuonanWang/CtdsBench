#include "common.h"

size_t rows = 10000;
size_t columns = 1;
size_t cell_size = 1024;

int main(int argc, char **argv){


    int mpiRank, mpiSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    try{


    string stman_type = argv[1];
    string filename = argv[2];

    if(mpiRank ==0)
    {
        cout << "rows = " << rows << ", columns = " << columns << ", cell size = "  << cell_size << ", total bytes = "  << cell_size*rows*columns*sizeof(float) << ", stman = " << stman_type << ", filename = " << filename << endl;
    }

    DataManager *stman;
    {
        if(stman_type == "Adios2StMan")
        {
            adios2::Params engineParams = {};
            vector<adios2::Params> transportParams;
            string engineType = "BPFile";
            stman = new Adios2StMan(MPI_COMM_WORLD, engineType, engineParams, transportParams);
        }
        else if(stman_type == "Hdf5StMan")
        {
            stman = new Hdf5StMan;
        }
        else if(stman_type == "AdiosStMan")
        {
            stman = new AdiosStMan("POSIX", "verbose=0", 1000, 1);
        }

        IPosition array_pos = IPosition(1,cell_size);

        TableDesc td("", "1", TableDesc::Scratch);

        for(size_t i=0; i<columns; ++i)
        {
            string column_name = "column" + to_string(i);
            td.addColumn (ArrayColumnDesc<Float>(column_name, array_pos, ColumnDesc::FixedShape));
        }

        SetupNewTable newtab(filename, td, Table::New);
        if(stman_type != "none")
        {
            newtab.bindAll(*stman);
        }
        Table tab(MPI_COMM_WORLD, newtab, rows);

        vector<ArrayColumn<Float>> colvec;
        for(size_t i=0; i<columns; ++i)
        {
            string column_name = "column" + to_string(i);
            colvec.emplace_back(tab, column_name);
        }

        Array<Float> arr_Float(array_pos);

        GenData(arr_Float, array_pos, mpiRank);

        for(int i=mpiRank; i<rows; i+=mpiSize)
        {
            for(auto &k : colvec)
            {
                k.put(i, arr_Float);
            }
        }
    }

    if(stman_type != "none")
    {
        delete stman;
    }


    }
    catch(std::exception &e){
        std::cout << e.what() << std::endl;
    }

    MPI_Finalize();

    return 0;
}



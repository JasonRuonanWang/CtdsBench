#include <memory>
#include <random>

#include <adios2.h>

#include "common.h"

struct options {
    int mpi_rank = 0;
    int mpi_size = 1;
    size_t columns = 1;
    size_t rows = 10000;
    size_t cell_size = 1;
    int n_dims = 1;
    std::string filename;
    std::string stman_type {"none"};
    std::string data_type {"float"};
};

template<typename T>
void fill_array(Array<T> &array)
{
    array = T (std::random_device()());
}

template<typename C>
void fill_array_complex(Array<C> &array)
{
    using ctype = typename C::value_type;
    std::random_device dev;
    array = C {ctype(dev()), ctype(dev())};
}

template<>
void fill_array<Complex>(Array<Complex> &array)
{
    fill_array_complex<Complex>(array);
}

template<>
void fill_array<DComplex>(Array<DComplex> &array)
{
    fill_array_complex<DComplex>(array);
}

template<typename T>
void write_to_table(const options &opts, IPosition &array_pos, std::unique_ptr<DataManager> &&stman)
{
    if (opts.mpi_rank == 0)
    {
        std::cout << "rows = " << opts.rows << ", columns = " << opts.columns
                  << ", cell size = "  << opts.cell_size
                  << ", total bytes = "
                  << opts.cell_size * opts.rows * opts.columns * sizeof(T)
                  << ", stman = " << opts.stman_type << ", filename = "
                  << opts.filename << '\n';
    }

    TableDesc td("", "1", TableDesc::Scratch);

    for (size_t i = 0; i < opts.columns; ++i)
    {
        string column_name = "column" + to_string(i);
        td.addColumn (ArrayColumnDesc<T>(column_name, array_pos, ColumnDesc::FixedShape));
    }

    SetupNewTable newtab(opts.filename, td, Table::New);
    if (stman)
    {
        newtab.bindAll(*stman);
    }
    Table tab(MPI_COMM_WORLD, newtab, opts.rows);

    vector<ArrayColumn<T>> colvec;
    for (size_t i = 0; i < opts.columns; ++i)
    {
        std::string column_name = "column" + to_string(i);
        colvec.emplace_back(tab, column_name);
    }

    // Create some data and write it
    Array<T> arr(array_pos);
    fill_array(arr);
    for (size_t i = opts.mpi_rank; i < opts.rows; i += opts.mpi_size)
    {
        for (auto &k : colvec)
        {
            k.put(i, arr);
        }
    }
}

std::unique_ptr<Adios2StMan> make_adios2_stman(const std::string &type, const adios2::Params &params)
{
    return std::make_unique<Adios2StMan>(MPI_COMM_WORLD, type, params, std::vector<adios2::Params>{});
}

void run(int argc, char **argv){

    options opts;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &opts.mpi_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &opts.mpi_size);

    opts.stman_type = argv[1];
    opts.filename = argv[2];
    if (argc > 3) {
      opts.cell_size = std::stold(argv[3]);
    }
    if (argc > 4) {
      opts.n_dims = std::stod(argv[4]);
    }
    if (argc > 5) {
      opts.data_type = argv[5];
    }
    if (argc > 6) {
      opts.rows = std::stold(argv[6]);
    }

    // The storage manager to use
    std::unique_ptr<DataManager> stman;
    if (opts.stman_type == "Adios2StMan")
    {
        stman = make_adios2_stman("BPFile", {});
    }
    else if (opts.stman_type == "Adios2StMan-HDF5")
    {
        stman = make_adios2_stman("HDF5", {{"Threads", "4"}, {"MaxBufferSize", "1Gb"}});
    }
#ifdef HAS_HDF5STMAN
    else if(opts.stman_type == "Hdf5StMan")
    {
        stman = std::make_unique<Hdf5StMan>();
    }
#endif
#ifdef HAS_ADIOSSTMAN
    else if(opts.stman_type == "AdiosStMan")
    {
        stman = std::make_unique<AdiosStMan>("POSIX", "verbose=0", 1000, 1);
    }
#endif

    // The dimensionality
    auto array_pos = std::make_unique<IPosition>(1, opts.cell_size);
    if (opts.n_dims == 2) {
        array_pos = std::make_unique<IPosition>(2, 1, opts.cell_size);
    }
    else if (opts.n_dims > 2) {
        throw std::runtime_error("Only 1 or 2 dimensions supported in writer_example");
    }

    // The data type
    if (opts.data_type == "float") {
        write_to_table<Float>(opts, *array_pos, std::move(stman));
    }
    else if (opts.data_type == "double") {
        write_to_table<Double>(opts, *array_pos, std::move(stman));
    }
    else if (opts.data_type == "complex") {
        write_to_table<Complex>(opts, *array_pos, std::move(stman));
    }
    else if (opts.data_type == "dcomplex") {
        write_to_table<DComplex>(opts, *array_pos, std::move(stman));
    }
    else {
        std::runtime_error("Only float/double/complex/dcomplex data types supported in writer_example");
    }
}

int main(int argc, char **argv)
{
    try {
        MPI_Init(&argc, &argv);
        run(argc, argv);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    MPI_Finalize();
    return 0;
}

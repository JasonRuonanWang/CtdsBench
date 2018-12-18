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

int main(int argc, char **argv){

    int mpiRank, mpiSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    std::string filename;
    if (argc < 2){
        filename = "aaa.table";
    }
    else{
        filename = argv[1];
    }

    {

        Adios2StMan stman(MPI_COMM_WORLD);

        int NrRows = mpiSize;

        IPosition array_pos = IPosition(2,5,6);

        TableDesc td("", "1", TableDesc::Scratch);
        td.addColumn (ScalarColumnDesc<Bool>("scalar_Bool"));
        td.addColumn (ScalarColumnDesc<uChar>("scalar_uChar"));
        td.addColumn (ScalarColumnDesc<Short>("scalar_Short"));
        td.addColumn (ScalarColumnDesc<uShort>("scalar_uShort"));
        td.addColumn (ScalarColumnDesc<Int>("scalar_Int"));
        td.addColumn (ScalarColumnDesc<uInt>("scalar_uInt"));
        td.addColumn (ScalarColumnDesc<Float>("scalar_Float"));
        td.addColumn (ScalarColumnDesc<Double>("scalar_Double"));
        td.addColumn (ScalarColumnDesc<Complex>("scalar_Complex"));
        td.addColumn (ScalarColumnDesc<DComplex>("scalar_DComplex"));

        td.addColumn (ArrayColumnDesc<Bool>("array_Bool", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<uChar>("array_uChar", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<Short>("array_Short", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<uShort>("array_uShort", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<Int>("array_Int", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<uInt>("array_uInt", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<Float>("array_Float", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<Double>("array_Double", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<Complex>("array_Complex", array_pos, ColumnDesc::FixedShape));
        td.addColumn (ArrayColumnDesc<DComplex>("array_DComplex", array_pos, ColumnDesc::FixedShape));

        SetupNewTable newtab(filename, td, Table::New);
        newtab.bindAll(stman);
        Table tab(MPI_COMM_WORLD, newtab, NrRows);

        ScalarColumn<Bool> scalar_Bool (tab, "scalar_Bool");
        ScalarColumn<uChar> scalar_uChar (tab, "scalar_uChar");
        ScalarColumn<Short> scalar_Short (tab, "scalar_Short");
        ScalarColumn<uShort> scalar_uShort (tab, "scalar_uShort");
        ScalarColumn<Int> scalar_Int (tab, "scalar_Int");
        ScalarColumn<uInt> scalar_uInt (tab, "scalar_uInt");
        ScalarColumn<Float> scalar_Float (tab, "scalar_Float");
        ScalarColumn<Double> scalar_Double (tab, "scalar_Double");
        ScalarColumn<Complex> scalar_Complex (tab, "scalar_Complex");
        ScalarColumn<DComplex> scalar_DComplex (tab, "scalar_DComplex");

        ArrayColumn<Bool> array_Bool (tab, "array_Bool");
        ArrayColumn<uChar> array_uChar (tab, "array_uChar");
        ArrayColumn<Short> array_Short (tab, "array_Short");
        ArrayColumn<uShort> array_uShort (tab, "array_uShort");
        ArrayColumn<Int> array_Int (tab, "array_Int");
        ArrayColumn<uInt> array_uInt (tab, "array_uInt");
        ArrayColumn<Float> array_Float (tab, "array_Float");
        ArrayColumn<Double> array_Double (tab, "array_Double");
        ArrayColumn<Complex> array_Complex (tab, "array_Complex");
        ArrayColumn<DComplex> array_DComplex (tab, "array_DComplex");

        Array<Bool> arr_Bool(array_pos);
        Array<Char> arr_Char(array_pos);
        Array<uChar> arr_uChar(array_pos);
        Array<Short> arr_Short(array_pos);
        Array<uShort> arr_uShort(array_pos);
        Array<Int> arr_Int(array_pos);
        Array<uInt> arr_uInt(array_pos);
        Array<Float> arr_Float(array_pos);
        Array<Double> arr_Double(array_pos);
        Array<Complex> arr_Complex(array_pos);
        Array<DComplex> arr_DComplex(array_pos);

        GenData(arr_Bool, array_pos, mpiRank);
        GenData(arr_uChar, array_pos, mpiRank);
        GenData(arr_Short, array_pos, mpiRank);
        GenData(arr_uShort, array_pos, mpiRank);
        GenData(arr_Int, array_pos, mpiRank);
        GenData(arr_uInt, array_pos, mpiRank);
        GenData(arr_Float, array_pos, mpiRank);
        GenData(arr_Double, array_pos, mpiRank);
        GenData(arr_Complex, array_pos, mpiRank);
        GenData(arr_DComplex, array_pos, mpiRank);

        Bool sca_Bool = mpiRank + 1;
        uChar sca_uChar = mpiRank + 1;
        Short sca_Short = mpiRank + 1;
        uShort sca_uShort = mpiRank + 1;
        Int sca_Int = mpiRank + 1;
        uInt sca_uInt = mpiRank + 1;
        Float sca_Float = mpiRank + 1;
        Double sca_Double = mpiRank + 1;
        Complex sca_Complex = mpiRank + 1;
        DComplex sca_DComplex = mpiRank + 1;

        scalar_Bool.put (mpiRank, sca_Bool);
        scalar_uChar.put (mpiRank, sca_uChar);
        scalar_Short.put (mpiRank, sca_Short);
        scalar_uShort.put (mpiRank, sca_uShort);
        scalar_Int.put (mpiRank, sca_Int);
        scalar_uInt.put (mpiRank, sca_uInt);
        scalar_Float.put (mpiRank, sca_Float);
        scalar_Double.put (mpiRank, sca_Double);
        scalar_Complex.put (mpiRank, sca_Complex);
        scalar_DComplex.put (mpiRank, sca_DComplex);

        array_Bool.put(mpiRank, arr_Bool);
        array_uChar.put(mpiRank, arr_uChar);
        array_Short.put(mpiRank, arr_Short);
        array_uShort.put(mpiRank, arr_uShort);
        array_Int.put(mpiRank, arr_Int);
        array_uInt.put(mpiRank, arr_uInt);
        array_Float.put(mpiRank, arr_Float);
        array_Double.put(mpiRank, arr_Double);
        array_Complex.put(mpiRank, arr_Complex);
        array_DComplex.put(mpiRank, arr_DComplex);
    }

    MPI_Finalize();

    return 0;
}



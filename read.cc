//    (c) Oak Ridge National Laboratory
//    1 Bethel Valley Road, Oak Ridge, TN 37830, United States
//
//    This library is free software: you can redistribute it and/or
//    modify it under the terms of the GNU General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this library. If not, see <http://www.gnu.org/licenses/>.
//
//    Any bugs, questions, concerns and/or suggestions please email to
//    wangr1@ornl.gov or jason.ruonan.wang@gmail.com


#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/tables/Tables/ScalarColumn.h>
#include <casacore/casa/namespace.h>

#include <iostream>

using namespace casacore;
using namespace std;


string filename = "aaa.table";
uInt gettingRow = 0;

void ReadFunction(){

    Table casa_table(filename);

    ROArrayColumn<Bool> array_Bool(casa_table, "array_Bool");
    Array<Bool> arr_Bool = array_Bool.get(gettingRow);
    Vector<Bool> vec_Bool = arr_Bool.reform(IPosition(1,arr_Bool.nelements()));
    cout << endl << "Column: array_Bool, Shape: " << arr_Bool.shape() << endl;

    ROArrayColumn<uChar> array_uChar(casa_table, "array_uChar");
    Array<uChar> arr_uChar = array_uChar.get(gettingRow);
    Vector<uChar> vec_uChar = arr_uChar.reform(IPosition(1,arr_uChar.nelements()));
    cout << endl << "Column: array_uChar, Shape: " << arr_uChar.shape() << endl;
    for (int i=0; i<arr_uChar.nelements(); i++){
        cout << (uInt)vec_uChar[i] << "  ";
        if ((i+1) % (arr_uChar.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<Short> array_Short(casa_table, "array_Short");
    Array<Short> arr_Short = array_Short.get(gettingRow);
    Vector<Short> vec_Short = arr_Short.reform(IPosition(1,arr_Short.nelements()));
    cout << endl << "Column: array_Short, Shape: " << arr_Short.shape() << endl;
    for (int i=0; i<arr_Short.nelements(); i++){
        cout << vec_Short[i] << "  ";
        if ((i+1) % (arr_Short.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<uShort> array_uShort(casa_table, "array_uShort");
    Array<uShort> arr_uShort = array_uShort.get(gettingRow);
    Vector<uShort> vec_uShort = arr_uShort.reform(IPosition(1,arr_uShort.nelements()));
    cout << endl << "Column: array_uShort, Shape: " << arr_uShort.shape() << endl;
    for (int i=0; i<arr_uShort.nelements(); i++){
        cout << vec_uShort[i] << "  ";
        if ((i+1) % (arr_uShort.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<Int> array_Int(casa_table, "array_Int");
    Array<Int> arr_Int = array_Int.get(gettingRow);
    Vector<Int> vec_Int = arr_Int.reform(IPosition(1,arr_Int.nelements()));
    cout << endl << "Column: array_Int, Shape: " << arr_Int.shape() << endl;
    for (int i=0; i<arr_Int.nelements(); i++){
        cout << vec_Int[i] << "  ";
        if ((i+1) % (arr_Int.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<uInt> array_uInt(casa_table, "array_uInt");
    Array<uInt> arr_uInt = array_uInt.get(gettingRow);
    Vector<uInt> vec_uInt = arr_uInt.reform(IPosition(1,arr_uInt.nelements()));
    cout << endl << "Column: array_uInt, Shape: " << arr_uInt.shape() << endl;
    for (int i=0; i<arr_uInt.nelements(); i++){
        cout << vec_uInt[i] << "  ";
        if ((i+1) % (arr_uInt.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<Float> array_Float(casa_table, "array_Float");
    Array<Float> arr_Float = array_Float.get(gettingRow);
    Vector<Float> vec_Float = arr_Float.reform(IPosition(1,arr_Float.nelements()));
    cout << endl << "Column: array_Float, Shape: " << arr_Float.shape() << endl;
    for (int i=0; i<arr_Float.nelements(); i++){
        cout << vec_Float[i] << "  ";
        if ((i+1) % (arr_Float.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<Double> array_Double(casa_table, "array_Double");
    Array<Double> arr_Double = array_Double.get(gettingRow);
    Vector<Double> vec_Double = arr_Double.reform(IPosition(1,arr_Double.nelements()));
    cout << endl << "Column: array_Double, Shape: " << arr_Double.shape() << endl;
    for (int i=0; i<arr_Double.nelements(); i++){
        cout << vec_Double[i] << "  ";
        if ((i+1) % (arr_Double.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<Complex> array_Complex(casa_table, "array_Complex");
    Array<Complex> arr_Complex = array_Complex.get(gettingRow);
    Vector<Complex> vec_Complex = arr_Complex.reform(IPosition(1,arr_Complex.nelements()));
    cout << endl << "Column: array_Complex, Shape: " << arr_Complex.shape() << endl;
    for (int i=0; i<arr_Complex.nelements(); i++){
        cout << vec_Complex[i] << "  ";
        if ((i+1) % (arr_Complex.shape())(0) == 0)
            cout << endl;
    }

    ROArrayColumn<DComplex> array_DComplex(casa_table, "array_DComplex");
    Array<DComplex> arr_DComplex = array_DComplex.get(gettingRow);
    Vector<DComplex> vec_DComplex = arr_DComplex.reform(IPosition(1,arr_DComplex.nelements()));
    cout << endl << "Column: array_DComplex, Shape: " << arr_DComplex.shape() << endl;
    for (int i=0; i<arr_DComplex.nelements(); i++){
        cout << vec_DComplex[i] << "  ";
        if ((i+1) % (arr_DComplex.shape())(0) == 0)
            cout << endl;
    }

    ROScalarColumn<Bool> scalar_Bool(casa_table, "scalar_Bool");
    Bool sca_Bool = scalar_Bool.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_Bool << endl;

    ROScalarColumn<uChar> scalar_uChar(casa_table, "scalar_uChar");
    uChar sca_uChar = scalar_uChar.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << (uInt)sca_uChar << endl;

    ROScalarColumn<Short> scalar_Short(casa_table, "scalar_Short");
    Short sca_Short = scalar_Short.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_Short << endl;

    ROScalarColumn<uShort> scalar_uShort(casa_table, "scalar_uShort");
    uShort sca_uShort = scalar_uShort.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_uShort << endl;

    ROScalarColumn<Int> scalar_Int(casa_table, "scalar_Int");
    Int sca_Int = scalar_Int.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_Int << endl;

    ROScalarColumn<uInt> scalar_uInt(casa_table, "scalar_uInt");
    uInt sca_uInt = scalar_uInt.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_uInt << endl;

    ROScalarColumn<Float> scalar_Float(casa_table, "scalar_Float");
    Float sca_Float = scalar_Float.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_Float << endl;

    ROScalarColumn<Double> scalar_Double(casa_table, "scalar_Double");
    Double sca_Double = scalar_Double.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_Double << endl;

    ROScalarColumn<Complex> scalar_Complex(casa_table, "scalar_Complex");
    Complex sca_Complex = scalar_Complex.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_Complex << endl;

    ROScalarColumn<DComplex> scalar_DComplex(casa_table, "scalar_DComplex");
    DComplex sca_DComplex = scalar_DComplex.get(gettingRow);
    cout << endl << "Column: scalar_Bool: " << sca_DComplex << endl;

}

int main(int argc, char **argv){
    int mpiRank, mpiSize;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpiRank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpiSize);

    if (argc >= 2){
        filename = argv[1];
    }
    if (argc >= 3){
        gettingRow = atoi(argv[2]);
    }

    ReadFunction();

    MPI_Finalize();

    return 0;
}



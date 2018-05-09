#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

class matrix_t {
    int ** data;
    unsigned int rows;
    unsigned int collumns;

public:

    matrix_t(){
        rows = 0;
        collumns = 0;
        data = nullptr;
    }

    matrix_t & operator =(matrix_t const & other) {
        if (this != &other) {
            for (unsigned int i = 0; i < rows; ++i)
                delete[] data[i];
            delete[] data;
            data = new int *[other.rows];
            for (unsigned int i = 0; i < other.rows; ++i) {
                data[i] = new int[other.collumns];
                for (unsigned int j = 0; j < other.collumns; ++j)
                    data[i][j] = other.data[i][j];
            }
            rows = other.rows;
            collumns = other.collumns;
        }
        return *this;
    }

    matrix_t(matrix_t const & other) {
        data = new int *[other.rows];
        for (unsigned int i = 0; i < other.rows; ++i) {
            data[i] = new int[other.collumns];
            for (unsigned int j = 0; j < other.collumns; ++j)
                data[i][j] = other.data[i][j];
        }
        rows = other.rows;
        collumns = other.collumns;
    }

    bool error = false;

    matrix_t add( matrix_t & other ){
        if (rows == other.rows && collumns == other.collumns)
            for (unsigned int i = 0; i < rows; i++)
                for (unsigned int j = 0; j < collumns; j++)
                    data[i][j] += other.data[i][j];
        else error = true;
        return *this;
    }

    matrix_t sub( matrix_t & other ){
        if (rows == other.rows && collumns == other.collumns)
            for (unsigned int i = 0; i < rows; i++)
                for (unsigned int j = 0; j < collumns; j++)
                    data[i][j] -= other.data[i][j];
        else error = true;
        return *this;
    }

    matrix_t mul( matrix_t & other ){
        matrix_t result;
        result.data = new int *[rows];
        for (int i = 0; i < rows; i++) {
            result.data[i] = new int[other.collumns];
            for (int j = 0; j < other.collumns; j++) {
                result.data[i][j] = 0;
                for (int f = 0; f < collumns; f++)
                    result.data[i][j] += data[i][f] * other.data[f][j];
            }
        }
        result.rows = rows;
        result.collumns = other.collumns;
        return result;
    }

    matrix_t trans(){
        matrix_t result;
        result.data = new int *[collumns];
        for (int i = 0; i < collumns; i++) {
            result.data[i] = new int[rows];
            for (int j = 0; j < rows; j++)
                result.data[i][j] = data[j][i];
        }
        result.rows = collumns;
        result.collumns = rows;
        return result;
    }

    ifstream & read(ifstream & file) {
        unsigned int row;
        unsigned int column;
        char zap;
        int ** elements = nullptr;
        string line;

        if (getline(file, line)) {
            istringstream sstream(line);
            if (sstream >> row && sstream >> zap && zap == ',' && sstream >> column && sstream.eof()) {
                elements = new int *[row];
                for (unsigned int i = 0; i < row; i++) {
                    elements[i] = new int[column];
                    if (getline(file, line)){
                        istringstream sstream2(line);
                        for (unsigned int j = 0; j < column; j++) {
                            sstream2 >> elements[i][j];
                            if (!sstream2) error = true;
                        }
                    }
                    else error = true;
                }
            }
            else error = true;
            data = elements;
            rows = row;
            collumns = column;
        }
        else error = true;
        return file;
    }

    ostream & write(ostream & stream) const {
        stream << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < collumns; j++) {
                stream << data[i][j] << ' ';
            }
            stream << endl;
        }
        return stream;
    }

    ~matrix_t() {
        for (unsigned int i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
};

int main() {
    matrix_t matrix1;
    string line, filename1, filename2;
    char op;

    if (getline(cin, line)){
        istringstream stream(line);
        if (stream >> filename1 && stream >> op){
            if (stream >> filename2){
                matrix_t matrix2;
                ifstream file1(filename1);
                ifstream file2(filename2);
                matrix1.read(file1);
                matrix2.read(file2);
                if (!matrix1.error && !matrix2.error){
                  switch (op) {
                      case '+':
                          matrix1 = matrix1.add(matrix2);
                          if (!matrix1.error) matrix1.write(cout);
                          else cout << "An error has occured while reading input data";
                          break;

                      case '-':
                          matrix1 = matrix1.sub(matrix2);
                          if (!matrix1.error) matrix1.write(cout);
                          else cout << "An error has occured while reading input data";
                          break;

                      case '*':
                          matrix1 = matrix1.mul(matrix2);
                          matrix1.write(cout);
                          break;

                      default: cout << "An error has occured while reading input data";
                  }
                }
                else cout << "An error has occured while reading input data";
            }
            else {
                ifstream file1(filename1);
                matrix1.read(file1);
                if (!matrix1.error && op == 'T'){
                    matrix1 = matrix1.trans();
                    matrix1.write(cout);
                }
                else cout << "An error has occured while reading input data";
            }
        }
        else cout << "An error has occured while reading input data";
    }
    else cout << "An error has occured while reading input data";
}
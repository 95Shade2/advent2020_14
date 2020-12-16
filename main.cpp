#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <iomanip>

#define coutfixed cout << fixed << setprecision(0)

using namespace std;

int Int(string num) {
    int num_i = 0;

    while (num.size() > 0) {
        num_i *= 10;
        num_i += num[0] - '0';
        num = num.substr(1);
    }

    return num_i;
}

vector<string> File_To_Str_Vec(string filename) {
    vector<string> lines_int;
    string line;
    ifstream file;

    file.open(filename.c_str());

    getline(file, line);
    while (file) {
        lines_int.push_back(line);
        getline(file, line);
    }

    file.close();

    return lines_int;
}

string Binary(double num, int length) {
    double dec = pow(2, length);

    if (length == 0) {
        if (num == 1) {
            return "1";
        }
        else {
            return "0";
        }
    }

    if (num >= dec) {
        num -= dec;
        return "1" + Binary(num, --length);
    }
    else {
        return "0" + Binary(num, --length);
    }
}

string Mask_Bin(string bin, string mask, int part = 1) {
    if (bin.size() == 0) {
        return "";
    }

    if (mask[0] == 'X') {
        if (part == 1) {
            return bin[0] + Mask_Bin(bin.substr(1), mask.substr(1));
        }
        else {
            return "1" + Mask_Bin(bin.substr(1), mask.substr(1), part);
        }
    }
    else {
        if (part == 2) {
            if (mask[0] == '0') {
                return bin[0] + Mask_Bin(bin.substr(1), mask.substr(1), part);
            }
            else {
                return mask[0] + Mask_Bin(bin.substr(1), mask.substr(1), part);
            }
        }
        else {
            return mask[0] + Mask_Bin(bin.substr(1), mask.substr(1));
        }
    }
}

double Bin_Double(string binary) {
    double num;
    double num2 = 0;

    for (int b = 0; b < binary.size(); b++) {
        num2 = pow(2, binary.size()-b-1);
        if (binary[b] == '1') {
            num += num2;
        }
    }

    return num;
}

int Count(string text, char chr) {
    int number = 0;

    for (int c = 0; c < text.size(); c++) {
        if (text[c] == chr) {
            number++;
        }
    }

    return number;
}

vector<double> Indexes(string text, char chr) {
    vector<double> indexes;

    for (double c = 0; c < text.size(); c++) {
        if (text[c] == chr) {
            indexes.push_back(c);
        }
    }

    return indexes;
}

double Mask(double value, string mask, int part, map<double, double> *memory, double address) {
    string binary = Binary(value, 35);
    string binary_2;
    double num;
    vector<double> x_indexes;
    string x_binary;

    if (part == 1) {
        binary = Mask_Bin(binary, mask);
        num = Bin_Double(binary);
    }
    else {
        binary = Binary(address, 35);
        binary = Mask_Bin(binary, mask, 2);
        x_indexes = Indexes(mask, 'X');
        num = pow(2, x_indexes.size());  //the number of different address we need to write to

        for (double a = 0; a < num; a++) {
            x_binary = Binary(a, x_indexes.size()-1);
            binary_2 = binary;

            for (double x = 0; x < x_indexes.size(); x++) {
                binary_2[x_indexes[x]] = x_binary[x];
            }

            (*memory)[Bin_Double(binary_2)] = value;
        }
    }

    return num;
}

void Mask_Values(vector<string> lines, int part = 1) {
    string line;
    map<double, double> memory;
    string mask;
    double address;
    double value;
    double sum = 0;

    for (int l = 0; l < lines.size(); l++) {
        line = lines[l];

        //set the bit mask
        if (line.substr(0, 4) == "mask") {
            mask = line.substr(line.find_last_of(" ")+1);
        }
        else {
            address = Int(line.substr(line.find("[")+1, line.find("]") - line.find("[")-1));
            value = Int(line.substr(line.find_last_of(" ")+1));

            if (part == 1) {
                value = Mask(value, mask, part, &memory, address);
                memory[address] = value;
            }
            else {
                Mask(value, mask, part, &memory, address);
            }
        }
    }

    for (auto const& element: memory) {
        sum += memory[element.first];
    }

    coutfixed << sum << endl;
}

int main()
{
    vector<string> lines;

    lines = File_To_Str_Vec("dock.txt");

    Mask_Values(lines);

    Mask_Values(lines, 2);

    return 0;
}

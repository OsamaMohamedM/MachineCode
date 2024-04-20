#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;
class Memory {
protected:
    int cells = 0;
    vector<string> memories = vector<string>(256);
public:
    Memory() {};
    string get(int address) const {
        return memories[address] + memories[address + 1];
    }

    void set(int cell, const string& data) {
        memories[cell] = data.substr(0, 2);
        memories[cell + 1] = data.substr(2, 2);
        cells += 2;
    }

    void set_val(int cell, const string& data) {
        string value = (data.size() == 1) ? "0" + data : data;
        memories[cell] = value;
    }

    int get_val(int address) const {
        return  stoi(memories[address]);
    }

};

class Machine : public Memory{
public:
    Machine() : Memory() {
        programCounter = 1, registers = vector<int>(18);
    }

    void showPC() const {
        cout << "The program counter is " << programCounter << " ";
    }

    void showIR(const string& instruction) const {
        cout << "The IR is " << instruction << "\n";
    }
    void set_prog_cnt(int par){
        programCounter = par;
    }
    void showregisters() const {
        for (int i = 0; i < registers.size(); i++) {
            cout << "register " << to_string(i) << " has value = " << hex << uppercase << registers[i] << "\n";
        }
        cout << "\n";
    }

    void showmemory() const {
        for (int i = 0; i < cells; i++) {
            cout << "memory address " << hex << i << " has " << memories[i] << "\n";
        }
        cout << "\n\n";
    }

    void executeInstruction(const string& instruction) {
        switch (instruction[0]) {
            case '1':
                executeopcode1(instruction);
                break;
            case '2':
                executeopcode2(instruction);
                break;
            case '3':
                executeopcode3(instruction);
                break;
            case '4':
                executeopcode4(instruction);
                break;
            case '5':
                executeopcode5(instruction);
                break;
            case 'B':
                executeopcodeB(instruction);
                break;
            case 'C':
                executeopcodeC(instruction);
                break;
            default:
                break;
        }
    }

    void run() {
        while (programCounter < cells && programCounter>0) {
            string instruction = get(programCounter);
            //---------------------------------------->
            executeInstruction(instruction);
            showPC();
            showIR(instruction);
            showregisters();
            showmemory();


        }
    }

private:
    int programCounter;
    vector<int> registers;
    void executeopcode1(const string& instruction) {
        int address =(isalpha(instruction[2]) ? ((instruction[2] - 'A' + 10) * 16) : (instruction[2] - '0') * 16) ;
        address += (isalpha(instruction[3]) ? (instruction[3] - 'A' + 10) : instruction[3] - '0');
        registers[instruction[1] - '0'] = get_val(address);
        programCounter += 2;
    }

    void executeopcode2(const string& instruction) {
        int value = (isalpha(instruction[2]) ? ((instruction[2] - 'A' + 10) * 16) : (instruction[2] - '0') * 16);
        value += (isalpha(instruction[3]) ? (instruction[3] - 'A' + 10) : instruction[3] - '0');
        registers[instruction[1] - '0'] = value;
        programCounter += 2;
    }

    void executeopcode3(const string& instruction) {
        if (instruction[1] == '0') {
            cout << "The Value is " << registers[instruction[1] - '0'] << "\n";
        } else {
            int address =(isalpha(instruction[2]) ? ((instruction[2] - 'A' + 10) * 16) : (instruction[2] - '0') * 16) ;
            address += (isalpha(instruction[3]) ? (instruction[3] - 'A' + 10) : instruction[3] - '0');
            set_val(address, to_string(registers[instruction[1] - '0']));
        }
        programCounter += 2;
    }

    void executeopcode4(const string& instruction) {
        registers[instruction[2] - '0'] = registers[instruction[3] - '0'];
        programCounter += 2;
    }

    void executeopcode5(const string& instruction) {
        char numberone=registers[instruction[2] - '0'];
        char numbertwo=registers[instruction[3] - '0'];
        registers[instruction[1] - '0'] = ( numberone+ numbertwo );
        programCounter += 2;
    }

    void executeopcodeB(const string& instruction) {
        if (registers[instruction[1] - '0'] == registers[0]) {
            int value = (isalpha(instruction[2]) ? ((instruction[2] - 'A' + 10) * 16) : (instruction[2] - '0') * 16);
            value += (isalpha(instruction[3]) ? (instruction[3] - 'A' + 10) : instruction[3] - '0');
            programCounter = value;
        } else
        {
            programCounter += 2;
        }
    }

    void executeopcodeC(const string& instruction) {
        cout << "Ending execution.\n";
        programCounter = -1;
    }
};


string getvalidfilename(const string& inputfilename) {
    if (inputfilename.find(".txt") == string::npos) {
        return inputfilename + ".txt";
    }
    return inputfilename;
}

bool processfile(const string& filename) {
    // open file
    bool check = true;
    ifstream inputfile(filename);
    if (!inputfile.is_open()) {
        cout << "Cannot find the file in the project. Check the name and location.\n";
        return false;
    }
    int address;
    string value;
    Machine* machine = new Machine();
    while (inputfile >> hex >> address, inputfile >> value) {
        if (check){
            machine->set_prog_cnt(address);
            check = false;
        }
        machine->set(address, value);
    }

    machine->run();
    delete machine;

    return true;
}

int main() {
    cout << "Hello user, nice to meet you again.\n";

    while (true) {
        cout << "Enter the name of the file: ";
        string filename;
        cin >> filename;

        filename = getvalidfilename(filename);

        if (processfile(filename)) {
            cout << "File processed successfully.\n";
        }

        cout << "Do you want to continue? Enter 'y' for yes, 'n' for no: ";
        char response;
        cin >> response;

        if (tolower(response) == 'n') {
            cout << "Goodbye! See you again.\n";
            break;
        }
    }

    return 0;
}

#include "iostream"
#include "thread"
#include "chrono"
#include "map"
#include "functional"
#include "algorithm"
#include "chrono"
#include "cstdlib"
#include "cwctype"
#include "string"
#include "thread"
#include "algorithm"

using namespace std;

void TMPMenu();
void Select();
void MKTMPFS();
void DELTMPFS();

int main() {
    system("clear");
    cout << "\033[32mchecking pigz\033[0m\n" << endl;
    if (system("command -v pigz > /dev/null 2>&1")) {
        cout << "\033[31mERROR PKG 'pigz' NOT FOUND! Install pigz First\033[0m\n" << endl;
        return 1;
    }
    cout << "\033[32mPIGZ LOADED!\033[0m\n" << endl;
    cout << "Welcome to TMPFS MAKER\n" << endl;
    cout << "Do Not Use 'CTRL + C' TO EXIT!" << endl;
    Select();
}

void TMPMenu() {

    const char* menu[] = {
        "Make TMPFS",
        "Delete TMPFS And Sync",
        "Exit (Just EXIT, TMP DIR Gonna be Untouched)"
    };

    int Length = sizeof(menu) / sizeof(menu[0]);

    for (int counter = 0 ; counter < Length; counter++) {
        cout << "[" << counter + 1 << "] " << menu[counter] << endl;
    }

}

void Select() {
    int Choice;
    string YN;
    string COPY;

    while (true) {
        TMPMenu();
        cout << "\nSelect: "; cin >> Choice;
        if (!cin) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "" << endl;
            continue;
        }

        switch (Choice) {
            case 1:
                system("clear");
                MKTMPFS();
                break;

            case 2: {
                system("clear");
                cout << "\033[31mWARNING! THE TMPFS IS A TEMPORARY DIR IN RAM, IF YOU UNMOUNT, ALL YOUR DATA WILL BE LOST\033[0m\n" << endl;
                cout << "\n\033[32mDO YOU WANT COPY ALL YOUR DATA INSIDE A BACKUP Archive? (Y/N): \033[0m"; cin >> COPY;
                cout << "\n\033[33mDO YOU WANT TO CONTINUE? (Y/N): \033[0m"; cin >> YN;
                transform(COPY.begin(),COPY.end(),COPY.begin(), ::towlower);
                transform(YN.begin(),YN.end(),YN.begin(), ::towlower);

                auto rsyncer = [](){
                    system("clear");
                    cout << "copying data and deleting" << endl; this_thread::sleep_for(chrono::seconds(1));
                    system("sync");
                    system("mkdir ./BACKUP");
                    system("rsync -aHAX --progress ./TMPFS/* ./BACKUP/");
                    system("sync");
                    system("tar -cf - ./BACKUP | pigz -6 > BACKUP.tar.gz");
                    system("rm -rf ./BACKUP/");
                    system("sync");
                };

                auto unmounter = [](){
                    system("clear");
                    cout << "unmounting and sycing" << endl; this_thread::sleep_for(chrono::seconds(1));
                    system("sync");
                    system("sudo umount -l ./TMPFS");
                    system("sudo rm -rf ./TMPFS");
                    system("sync");
                    cout << "Completed!" << endl;

                };
                map<string, function<void()>> rsync = {
                    {"y",rsyncer},
                    {"yes", rsyncer},
                    {"n", [](){cout << "" << endl;}},
                    {"no",[](){cout << "" << endl;}}
                };

                map<string, function<void()>> actions = {
                    {"y",unmounter},
                    {"yes", unmounter},
                    {"n", [](){system("clear"); cout << "\033[32mNothing Changed\033[0m\n" << endl;}},
                    {"no",[](){system("clear"); cout << "\033[32mNothing Changed\033[0m\n" << endl;}}
                };

                if (rsync.count(COPY))
                    rsync[COPY]();

                if (actions.count(YN))
                    actions[YN]();
                break;
            }
            case 3:
                system("clear");             
                cout << "Exiting\nSyncing" << endl; this_thread::sleep_for(chrono::seconds(1));
                system("sync");
                cout << "Completed!" << endl;
                return;
                break;
            
            default:
                cout << "Invalid Option" << endl;
                break;

        }
    }
}

void MKTMPFS() {
    auto TGB = [] () {\
        string input;
        cout << "\nType your Size (GB): "; cin >> input;

        if (!all_of(input.begin(),input.end(), ::isdigit)) {
            cout << "\033[31mInvalid Size\033[0m\n" << endl;
            return;
        }

        int GB = stoi(input);
        string mktmpfs = "sudo mount -t tmpfs -o size=" + to_string(GB) + "G tmpfs ./TMPFS";
        system("mkdir ./TMPFS");
        system(mktmpfs.c_str());
    };

    auto TMB = [] () {\
        string input;
        cout << "\nType your Size (MB): "; cin >> input;

        if (!all_of(input.begin(),input.end(), ::isdigit)) {
            cout << "\033[31mInvalid Size\033[0m\n" << endl;
            return;
        }

        int MB = stoi(input);

        string mktmpfs = "sudo mount -t tmpfs -o size=" + to_string(MB) + "M tmpfs ./TMPFS";
        system("mkdir ./TMPFS");
        system(mktmpfs.c_str());
    };

    string Choice; cout << "\nType the TMPFS Data UNIT (GB OR MB) OR Type Cancel to cancel: "; cin >> Choice;
    transform(Choice.begin(), Choice.end(), Choice.begin(), ::towlower);

    map<string, function<void()>> actions = {
        {"gb", TGB},
        {"mb", TMB},
        {"Cancel",[](){cout << "\033[32mProgress Canceled\033[0m\n" << endl;}},
        {"cancel",[](){cout << "\033[32mProgress Canceled\033[0m\n" << endl;}},

    };

    if (actions.count(Choice))
        actions[Choice]();
    else
        cout << "\033[31mInvalid unit\033[0m\n" << endl;
}

#include <iostream>
#include <vector>
#include <sstream>
#include <typeinfo>
#include <queue>
#include <algorithm>
using namespace std;
class Process{
    public:
        vector<int> data;
        int b1;
        int b2;
        int terminate;
        int id;
        int stopprint;
        void set_data(vector<int> d);
        void set_b1(int b);
        void set_b2(int b);
        void set_terminate(int t);
        void set_id(int i);
        void set_stopprint(int s);
};
void Process::set_b1(int b){
    b1 = b;
}
void Process::set_b2(int b){
    b2 = b;
}
void Process::set_terminate(int t){
    terminate = t;
}
void Process::set_data(vector<int> d){
    data = d;
}
void Process::set_id(int i){
    id = i;
}
void Process::set_stopprint(int s){
    stopprint = s;
}

bool keep_doing(vector<Process> myP){
    bool keep = false;
    for (int i = 0; i < myP.size() ; i++){
        if (myP[i].terminate == 0) {
            keep = true;
        }
    }
    return keep;
}
int main() // int argc, char** argv
{
    // Read data and initialize process vector
    std:: string data_all;
    getline (cin, data_all);
    std::istringstream is( data_all );
    int num_p, choice, n, count_4, id_index, rr_para;
    is >> num_p >> choice;
    if (choice == 2){
        is >> rr_para;
    }
    vector<Process> myProcess;
    vector<int> data_process;
    count_4 = 0;
    id_index = 0;
    while( is >> n ) {
        data_process.push_back(n);
        count_4 ++;
        if (count_4 == 4){
            count_4 = 0;
            Process newprocess;
            newprocess.set_b1(0);
            newprocess.set_b2(0);
            newprocess.set_terminate(0);
            newprocess.set_stopprint(0);
            data_process.push_back(1);
            newprocess.set_data(data_process);
            newprocess.set_id(id_index);
            myProcess.push_back(newprocess);
            // store current process data
            data_process.clear();
            id_index ++;
        }
    }

    // initialize queue
    queue <int> ready_process;
    for (int i = 0; i < num_p; i++){
        ready_process.push(i);
    }
    
    // initliaze running process and running round
    int running_processid = -1; // range from 0,1,....
    int running_round = -1;  // range from 1,2,3
    int time = 1;
    int running_rr_record = 0;
    while (keep_doing(myProcess) == true){
        vector<int> enqueue_candidate; // for tie breaking 
        //// handle running process
        // dequeue if none is running
        cout << "Time " << time << ":\n";
        if ((running_processid == -1) && (ready_process.empty() == false)){
            running_rr_record = 0;
            running_processid = ready_process.front();
            ready_process.pop();
            vector<int> get_data_for_round = myProcess[running_processid].data;
            if (get_data_for_round[0] != 0){
                running_round = 1;
            }
            else if (get_data_for_round[2] != 0){
                running_round = 2;
            }
            else{
                running_round = 3;
            }
        }
        int running_processid_copy = running_processid;
        // runtime -= 1 , block if needed
        if (running_processid != -1){
            running_rr_record ++;
            bool hit_bottom = false;
            vector<int> get_data_for_decrease = myProcess[running_processid].data;
            if (running_round == 1){
                get_data_for_decrease[0]--;
                if (get_data_for_decrease[0] == 0){
                    hit_bottom = true;
                }
            }
            else if (running_round == 2){
                get_data_for_decrease[2]--;
                if (get_data_for_decrease[2] == 0){
                    hit_bottom = true;
                }
            }
            else{
                get_data_for_decrease[4]--;
                hit_bottom = true;
                myProcess[running_processid].set_terminate(1);
            }
            myProcess[running_processid].set_data(get_data_for_decrease);
            if (hit_bottom == true){
                if (running_round == 1){
                    myProcess[running_processid].set_b1(1);
                }
                else if (running_round == 2){
                    myProcess[running_processid].set_b2(1);
                }
                else{
                    myProcess[running_processid].set_terminate(1);
                }
                running_processid = -1;
                running_round = -1;
            }
            else if ((choice == 2 ) && (running_rr_record == rr_para)){
                enqueue_candidate.push_back(running_processid_copy);
                running_processid = -1;
                running_round = -1;
            }
        }

        //// handle blocking process 
        // decrease blocktime and collect enqueue info

        vector<int> block_candidate;
        for (int i = 0; i < num_p ; i++){
            if (i == running_processid_copy){
                continue;
            }
            vector<int> get_data_for_decrease_block = myProcess[i].data;
            if (myProcess[i].b1 == 1){
                block_candidate.push_back(i);
                get_data_for_decrease_block[1] --;
                if (get_data_for_decrease_block[1] == 0){
                    myProcess[i].set_b1(0);
                    enqueue_candidate.push_back(i);
                }
            }
            else if (myProcess[i].b2 == 1){
                block_candidate.push_back(i);
                get_data_for_decrease_block[3] --;
                if (get_data_for_decrease_block[3] == 0){
                    myProcess[i].set_b2(0);
                    enqueue_candidate.push_back(i);
                }  
            }
            myProcess[i].set_data(get_data_for_decrease_block);
        }
        sort(enqueue_candidate.begin(), enqueue_candidate.end());
        
        // enqueue
        for (int i = 0; i<enqueue_candidate.size() ; i++){
            ready_process.push(enqueue_candidate[i]);
            
        }
        // clear enqueue_candidate for next-time use
        enqueue_candidate.clear();
        
        for (int i = 0; i <num_p;i++){
            cout << "Process " << i << " ";
            if (myProcess[i].stopprint == 1){
                cout << "\n";
                continue;
            }
            if (myProcess[i].terminate == 1){
                cout << "Terminate\n";
                myProcess[i].set_stopprint(1);
                continue;
            }
            if (i == running_processid_copy){
                cout << "running\n";
            }
            else if (std::find(block_candidate.begin(), block_candidate.end(), i) != block_candidate.end()){
                cout << "Blocked\n";
            }
            else{
                cout << "Ready\n";
            }
        }
        cout << "---------------------------\n";
        time ++;
    }
    return 0;
}

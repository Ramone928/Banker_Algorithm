//Ramone Fields
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

int ProcessCount(std::vector<std::string>);
int MatrixBeginning(std::vector<std::string>, std::string);
void DisplayInformation(std::vector<int>, std::vector<std::string>, int, int, int, int, int, int, std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::vector<int>);

std::vector<std::vector<int>> Table2D(std::vector<std::string>, int, int, int);
std::vector<int> AvailableTable(std::vector<std::string>);

std::vector<std::vector<int>> NeedTable2D(std::vector<std::vector<int>>, std::vector<std::vector<int>>, int);
std::vector<int> StateSafe(std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::vector<int>, int);
int ResourceCount(std::vector<std::string>);

int main() {

    std::ifstream input;

    std::vector<std::string> file_contents;


    std::string line;

    

    input.open("input.txt");

   
    if (input.is_open()){
        while (getline(input, line)) {
            file_contents.push_back(line);
        }
    }

    input.close();

    int number_of_resources = ResourceCount(file_contents);
    int number_of_processes = ProcessCount(file_contents);

    int allocation_matrix_start = MatrixBeginning(file_contents, "Allocation:");
    int allocation_matrix_end = allocation_matrix_start + number_of_processes;

    int max_matrix_start = MatrixBeginning(file_contents, "Max:");
    int max_matrix_end = max_matrix_start + number_of_processes;

    std::vector<int> available_table = AvailableTable(file_contents);

    std::vector<std::vector<int>> allocation_table = Table2D(file_contents, allocation_matrix_start, allocation_matrix_end, number_of_resources);
    std::vector<std::vector<int>> max_table = Table2D(file_contents, max_matrix_start, max_matrix_end, number_of_resources);
   
    std::vector<std::vector<int>> need_table = NeedTable2D(max_table, allocation_table, number_of_resources);
    std::vector<int> results = StateSafe(allocation_table, need_table, available_table, number_of_processes);
    DisplayInformation(results,file_contents, number_of_processes, number_of_resources, allocation_matrix_start, allocation_matrix_end, max_matrix_start, max_matrix_end, allocation_table, max_table, available_table);
    
    return 0;
}

int MatrixBeginning(std::vector<std::string> to_process, std::string string_to_match) {
    int token = -1;
    for (auto i=0; i < to_process.size(); ++i) {

        if (to_process[i] == string_to_match) {

            token = i + 1;
        }
    }


    return token;
}

int ProcessCount(std::vector<std::string> to_process) {
    int process_count = 0;

    for (auto i = 0; i < to_process.size(); ++i) {

        if (to_process[i] == "Allocation:" || to_process[i] == "Max:")
            process_count = 0;

        else if (to_process[i].length() > 1)
            ++process_count;
    }

    return process_count;
}


int ResourceCount(std::vector<std::string> to_process) {
    int resource_count = 0;

    for (auto i = 0; i < to_process.size(); ++i) {

        if (i == 0) {

            for (auto j = 0; j < to_process[i].length(); ++j){   

                if (to_process[i].at(j) != ' ')

                    ++resource_count;
            }
        }
    }

    return resource_count;
}

std::vector<std::vector<int>> NeedTable2D(std::vector<std::vector<int>> max, std::vector<std::vector<int>> available, int resource_num) {
    std::vector<int> temp;
    std::vector<std::vector<int>> temp_2d;

    for (auto i = 0; i < max.size(); ++i) {
        int count = 0;

        for (auto j = 0; j < max[i].size(); ++j) {

            ++count;
            temp.push_back(max[i][j] - available[i][j]);


            if ((count % resource_num) == 0) {

                temp_2d.push_back(temp);
                temp.clear();
                count = 0;
            }

        }
    }

    return temp_2d;
}

std::vector<int> AvailableTable(std::vector<std::string> to_process) {
    std::vector<int> temp;
    for (auto i = 0; i < to_process.size(); ++i) {

        if (i == 0) {
            for (auto j = 0; j < to_process[i].length(); ++j){  

                if (to_process[i].at(j) != ' ')
                    temp.push_back(to_process[i].at(j) - 48);
            }
        }
    }

    return temp;
}

std::vector<std::vector<int>> Table2D(std::vector<std::string> input_file, int start, int end, int resource_num) {
    std::vector<int> temp;
    std::vector<std::vector<int>> temp_2d;

    for (auto i = start; i < end; ++i) {
        int count = 0;
        for (auto j = 0; j < input_file[i].length(); ++j) {

            if (input_file[i].at(j) != ' ') {
                ++count;
                temp.push_back(input_file[i].at(j) - 48);

                if ((count % resource_num) == 0) {
                    temp_2d.push_back(temp);
                    temp.clear();
                    count = 0;
                }
            }
        }
    }

    return temp_2d;
}


std::vector<int> StateSafe(std::vector<std::vector<int>> allocation_tbl, std::vector<std::vector<int>> need_tbl, std::vector<int> available_tbl, int process_num) {
    std::vector<int> work = available_tbl;
    
    std::vector<bool> finish(process_num, false); 
   
    std::vector<int> process_order;
    
    while (process_order.size() < process_num) {
        
        for (auto i = 0; i < process_num; ++i) {
           
            if (finish[i]) 
                continue;

           
            if (!finish[i]) {
                bool need_less_than_work = false;

                for (auto j = 0; j < work.size(); ++j) {

                    if (need_tbl[i][j] < work[j])
                        need_less_than_work = true;

                    if (need_less_than_work)
                        continue;
                    
                }
            } 

            for (auto k = 0; k < work.size(); ++k) 
                work[k] += allocation_tbl[i][k];

            finish[i] = true;

            process_order.push_back(i);
        }

    }

   
    std::vector<int> deadlock(1,0);

    for (auto i = 0; i < finish.size(); ++i) {
        if (!finish[i]) {

            deadlock[0] = -1;
            continue;
        }
    }

   
    if (deadlock[0] == -1) {

        process_order.clear();
        process_order = deadlock;
    }

    
    return process_order;
}


void DisplayInformation(std::vector<int> result, std::vector<std::string> to_process, int process_num, int resource_num, int alloc_start, int alloc_end, int max_start, int max_end, std::vector<std::vector<int>> alloc_table, std::vector<std::vector<int>> mx_table, std::vector<int> avail_table  ) {
   
    std::cout << " Input.txt Information \n";

    std::cout << "Number of Processes: " << process_num << '\n';
    std::cout << "Number of Resources: " << resource_num << '\n';

    std::cout << "Allocation Start: " << alloc_start << '\n';
    std::cout << "Allocation End: " << alloc_end << '\n';

    std::cout << "Max Start: " << max_start << '\n';
    std::cout << "Max End: " << max_end << '\n';

    std::cout << "Allocation Table Size: " << alloc_table.size() << '\n';
    std::cout << "Max Table Size: " << mx_table.size() << '\n';

    std::cout << "\n";
    std::cout << " Available Table \n";

    std::cout << "    ";
    
    for (auto i = 0; i < avail_table.size(); ++i) {
       
        std::cout << avail_table[i] << " ";
    }
   
    std::cout << "\n";
  
    std::cout << " Allocation Table \n";
  
    for (auto i = alloc_start; i < alloc_end; ++i) {
        
        std::cout << "    " << to_process[i] << std::endl;
    }
    
    std::cout << "\n";
    std::cout << " Max Table \n";
   
    for (auto i = max_start; i < max_end; ++i) {
       
        std::cout << "    " << to_process[i] << std::endl;
    }


    std::cout << "\n Process State \n";
    if (result[0] != -1) {

        std::cout << "In safe state. \nProcess Order: ";

        for (int i = 0; i < result.size(); ++i) {

            std::cout << "P" << result[i];

            if(i != (result.size() - 1))
                std::cout << " -> ";
            else
                std::cout << "\n";
        }
        
    } else 
        std::cout << "Not in a safe state. Deadlock Detected";

}
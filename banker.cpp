//Ramone Fields
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
 
int processnum(std::vector<std::string>);
int matrix_start(std::vector<std::string>, std::string);
void displayInfo(std::vector<int>, std::vector<std::string>, int, int, int, int, int, int, std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::vector<int>);
 
std::vector<std::vector<int>> table_chart(std::vector<std::string>, int, int, int);
std::vector<int> availableTable(std::vector<std::string>);
 
std::vector<std::vector<int>> needtable_chart(std::vector<std::vector<int>>, std::vector<std::vector<int>>, int);
std::vector<int> stateSafe(std::vector<std::vector<int>>, std::vector<std::vector<int>>, std::vector<int>, int);
int resourceCount(std::vector<std::string>);
 
int main() {
 
   std::vector<std::string> fileinfo;
 
   std::string line;
 
   std::ifstream input;
 
   input.open("input.txt");
 
 
   if (input.is_open()){
       while (getline(input, line)) {
           fileinfo.push_back(line);
       }
   }
 
   input.close();
  
  
   int number_resources = resourceCount(fileinfo);
   int number_processes = processnum(fileinfo);
  
   int allocation_start = matrix_start(fileinfo, "Allocation:");
   int allocation_end = allocation_start + number_processes;
 
  
   int max_start = matrix_start(fileinfo, "Max:");
   int max_end = max_start + number_processes;
 
  
   std::vector<int> available_table = availableTable(fileinfo);
 
   std::vector<std::vector<int>> allocation_table = table_chart(fileinfo, allocation_start, allocation_end, number_resources);
   std::vector<std::vector<int>> max_table = table_chart(fileinfo, max_start, max_end, number_resources);
 
   std::vector<std::vector<int>> need_table = needtable_chart(max_table, allocation_table, number_resources);
   std::vector<int> results = stateSafe(allocation_table, need_table, available_table, number_processes);
   displayInfo(results,fileinfo, number_processes, number_resources, allocation_start, allocation_end, max_start, max_end, allocation_table, max_table, available_table);
  
   return 0;
}
 
int matrix_start(std::vector<std::string> to_process, std::string string_to_match) {
   int token = -1;
   for (auto i=0; i < to_process.size(); ++i) {
 
       if (to_process[i] == string_to_match) {
 
           token = i + 1;
       }
   }
 
 
   return token;
}
 
int processnum(std::vector<std::string> to_process) {
   int process_count = 0;
 
   for (auto i = 0; i < to_process.size(); ++i) {
 
       if (to_process[i] == "Allocation:" || to_process[i] == "Max:")
           process_count = 0;
 
       else if (to_process[i].length() > 1)
           ++process_count;
   }
 
   return process_count;
}
 
 
int resourceCount(std::vector<std::string> to_process) {
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
 
std::vector<std::vector<int>> needtable_chart(std::vector<std::vector<int>> max, std::vector<std::vector<int>> available, int resource_num) {
   std::vector<std::vector<int>> Temp_2d;
   std::vector<int> Temp;
 
   for (auto i = 0; i < max.size(); ++i) {
       int count = 0;
 
       for (auto j = 0; j < max[i].size(); ++j) {
 
           ++count;
           Temp.push_back(max[i][j] - available[i][j]);
 
 
           if ((count % resource_num) == 0) {
 
               Temp_2d.push_back(Temp);
               Temp.clear();
               count = 0;
           }
 
       }
   }
 
   return Temp_2d;
}
 
std::vector<int> availableTable(std::vector<std::string> to_process) {
   std::vector<int> Temp;
   for (auto i = 0; i < to_process.size(); ++i) {
 
       if (i == 0) {
           for (auto j = 0; j < to_process[i].length(); ++j){ 
 
               if (to_process[i].at(j) != ' ')
                   Temp.push_back(to_process[i].at(j) - 48);
           }
       }
   }
 
   return Temp;
}
 
std::vector<std::vector<int>> table_chart(std::vector<std::string> input_file, int start, int end, int resource_num) {
   std::vector<std::vector<int>> Temp_2d;
 
   std::vector<int> Temp;
 
   for (auto i = start; i < end; ++i) {
      
       int count = 0;
     
       for (auto j = 0; j < input_file[i].length(); ++j) {
 
         
           if (input_file[i].at(j) != ' ') {
             
               ++count;
              
               Temp.push_back(input_file[i].at(j) - 48);
 
              
               if ((count % resource_num) == 0) {
                 
                   Temp_2d.push_back(Temp);
                   Temp.clear();
                 
                   count = 0;
               }
           }
       }
   }
 
   return Temp_2d;
}
 
 
std::vector<int> stateSafe(std::vector<std::vector<int>> allocation_tbl, std::vector<std::vector<int>> need_tbl, std::vector<int> available_tbl, int process_num) {
   std::vector<bool> finish(process_num, false);
 
   std::vector<int> processorder;
 
   std::vector<int> work = available_tbl;
  
  
   while (processorder.size() < process_num) {
      
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
 
           processorder.push_back(i);
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
 
       processorder.clear();
       processorder = deadlock;
   }
 
  
   return processorder;
}
 
 
void displayInfo(std::vector<int> result, std::vector<std::string> process, int processnumumber, int resourcenumber, int allocstart, int allocfinish, int maxstart, int maxfinish, std::vector<std::vector<int>> alloc_tbl, std::vector<std::vector<int>> mx_tbl, std::vector<int> avail_tbl  ) {
 
    std::cout << "Allocation Start: " << allocstart << '\n';
   std::cout << "Allocation End: " << allocfinish << '\n';
 
   std::cout << "Number of Processes: " << processnumumber << '\n';
   std::cout << "Number of Resources: " << resourcenumber << '\n';
  
   std::cout << "Allocation Table Size: " << alloc_tbl.size() << '\n';
   std::cout << "Max Table Size: " << mx_tbl.size() << '\n';
 
     std::cout << "Max Start: " << maxstart << '\n';
   std::cout << "Max End: " << maxfinish << '\n';
 
 
   std::cout << "\n";
    std::cout << " Allocation Table \n";
    for (auto i = allocstart; i < allocfinish; ++i) {
      
       std::cout << "    " << process[i] << std::endl;
   }
  
   std::cout << "\n";
   std::cout << " Max Table \n";
 
   for (auto i = maxstart; i < maxfinish; ++i) {
     
       std::cout << "    " << process[i] << std::endl;
   }
 
   std::cout << " Available Table \n";
 
   std::cout << "    ";
  
   for (auto i = 0; i < avail_tbl.size(); ++i) {
     
       std::cout << avail_tbl[i] << " ";
   }
 
   std::cout << "\n";
 
 
   std::cout << "\n This is the Process State. \n";
   if (result[0] != -1) {
 
       std::cout << " Process in safe state. \nProcess Order: ";
 
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

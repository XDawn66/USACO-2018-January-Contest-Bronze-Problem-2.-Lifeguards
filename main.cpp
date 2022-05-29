//By Zhenyu Jiang
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>

struct staff
{
  int start;
  int end;
};

void selection_sort(std::vector<staff>& a, int size) {
  for (int i = 0; i < size - 1; i++) {
    int min_pos = i;
    for (int j = i + 1; j < size; j++) {
      if (a[j].start < a[min_pos].start) {
        min_pos = j;
      }
    }
    staff temp = a[i];
    a[i] = a[min_pos];
    a[min_pos] = temp;
  }
}

void fillstaff(staff arr[], int& num_of_staff) //this function will create as many as staff struct which contians their own work schedules and fill those staff struct into an array
{
  std::string line;
  std::ifstream file("input.txt");
    if(file.is_open())
    {
      std::getline(file, line);
      num_of_staff = std::stoi(line);
    }
    int counter = 0;
    while(getline(file,line) && counter < num_of_staff)
      {
        staff a;
        std::istringstream sinput(line);
        sinput>>a.start>>a.end;
        arr[counter] = a;
        counter++;
      }
}

void creatinit(std::vector<staff>& a, staff arr[], int num_of_staff) //this function will generate the first combination
{
  for(int i = 0; i<num_of_staff-1; i++)
  {
    a.push_back(arr[i]);
  }
}

int calhours(std::vector<staff> current, int num_of_Staff) //Everytime this function get called, it will calculate the total time cover by the given combination
{
  int hour = 0;
  std::vector<staff> a = current;
  selection_sort(a,num_of_Staff-1);
  int from = a[0].start;
  int to  = a[0].end;
  for(int i = 1; i < a.size(); i++)
  {
    if(to < a[i].start)
    {
        hour += to - from;
        from = a[i].start;
        to = a[i].end;
    }
    else if(from < a[i].start && to < a[i].end) 
    {
      to = a[i].end;
    }
    if(i == a.size()-1)
    {
      hour += to - from;
    } 
  }
  return hour;
}

void tryall(staff arr[], int& max, int num_of_staff) //This function will try all the possible combinations and find out the maximum hours that given staffs can covered
{
  int temp = 0;
  int total_combination = num_of_staff; 
  std::vector<staff> oneline; //this vecotr will contain numbers of struct staff
  creatinit(oneline,arr,num_of_staff);
  int current_index = oneline.size()-1;
  staff current_limit = arr[current_index+1];
  max = calhours(oneline, num_of_staff); 
  int count = 1;
  for(int i = 1; i < total_combination; i++) //we start at one becasue the first combination is calculated
  {
    if(oneline[current_index].start != current_limit.start && oneline[current_index].end != current_limit.end ) //if the current index doesn't reach the limit(the last staff)
    {
      oneline[current_index] = arr[current_index+count]; //
      temp = calhours(oneline,num_of_staff);
      count++;
    }
    else //if the current index reaches its "limit"
    {
      count = 1; //reset the counter
      for(int j = current_index; j < total_combination; j++) //generate the new combination
      {
        if(j-1 >= 0)
        {
          oneline[j-1] = arr[j];
        }
      }
      temp = calhours(oneline,num_of_staff);
      current_index--; //move the current index to left index
      current_limit = arr[current_index+1]; //change the limit that the new index can reach
    }
    if(temp > max)
    {
      max = temp;
    }
  }
}

int main()
{
    staff arr[100]; //this array will act like a database that holds all staffs' schedule, and it will be used to created different combinations.
    //However, the data inside will not be changed after filling staffs
    int num_of_staff = 0;
    int max;
    std::string text;
    fillstaff(arr,num_of_staff);
    tryall(arr,max,num_of_staff);
    std::cout<<max;
}

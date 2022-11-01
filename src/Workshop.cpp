#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

/* 
A student signed up for  workshops and wants to attend the maximum number 
of workshops where no two workshops overlap. You must do the following:

Implement  structures:

struct Workshop having the following members:

The workshop's start time.
The workshop's duration.
The workshop's end time.
struct Available_Workshops having the following members:

An integer,  (the number of workshops the student signed up for).
An array of type Workshop array having size .
Implement  functions:

Available_Workshops* initialize (int start_time[], int duration[], int n)
Creates an Available_Workshops object and initializes its elements using the elements in the  and  parameters (both are of size ). Here,  and  are the respective start time and duration for the  workshop. This function must return a pointer to an Available_Workshops object.

int CalculateMaxWorkshops(Available_Workshops* ptr)
Returns the maximum number of workshops the student can attend—without overlap. The next workshop cannot be attended until the previous workshop ends.

Note: An array of unknown size () should be declared as follows:

DataType* arrayName = new DataType[n];
Input Format

Input from stdin is handled by the locked code in the editor; you simply need to write your functions to meet the specifications of the problem statement above.

Constraints

Output Format

Output to stdout is handled for you.

Your initialize function must return a pointer to an Available_Workshops object.
Your CalculateMaxWorkshops function must return maximum number of non-overlapping 
workshops the student can attend.
*/

//Define the structs Workshops and Available_Workshops.
struct Workshop {
  Workshop() = default;
  void SetTime(int const&, int const&);
  ostream& DisplayWorkshop(ostream& out) const {
    return out << "( " << m_startTime << ", " << m_duration << " ) = " << m_endTime;
  }
  int m_startTime;
  int m_duration;
  int m_endTime;
};

bool operator!=(Workshop const& lhs, Workshop const& rhs) {
  return lhs.m_startTime != rhs.m_startTime || lhs.m_endTime != rhs.m_endTime;
}

void Workshop::SetTime(int const& startTime, int const& duration) {
  m_startTime = startTime;
  m_duration = duration;
  m_endTime = startTime + duration;
}

struct Available_Workshops {
  Available_Workshops(int, int[], int[]);
  void SortWorkshop();
  int m_numberOfWorkshops;
  vector<Workshop> arrayOfWorkshops; // I'm using a vector to avoid defining function to free the space
};

Available_Workshops::Available_Workshops(int n, int startTime[], int duration[]) 
  : m_numberOfWorkshops(n), arrayOfWorkshops(n) {
    
    for (int i = 0; i < n; ++i) {
      arrayOfWorkshops[i].SetTime(startTime[i], duration[i]);
    } 
}

void Available_Workshops::SortWorkshop() {
  sort(arrayOfWorkshops.begin(), 
       arrayOfWorkshops.end(),
       [](Workshop const& lhs, Workshop const& rhs) {
    return lhs.m_endTime < rhs.m_endTime;
  });
}

//Implement the functions initialize and CalculateMaxWorkshops
Available_Workshops* initialize(int startTime[], int duration[], int n) {
  return new Available_Workshops(n, startTime, duration);
}

int CalculateMaxWorkshops(Available_Workshops* ptr) {
  // every couple (x,y), start-duration, has a length determined by the sum x+y, when the workshop ends. 
  // The steps to solve the problem are
  // 1. sorted the workshop based on the ends, O(n log(n))
  // 2.  I start scanning all the items in the vector, incrementing by one the counter
  // which would be returned and deleting whose couples which starts before the ends of the couple 
  // picked, O(n). Therefore the algorithm takes O(n log(n)) in terms of time.
  // For example: (0,2) c{1} => delete (0,5). Then (2,5) => c{1}.
  // Pseudo code:
  // sort the vector
  // for each workshop in vector if currentWorkshop.start < previousEnd || same couple => not counting,
  //  otherwise increment by one the counter and set new couple as current couple
  ptr->SortWorkshop();
  
  int counter{1};
  Workshop currenWorkshop(ptr->arrayOfWorkshops[0]);
  for (Workshop const& wk : ptr->arrayOfWorkshops) {
    if (wk != currenWorkshop && wk.m_startTime >= currenWorkshop.m_endTime) {
      currenWorkshop = wk;
      counter += 1;
    }
  }
  return counter;

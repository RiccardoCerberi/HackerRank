#include<iostream>
#include<vector>
#include<string>
#include<map>

using namespace std;

// this is the hardest part of a the spell problem in HackeRank. It consists in calculating the longest
// common substring length.

void InitMatrix(vector<vector<int>>& v, int const& row, int const &col) {
  for (int i = 0; i < row; ++i) {
    vector<int> temp;
    for (int j = 0; j < col; ++j) {
      temp.push_back(0);
    }
    v.push_back(temp);
  }
}

int CalcSubStringLengthAux(vector<vector<int>> const& matrixOfComparison, int const& i, int const& j) {
  int subJournalStringIndex{i - 1}, subScrollNameStringIndex{j-1};

  if (i - 1 >= 0 && j - 1 >= 0) {
    return matrixOfComparison[subJournalStringIndex][subScrollNameStringIndex];
  } 
  return 0;
}

int MaxSubStringLengthAux(vector<vector<int>> const& matrixOfComparison, int const& i, int const& j) {
  int lengthDeletingScrollLastChar{0}, lengthDeletingJournalLastChar{0};
  if (j - 1 >= 0) {
    lengthDeletingScrollLastChar = matrixOfComparison[i][j - 1];
  } else {
    lengthDeletingScrollLastChar = 0;
  }
  if (i - 1 >= 0) {
    lengthDeletingJournalLastChar = matrixOfComparison[i - 1][j];
  } else {
    lengthDeletingJournalLastChar = 0;
  }
  return max(lengthDeletingJournalLastChar, lengthDeletingScrollLastChar);
}

int CalcSubStringLength(string const& journal, string const& scrollName) {
  // The idea is to compare the last two characters of the string porvided: If they're equal the longest
  // subsequence length would be 1 + the longest subsequence calculating by passing the current strings
  // except for the last characters already computed; in case of difference journal contains a
  // subsequence of scrollName characters except for the one just computed or viceversa. The results obtain
  // by this two ways must be compared to find out the maximum value. 
  // This method can be implemented by recursion or, as I did, by dynamic programming. The only difference
  // between these two strategies is where to start from: in the first case from the last character in the 
  // second one from the first character, comparing it with the other string being aware of the constraints
  // due to the indexes.

  // I'm going to set the matrix in this way: journal's characters vertically (every character is an index)
  // and scrollName's characters orizzontally. 
  // therefore if we had journal = abc and scrollname = bc, then the matrix would be: 
  //    b c
  //  a
  //  b
  //  c
  // This solution can be reached using recursion, where isntead

  vector<vector<int>> matrixOfComparison;

  int const row = journal.length();
  int  const col = scrollName.length();

  InitMatrix(matrixOfComparison, row, col);

  string subString;
  for (int i = 0; i < row; ++i) {
    for (int j = 0; j < col; ++j) {
      if (journal[i] == scrollName[j]) {
        matrixOfComparison[i][j] = 1 + CalcSubStringLengthAux(matrixOfComparison, i, j);
      } else {
        matrixOfComparison[i][j] = MaxSubStringLengthAux(matrixOfComparison, i, j);
      }
    }
  }
  cout << "The substring is: " << subString << endl;
  return matrixOfComparison[row - 1][col - 1];
}

// when it's used recursively remember to compare the two substrings producted by the two different recursive
// calls.
void SubStringAux(string const& s1, string const& s2, int lasts1, int lasts2, string& subString) {
  if (s1[lasts1] == s2[lasts2]) {
    subString += 1;
    SubStringAux(s1, s2, lasts1 - 1, lasts2 - 1, subString);
  } else {

  }
}

string SubString(string const& s1, string const& s2) {
  string subString;
  SubStringAux(s1,s2, s1.length() - 1, s2.length() - 1, subString);
  return subString;
}
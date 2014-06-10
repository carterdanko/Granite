#include <iostream>
using namespace std;
void MoveDisk( int NumDisks, char SrcPeg, char DestPeg, char TempPeg);

int main(int argc, char* argv[])
{
   int DiskHt;

  if (argc==1)
    DiskHt = 3;
  else
    DiskHt=atoi(argv[1]);
  MoveDisk(DiskHt, 'A', 'C', 'B');


  return 1;
}

void MoveDisk( int NumDisks, char SrcPeg, char DestPeg, char TempPeg)
{

  // This routine solves a single move with each call

  if (NumDisks == 1)
   cout << "Move a disk from " << SrcPeg << " to " << DestPeg << endl;
  else
  {
   MoveDisk(NumDisks - 1, SrcPeg, TempPeg, DestPeg);
   cout << "Move a disk from " << SrcPeg << " to " << DestPeg << endl;
   MoveDisk(NumDisks - 1, TempPeg, DestPeg, SrcPeg);
  }
  return;
}

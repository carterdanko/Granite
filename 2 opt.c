#include <nodelist.h>

long TwoOpt (numOfVert, Path)
   unsigned numOfVert;
   NODE *Path;
{
   unsigned ArcCost();
   long CircuitCost;
   unsigned count, index, pindex, sindex, k;
   unsigned D1, D2, D3, D4;
   NODE *First, *Last, *Kth, *Save, *Reverse;

   count = 1;
   First = Path;
   do {
      Last = First->prior;
      Kth = First->next;
      do {
         D2 = ArcCost (First->position, Kth->next->position)  /* 2-Opt */
            + ArcCost (Kth->position, Last->position);
         D4 = ArcCost (First->position, Last->position)
            + ArcCost (Kth->position, Kth->next->position);
         if (D2 < D4) {
            for ( Reverse = First; Reverse != Kth; Reverse = Save) {
               Save = Reverse->next;
               Reverse->next = Reverse->prior;
               Reverse->prior = Save;
            }
            First->next = Kth->next;
            Kth->next->prior = First;
            Kth->next = Kth->prior;
            Kth->prior = Last;
            Last->next = Kth;
            count = 0;
            First = Last->next;
            Kth = First->next;
         } else
            Kth = Kth->next;
      } while ((Kth != Last->prior->prior) && (count != 0));
      if (count != 0)
         First = First->next;
      count++;
   } while (count < numOfVert);
   Last = First->prior;
   CircuitCost = ArcCost (Last->position, First->position);
   for ( Kth = First; Kth != Last; Kth = Kth->next)
      CircuitCost += ArcCost (Kth->position, Kth->next->position);
   return (CircuitCost);
}
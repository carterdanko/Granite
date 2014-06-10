      program fortran
      INTEGER SIZE
      INTEGER i, j
      PARAMETER (SIZE = 10)
      
      real vectorOne(size)
      real vectorShift(size)

      forall (i=1:size) 
	vectorOne(i)=i
      end forall
      

      vectorShift = cshift(vectorOne,10)

      print*, vectorShift

      end program fortran

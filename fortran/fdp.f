      program fortran
      INTEGER SIZE
      INTEGER i, j
      PARAMETER (SIZE = 10)
      
      real vectorOne(size), vectorTwo(size)
      real vectorResult(size)      

      forall (i=1:size) 
	vectorOne(i)=i
	vectorTwo(i)=(i*3)
	vectorResult(i) = vectorOne(i) * vectorTwo(i)
      end forall

      print*, vectorResult
      
      end program fortran

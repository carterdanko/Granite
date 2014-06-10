      program fortran
      INTEGER SIZE
      INTEGER i, j
      PARAMETER (SIZE = 10)
      
      real matrixTwo(size,size)
      real vectorOne(size)
      real matrixTranspose(size, size)

      forall (i=1:size) 
	vectorOne(i)=i
      end forall
      

      forall (i=1:size)
      forall (j=1:size)
	matrixTwo(i,j) = i * j * vectorOne(j)
      end forall
      end forall


      matrixTranspose = transpose(matrixTwo)

      print*, matrixTranspose

      end program fortran

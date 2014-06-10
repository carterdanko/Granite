      program fortran
      INTEGER SIZE
      INTEGER i, j
      PARAMETER (SIZE = 1000)
      
      real matrixOne(size,size), matrixTwo(size,size)
      real vectorOne(size), vectorTwo(size)
      real matrixMatmul(size,size), vectorResult(size)      
      real matrixTranspose(size, size), vectorShift(size)

      forall (i=1:size) 
	vectorOne(i)=i
	vectorTwo(i)=(i*3)
	vectorResult(i) = vectorOne(i) * vectorTwo(i)
      end forall
      

      forall (i=1:size)
      forall (j=1:size)
        matrixOne(i,j) = i + j + vectorTwo(j)
	matrixTwo(i,j) = i * j * vectorOne(j)
      end forall
      end forall


      matrixMatmul = matmul(matrixOne, matrixTwo)
      matrixTranspose = transpose(matrixTwo)
      vectorShift = cshift(vectorOne,10)

      end program fortran

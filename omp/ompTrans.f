      program fortranomp
      integer size, i, j, chunk, temp
      parameter (size = 10)
      real matrixTwo(size, size)
      real matrixResult(size, size)
      real matrixTrans(size, size)
      chunk = 1

!$omp parallel shared(matrixTwo, chunk)

!$omp do schedule(static, chunk)
      do 10 i=1, size
	do 10 j=1, size
	    matrixTwo(i,j) = i * j * j
  10  continue

!$omp do schedule(static, chunk)
      do 20 j=1, size
	do 20 i= j + 1, size
	    temp = matrixTwo(i,j)
	    matrixTwo(i,j) = matrixTwo(j,i)
	    matrixTwo(j,i) = temp
  20  continue

!$omp end parallel
      print*, matrixTwo
      end 

      program fortranomp
      integer size, i, j, chunk, shift, test
      parameter (shift = 10)
      parameter (chunk = 10)
      parameter (size = 10)
      real vectorOne(size), temp(size)
      real vectorShift(size)
      count = 0

!$omp parallel shared(vectorOne, vectorShift, chunk)

!$omp do schedule(static, chunk)
      do 10 i=1, size
	vectorOne(i) = i
  10  continue

!$omp do schedule(static, chunk)
      do 20 j = 1, (size - shift)
	temp(j) = vectorOne(j + shift)
  20  continue

!$omp do schedule(static, chunk)
      do 30 j = 1, shift
	temp(size - shift + j) = vectorOne(j)
  30  continue

!$omp end parallel
      print*, vectorOne 
      end program

      program fortranomp
      integer size, i, chunk
      parameter (size = 10)
      parameter (chunk = 100)
      real vectorResult(size)

!$omp parallel shared(vectorResult, chunk)

!$omp do schedule(static, chunk)
      do 10 i=1, size
	vectorResult(i)= i * i * 3
  10  continue

!$omp end parallel
      print*, vectorResult
      end program
     

      program fortranomp
      integer size, i, j, k, chunk
      parameter (size = 10)
      real*8 A(size, size)
      real*8 B(size, size)
      real*8 C(size, size)
      chunk = 100

!$omp parallel shared(a,b,c,chunk) private(i,j,k)

!$omp do schedule (static, chunk)
      do 30 j=1, size
	 do 30 i=1, size
	   A(i,j) = i + j + (j*3)
  30  continue

!$omp do schedule (static, chunk)
      do 40 j=1, size
	do 40 i=1, size
	  B(i,j) = i * j * j
  40  continue

!$omp do schedule (static, chunk)
      do 50 j=1, size
	do 50 i=1, size
	  C(i,j) = 0
  50  continue

!$omp do schedule (static, chunk)
      do 60 i=1, size
	do 60 j=1, size
	  do 60 k=1, size
	     C(i,j) = C(i,j) + A(i,k) * B(k,j)
  60  continue

!$omp end parallel
      
      print*, C      

      end 


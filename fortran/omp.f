      program omp
      integer i, j, chunk, size
      PARAMETER (size=10)
      CHUNK = 10


      real*8 m1(size,size), m2(size,size)
      real*8 v1(size), v2(size)
c      real*8 matrixMatmul(size,size), vectorResult(size)      
c      real*8 matrixTranspose(size, size), vectorShift(size)
  

!$OMP PARALLEL SHARED(m1,m2,v1,v2chunk) PRIVATE(i,j)

!$OMP DO SCHEDULE(static, chunk)
      DO 10 i=1, size
	v1(i)=i
	v2(i)=(i*3)
  10  CONTINUE

!$OMP DO SCHEDULE(STATIC, CHUNK)
      DO 30 i=1, size
	DO 30 i=1, size
	  m1(i,j) = i + j + v2(j)
	  m2(i,j) = i * j * v1(j)
  30  CONTINUE

!$OMP END PARALLEL

      END program omp

c      matrixMatmul = matmul(matrixOne, matrixTwo)
c      matrixTranspose = transpose(matrixTwo)
c      vectorShift = cshift(vectorOne,10)

c      end program fortran

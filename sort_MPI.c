#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi.h>

//DECLARED FUNCTION
void create_pointer(int** matrix, int y, int value); //to creater rows in a dynamic matrix of int
void initialize_matrix(int** matrix, int x, int y); //to set all value of a int matrix to 0
void read_matrix(FILE* matrix_file, int x_num, int y_num, int b_num, int**x, int* y); //to read feature matrix
int count_member(int* avaiable, int comm_sz);
set_avaiable(int* avaiable_sx, int* avaiable_dx,int num_member, int comm_sz);
int obtain_first(int* avaiable, int comm_sz);
int obtain_last(int* avaiable, int comm_sz);
void merge(int arr[], int l, int m, int r);

main(int argc, char **argv)
{
	int comm_sz;
	int my_rank;
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	
	
	int root=0;
	
	int rest;
	
	//GENRAL VARIABLE
		/*used for cycle*/
		int i;
		int j;
			
		/*used for performance measure*/
		struct timeval start, end;
		
		/*access to first line infos*/
   		int	x_num=0; //number of x
		int y_num=0; //numbero of y
		int b_num=0; //number of channels
		
		int limit;
	
		int *y; 
   		int **x; 
   		int** x_transpose;
   		int** x_product;
   		
	
	if(my_rank == 0)//only process 0 execute reading and initializing of file
	{
	
		//ACCESS TO FILE
		/*access to file*/
		FILE* matrix_file=fopen(argv[1], "r");
		
		fscanf( matrix_file, "%i", &x_num);
		getc(matrix_file);
		fscanf( matrix_file, "%i", &y_num);
		getc(matrix_file);
		fscanf( matrix_file, "%i", &b_num);
		 limit = x_num*y_num;
		
		

 	   //1. USED MATRICES CONSTRUCTION
  	  /*construction of feature and result matrix*/

 	   	y=(int*)malloc((x_num*y_num) * sizeof(int));
    	x=(int **)malloc((x_num*y_num) * sizeof(int*));
  
    	 x_transpose=(int **)malloc((b_num+2) * sizeof(int*)); //for the transpose
    	 x_product=(int **)malloc((b_num+2) * sizeof(int*)); //for the product of x and transpose
    	/*initialization of matrix used*/
		//for x
		gettimeofday(&start, NULL);
		create_pointer(x, (x_num*y_num), (b_num+2));
		initialize_matrix(x, (b_num+2), (x_num*y_num));	 //all its value are setted to 0
		gettimeofday(&end, NULL);
    	printf("time for creating and initializing x matrix %ld\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec)));
		//for x_transpose
		gettimeofday(&start, NULL);
		create_pointer(x_transpose, (b_num+2), (x_num*y_num));
		gettimeofday(&end, NULL);
    	printf("time for creating and initializing x_transpose matrix %ld\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec)));
		//for x_product
    	gettimeofday(&start, NULL);
		create_pointer(x_product, (b_num+2), (b_num+2));
		gettimeofday(&end, NULL);
    	printf("time for creating and initializing x_product matrix %ld\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec)));
		/*read and create X matrix*/
   		 gettimeofday(&start, NULL);
   		 read_matrix(matrix_file,  x_num, y_num, b_num, x,  y);
		gettimeofday(&end, NULL);
   		 printf("time for reading main matrix %ld\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec)));
		/*close the file*/
		fclose(matrix_file);

		
	}
	
	//broadcast subdivision of vertically matrix
	MPI_Bcast(&b_num, 1, MPI_INT, root, MPI_COMM_WORLD);
		//broadcast subdivision of vertically matrix
	MPI_Bcast(&x_num, 1, MPI_INT, root, MPI_COMM_WORLD);
	//broadcast subdivision of vertically matrix
	MPI_Bcast(&y_num, 1, MPI_INT, root, MPI_COMM_WORLD);
	//broadcast subdivision of vertically matrix
	MPI_Bcast(&limit, 1, MPI_INT, root, MPI_COMM_WORLD);



	MPI_Barrier(MPI_COMM_WORLD );



	
			
		
	
		
				
				
			if(my_rank==0)
			{
					gettimeofday(&start, NULL);
				
			}
			
		
			
			int* avaiable=(int*)malloc((comm_sz)*sizeof(float));
			int* avaiable_sx=(int*)malloc((comm_sz)*sizeof(float));
			int* avaiable_dx=(int*)malloc((comm_sz)*sizeof(float));

			
			if(my_rank==0)
			{
				
				
				//4. EIGENVALUE CALCULATION
				float* eigenvalues=(float*)malloc((b_num+2) * sizeof(float));
				/*temporary*/
				for(i=0; i<(b_num+2); i++)
				{
					eigenvalues[i]=i;
				}
				//eigenvectors matrix
				float** eigenvectors=(float**)malloc((b_num+2) * sizeof(float*));
				create_pointer_float(eigenvectors, (b_num+2), (b_num+2));
				/*temporary*/
				for(i=0; i<(b_num+2); i++)
				{
					for(j=0; j<(b_num+2); j++)
					{
						eigenvectors[i][j]=i*j;
					}
		
				}
				
				
				
				//5. SORTING
				
				for(i=0; i<comm_sz; i++)
				{
					avaiable[i]=i;
					avaiable_sx[i]=i;
					avaiable_dx[i]=i;
				}
				avaiable[0]=-1;
				avaiable_sx[0]=-1;
					avaiable_dx[0]=-1;
				
				int num_member=count_member(avaiable, comm_sz);
				
				if(num_member==1)
				{
					int first=obtain_first(avaiable, comm_sz);
					//sendo all ultimo rimasto
					
					//mi metto in ascolto
					
				}else if(num_member>1)
				{
					//divido ai due lati
					
					
						int divisione=0;
						int first;
						int last;
				
						if(((b_num+2)%2)==0)
						{
							divisione=(b_num+2)/2;	
							int* first_half=(float*)malloc((divisione) * sizeof(float*));
							for(i=0; i<divisione; i++)
							{
								first_half[i]=eigenvectors[i];
							}
									
							int* second_half=(float*)malloc((divisione) * sizeof(float*));
							for(i=divisione; i<(b_num+2); i++)
							{
								second_half[i]=eigenvectors[i];
							}		
							
							set_avaiable(avaiable_sx, avaiable_dx, num_member, comm_sz);	//setta gli elementi avaiable a dx e sx
							int first=obtain_first(avaiable, comm_sz);
							int last=obtain_last(avaiable, comm_sz);
							avaiable_sx[first]=-1;
							avaiable_dx[last]=-1;
							
							//sendo a sx
							//sendo a dx
							
									
						}else
						{
							divisione=(b_num+1)/2;
							divionse+=1;
							divisione=(b_num+2)/2;	
							int* first_half=(float*)malloc((divisione) * sizeof(float*));
							for(i=0; i<divisione; i++)
							{
								first_half[i]=eigenvectors[i];
							}
					
							int* second_half=(float*)malloc((divisione) * sizeof(float*));
							for(i=divisione; i<(b_num+2); i++)
							{
								second_half[i]=eigenvectors[i];
							}		
							
							set_avaiable(avaiable_sx, avaiable_dx, num_member, comm_sz);	//setta gli elementi avaiable a dx e sx
							first=obtain_first(avaiable, comm_sz);
							 last=obtain_last(avaiable, comm_sz);
							avaiable_sx[first]=-1;
							avaiable_dx[last]=-1;
							
							//sendo a sx
							//sendo a dx
															
						}
				
				
				//mi metto in ascolto per ricomporre
				
				int* buffer1=(float*)malloc((b_num+2) * sizeof(float));
				int* buffer2=(float*)malloc((b_num+2) * sizeof(float));
				
				//ricevo
				
				//mergio
				merge(eigenvec)
						
					
				}else
				{
					quicksort(eigenvalues, b_num);
				}
				
				
				
					
					
						
			
				
				
				
				
			
			}else
			{
				//ricevo i due vettori
				//setto me stesso a -1
				
				int num_member=count_member(avaiable, comm_sz);
				
				if(num_member==1)
				{
					//sendo all ultimo rimasto e mi metto in ascolto (uguale all altro)
					
				
					
				}else if(num_member>1)
				{
					
					//divido
					//setto a meno uno gli el. di sx di avaiable che mando a dx compreso il prossimo (uguale all altro )
				
					
				}else
				{
					quicksort(eigenvalues, b_num);
				}
				
				
				
			}
			
		

			
			if(my_rank==0)
			{
				
				
			
				
				
				gettimeofday(&end, NULL);
				printf("time for calculate product+transpose for all features %ld\n", ((end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec)));
	
	
	
	
			}
			
			MPI_Barrier(MPI_COMM_WORLD );




	
	return 0;
	
	
}

void create_pointer(int** matrix, int y, int value)
{
	 int i;

	 for(i=0; i<y; i++){
    	
    	matrix[i] = (int *)malloc((value) * sizeof(int));
 
	}
}


void initialize_matrix(int** matrix, int x, int y)
{
	int i;
	int j;
	
	for(i=0; i<y; i++)
	{
		for(j=0; j<x; j++)
		{
			matrix[i][j]=0;
		}
	}
	

}

void read_matrix(FILE* matrix_file, int x_num, int y_num, int b_num, int**x, int* y)
{
	int i;
	int j;
	
	for(i=0; i<(x_num*y_num); i++)
    {
    	
	
    	for(j=0; j<(b_num+2); j++)
    	{
    		int matrix_value;
    		fscanf( matrix_file, "%i", &matrix_value);
    		getc(matrix_file);		
    		
			x[i][j]=matrix_value;
			

    	
		}
	}	
}



void transpose_calculation(int** x, int** x_transpose, int limit, int b_num)
{
	int i;
	int j;
	
	
	
	for(i=0; i<limit; i++)
	{
		for(j=0; j<b_num+2; j++)
		{
			x_transpose[j][i]=x[i][j];
		}
	}
}

int count_member(int* avaiable, int comm_sz)
{
	int i=0;
	int counter=0;
	for (int i=0; i<comm_sz; i++)
	{
		if(avaiable[i]!=-1)
		{
			counter=counter+1;
		}
	}
	return counter;
}

set_avaiable(int* avaiable_sx, int* avaiable_dx,int num_member, int comm_sz)
{
	int i=0;
	if((num_member%2)==0)
	{
		half=num_member/2;
		for(i=0; i<half; i++)
		{
			avaiable_dx[i]=-1;
		}
		for(i=half; i<comm_sz; i++)
		{
			avaiable_sx[i]=-1;
		}
	}else
	{
			half=(num_member/2)+1;
		for(i=0; i<half; i++)
		{
			avaiable_dx[i]=-1;
		}
		for(i=half; i<comm_sz; i++)
		{
			avaiable_sx[i]=-1;
		}
		
	}
}


int obtain_first(int* avaiable, int comm_sz)
{
	int i=0;
	int result=0;
	for(i=0; i<comm_sz; i++)
	{
		if (avaiable[i]!=-1)
		{
			result=i;
			break;
		}
	}
	return result;
}
int obtain_last(int* avaiable, int comm_sz)
{
	int i=0;
	int result=0;
	for(i=comm_sz-1; i>=0; i--)
	{
		if (avaiable[i]!=-1)
		{
			result=i;
			break;
		}
	}
	return result;
}

void merge(int arr[], int l, int m, int r)
{
    
}
 

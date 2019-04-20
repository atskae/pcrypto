#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

/*
    Parallelize using tasks
*/

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

#define NUM_THREADS 4

struct node {
   int data;
   int fibdata; 
   struct node* next;
};

struct node* ptrs[NUM_THREADS] = {NULL};
int nodes_per_thread = N/NUM_THREADS + 1;

int fib(int n) {
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
	  return (x + y);
   }
}

void processwork(struct node* p) 
{ 
   int n;
   n = p->data;
   p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;
    int index = 0;   
 
    head = malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    ptrs[index] = p;
    index++;
    for (i=0; i< N; i++) {
       temp  =  malloc(sizeof(struct node));
       p->next = temp;
       p = temp;
       p->data = FS + i + 1;
       p->fibdata = i+1;
       if( ((i+1) % nodes_per_thread) == 0) {
           ptrs[index] = p;
           index++;
       }
    }
    p->next = NULL;
    return head;
}

int main(int argc, char *argv[]) {
     double start, end;
     struct node *p=NULL;
     struct node *temp=NULL;
     struct node *head=NULL;
     
	 printf("Process linked list\n");
     printf("  Each linked list node will be processed by function 'processwork()'\n");
     printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);      
 
     p = init_list(p);
     head = p;
     omp_set_num_threads(NUM_THREADS);

     start = omp_get_wtime();
     {             
           
           #pragma omp parallel
           {
                #pragma omp single
                {
                    for(struct node* n=head; n; n=n->next) {
	                   #pragma omp task firstprivate(n)
                        processwork(n);
                    }
                }
           }
     }

     end = omp_get_wtime();
     
     // cleans up linked list
     p = head;
	 while (p != NULL) {
        printf("%d : %d\n",p->data, p->fibdata);
        temp = p->next;
        free (p);
        p = temp;
     }  
	 free (p);

     printf("Compute Time: %f seconds\n", end - start);

     return 0;
}


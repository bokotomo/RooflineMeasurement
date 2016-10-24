#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

#define NUM 100
#define NUM2 10000
#define HOTNUM 1000
#define SHOW 1
#define ROOP 1
#define SAVE 1

double *A,*B;
unsigned long num = 0;
int threads_num;
long arg_n = NUM;

void func(){
        int i,j,k;
        
        #pragma omp parallel num_threads(threads_num)
        {
               
                #pragma omp for private(i)
                for(i=0;i<num;i++){
                        A[i] = A[i] * A[i];
                }
        }       
}

void mat(){
   int i,j,k;
   
#pragma omp parallel
{
   
   #pragma omp for private(k,j)
   for(i=0;i<arg_n;i++) {
      for(k=0;k<arg_n;k++) {
         for(j=0;j<arg_n;j++) {
 //           C[i*arg_n + j] += A[i*arg_n + k] * B[k*arg_n + j];
         }
      }
   }

}

      
}

void jakobi(){
        long i,j,k;
        double l,r,t,b;
        
        //１回目
        //Aは最新、Bは一つ前
        
        //現在の状態保存
        for(i=0;i<NUM*NUM;i++){
                B[i] = A[i];
        }
        
        //現在の状態保存
        for(i=0;i<NUM*NUM;i++){
                l = (i%NUM != 0) ? B[i - 1] : 0;
                r = ((i+1)%NUM != 0) ? B[i + 1] : 0;
                t = (i > NUM) ? B[i - NUM] : 0;
                b = (i < NUM*NUM) ? B[i + NUM] : 0;
                
                A[i] = (l+ r + t + b)/4;
        }
}

void jakobi_p(){
        long i,j,k;
        double l,r,t,b;
        
        //１回目
        //Aは最新、Bは一つ前
        for(j=0;j<1;j++){
        //現在の状態保存
        for(i=0;i<NUM*NUM;i++){
                B[i] = A[i];
        }
        
        //現在の状態保存
        #pragma omp parallel num_threads(threads_num)
        {
                #pragma omp for private(i,l,r,t,b)
                for(i=0;i<NUM*NUM;i++){
                        l = (i%NUM != 0) ? B[i - 1] : 0;
                        r = ((i+1)%NUM != 0) ? B[i + 1] : 0;
                        t = (i >= NUM) ? B[i - NUM] : 0;
                        b = (i <= NUM*NUM) ? B[i + NUM] : 0;
                        
                        A[i] = (l+ r + t + b);
                }
        }
        
        }
}


//行列表示
void show_mat(){
        if(SHOW==1){
        long i=0;
        for(i=0;i<NUM*NUM;i++){
                printf("[%lf]",A[i]);
                if((i+1)%NUM==0)puts("");
        }
        }
}

int main(int argc, char *argv[]){
        unsigned long i,j,k;
        int res;
        double err;
        double st,et,rt,rt_sum,rt_ave;
        char msg[100],csv_path[100];
        FILE *fp;
        double flops;
      
        if(argc > 3 ){
                puts("引数が多すぎます");
                for (i = 0; i < argc; i++) {
                        printf("%d番目 : %s\n", i, argv[i]);
                }
                exit(1);
        }else if(argc==1){
                puts("引数が足りません");
                exit(1);
        }
        
        #pragma omp parallel
        {
                #pragma omp single
                {
                        if(argc == 3 ){
                                threads_num = atoi(argv[2]);
                        }else{
                                threads_num = omp_get_num_threads();
                        }
                }
        }
	
        omp_set_num_threads(threads_num);
        
        #pragma omp parallel num_threads(threads_num)
        {
                #pragma omp single
                {
                        printf("n/max = %d/%d\n", omp_get_num_threads(),omp_get_max_threads() );
                }
        }
   
        if(SAVE == 1){
        sprintf(csv_path,"./csv1025/numa_%d_%s.csv",threads_num,argv[1]);
    
        if ((fp = fopen(csv_path, "w")) == NULL) {
                printf("file open error!!\n");
                exit(EXIT_FAILURE);
        }else{
                printf("file path : ");
                printf(csv_path);
                puts("");
        }
        
        
        sprintf(msg,",,time(Sec)\n");

        printf(msg);
        fputs(msg, fp);
         
        }
         
         
        num = ( HOTNUM*1000*1000 ) / sizeof(double);
        
        for(i=0;i<10;i++){
                
                A = (double *)malloc(sizeof(double) * num);
                //B = (double *)malloc(sizeof(double) * num);
                
                //初期値代入
                #pragma omp parallel
                {
                        #pragma omp for private(j)
                        for(j=0;j<num/2;j+=2){
                                A[j] = rand()%10;
                                A[j+1] = rand()%10;
                               // B[j] = 0;
                               // B[j+1] = 0;
                        }
                }
                func();
                num += 128*1000;
                free(A);
                //free(B);
        }
        puts("end hotstart");
        
        num = ( (long)NUM2*1000*1000 ) / sizeof(double);
        
        printf("size is %luMB\n",sizeof(double)*num/(1000*1000));
        
        for(i=0;i<ROOP;i++){
         for(j=0;j<3;j++){
                
                A = (double *)malloc(sizeof(double) * num);
                //B = (double *)malloc(sizeof(double) * num);
                
                printf("%d MB\n",sizeof(double) * num/(1000*1000));
                
                //初期値代入
                #pragma omp parallel
                {
                        #pragma omp for private(j)
                        for(k=0;k<num/2;k+=2){
                                A[k] = rand()%10;
                                A[k+1] = rand()%10;
                                //B[k] = 0;
                                //B[k+1] = 0;
                        }
                }
                
                
                st = omp_get_wtime();
                func();
                et = omp_get_wtime();
                rt = (double)(et - st);
                printf("%f s\n",rt);
                
                rt_sum += rt;
        	
                free(A);
                //free(B);
        }

      rt_ave = rt_sum/3;
      flops = ( (num/rt_ave) ) / (1000*1000*1000) * 2;
      sprintf(msg,"%d,%f,%f\n",(int)sizeof(double)*num/(1000*1000),flops,rt_ave);
      printf(msg);
      if(SAVE == 1)fputs(msg, fp);
        
	         	if(ROOP - 1 == i && j==2){
			         res = system("numactl --hardware");
			         printf("%d/n",res);
               }


               rt_sum=0;
               num += 128*1000*4;
        } 



        res = system("numactl --show");
        printf("%d/n",res);

        res = system("numastat");
        printf("%d/n",res);
        
        
        if(SAVE==1)fclose(fp);
        
        
        return 0;
}

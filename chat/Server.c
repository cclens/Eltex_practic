#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mqueue.h>
#include <pthread.h>

struct mq_attr attr;
mqd_t mq_in;
mqd_t mq_out;
mqd_t mq_adm_in;
mqd_t mq_adm_out;
mqd_t mq_list_in;
mqd_t mq_list_out;

pthread_t thread_main;
pthread_t thread_count;
pthread_t thread_list;


char buffer[100];
int count = 0;
char name_list[100][100];

/*
void Track_user(){
        for (int i = 0; i <= count; i++){
        for (int j = 0; j <= count; j++){
                printf("%s\n", name_list[i]);
                mq_send(mq_list_in, name_list[i], strlen(name_list[i])+1, 0);
        }
        }
}
*/
void* User_count(){
    while(1){
                mq_receive(mq_adm_in, buffer, sizeof(buffer), NULL);
                        strcpy(name_list[count], buffer);
        //              printf("%d\n", count);

                        for (int i = 0; i < count; i++){
                mq_send(mq_adm_out, buffer, strlen(buffer)+1, 0);
                        } 

            for (int i = 0; i <= count; i++){
                    for (int j = 0; j <= count; j++){
                            printf("%s\n", name_list[i]);
                            mq_send(mq_list_in, name_list[i], strlen(name_list[i])+1, 0);
                    }
            }

                        if (strcmp(buffer, "exit\0") == 0){
                        count -= 1;
                        printf("%d\n", count);
                }
            else{
                count += 1;
            }
    }
}

void* Join_track(){
    while (1){
        mq_receive(mq_in, buffer, sizeof(buffer), NULL);
        printf("Received message from client: %s\n", buffer);
        for (int i = 0; i < count; i++){
            mq_send(mq_out, buffer, strlen(buffer)+1, 0);
        }
    }
}
 
int main(){
    attr.mq_msgsize = 100;
    attr.mq_curmsgs = 0;
    attr.mq_flags; 
    attr.mq_maxmsg = 10;

    mq_in = mq_open("/flag.txt1", O_CREAT|O_RDWR, 0666, &attr);
    mq_out = mq_open("/flag.txt2", O_CREAT|O_RDWR, 0666, &attr);
    mq_adm_in = mq_open("/flag.txt3", O_CREAT|O_RDWR, 0666, &attr);
    mq_adm_out = mq_open("/flag.txt4", O_CREAT|O_RDWR, 0666, &attr);
        mq_list_in = mq_open("/flag.txt5", O_CREAT|O_RDWR, 0666, &attr);

    pthread_create(&thread_main, NULL, Join_track, NULL);
    pthread_create(&thread_count, NULL, User_count, NULL);


    pthread_join(thread_main, NULL);
    pthread_join(thread_count, NULL);

    mq_close(mq_in);
    mq_close(mq_out);
    mq_close(mq_adm_in);
    mq_close(mq_adm_out);
        mq_close(mq_list_in);
        mq_close(mq_list_out);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ucontext.h>
#include <pthread.h>

//socket programming headers
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 8080

//you write the code here

struct psu_thread_info{
    int diff;
    ucontext_t uctx_data;
    char stack_data[16384];

};

typedef void (* convert_t)(void);

typedef struct psu_thread_info psu_thread_info_t;

//global variables
static ucontext_t uctx_main, uctx_create, uctx_foo, uctx_recv;
#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)

char create_stack[16384];
char foo_stack[16384];
int gl_mode;

//client side: 
int sock_cl;
struct sockaddr_in serv_addr_cl; // server address which client will ping
pthread_t tid;
void *exit_status;

//server side:
int sock_sv, new_socket, val_read;
struct sockaddr_in serv_addr_sv; // server address which server can accept
int opt;
int addrlen;

void psu_thread_setup_init(int mode)
{
	//Read from a file to set up the socket connection between the client and the server
	gl_mode = mode;    
    	//setting up foo for context switch
    	if(getcontext(&uctx_foo) == -1){handle_error("getcontext for func foo failed! ");}
    	uctx_foo.uc_stack.ss_sp = foo_stack;
    	uctx_foo.uc_stack.ss_size = sizeof(foo_stack);
    	uctx_foo.uc_link = &uctx_create;
	if(mode == 0)
	{
		//client part: 
		//1. setup socket
		//2. put the address of the server
		//

		//printf("I am client!\n");
		if((sock_cl = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		    printf("*Clinet*: Error in creating socket! \n");
		    //return 0;
		}

		serv_addr_cl.sin_family = AF_INET;
		serv_addr_cl.sin_port = htons(PORT);

		//convert address into right format to store in serv_addr_cl
		if(inet_pton(AF_INET, "127.0.0.1", &serv_addr_cl.sin_addr) <=0){
		    printf("*Client*: storing address - Address is invalid\n");
		    //return 0;
		}

		printf("Successfully did client side socket init! \n");


    	}
    	else
    	{
    		printf("I am server!\n");
		opt = 1;
		// Create the socket file descriptor
		if((sock_sv = socket(AF_INET, SOCK_STREAM, 0)) == 0){
		    perror("Server: socket failed!\n");
		    exit(EXIT_FAILURE); // this will print the reason for the failure in creating the socket
		}

		//Forcefully attaching socket to the port 8080
		if (setsockopt(sock_sv, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
		{
		    perror("Server: setsockopt\n"); // failed to attach to 8080
		    exit(EXIT_FAILURE);// this is a #define or pragma
		}

		serv_addr_sv.sin_family = AF_INET; // address family, same as PF_INET
		serv_addr_sv.sin_addr.s_addr = INADDR_ANY; // client could be at any address
		serv_addr_sv.sin_port = htons(PORT);

		addrlen = sizeof(serv_addr_sv);

		//Forcefully attaching socket to the port 8080
		if (bind(sock_sv, (struct sockaddr *)&serv_addr_sv, sizeof(serv_addr_sv)) < 0)
		{
		    perror("Server: bind failed!\n");
		    exit(EXIT_FAILURE);
		}
    	}	
	return;
}

int psu_thread_create(void* (*user_func)(void*), void *user_args)
{
	// make thread related setup
	// create thread and start running the function based on *user_func
	makecontext(&uctx_foo, (convert_t)user_func, 0);
	//if(gl_mode == 0)
	//{
        //client part
        //1. create a thread, it goes to thread function. while, parent thread? may wait or exit. 
		//pthread_create(&tid, NULL, user_func, user_args);
		//pthread_join(tid, &exit_status);
		//printf("Client: completed going to foo! \n");
    	//}
	return 0; 
}

void psu_thread_migrate(const char *hostname)
{
	//thread Migration related code
	return;
}

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <poll.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#define PORT 8012


struct http{
	char *method;
	char *path;
	char *protocol;
};


//sets the fd so that it can be listened on correctly

int set_up_listen(int fd, struct addrinfo *res){
	struct addrinfo *temp;
	for(temp = res; temp != NULL; temp = temp->ai_next){

		fd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
		if( fd == -1){
			perror("socket failed\n");
		}else {
			printf("\nsocket\n");

			int option = 1;
			int setsocketopt = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
			if(setsocketopt == -1){
				perror("setsocketopt failed\n");
			}else{
				printf("setsocketopt\n");
			}
			

			int en  = 1;
			if(ioctl(fd, FIONBIO, &en) == -1){
				perror("ioctl failed\n");			
			}else{
				printf("ioctl\n");
			}	
			
			//bind params file descriptor, address to bind to, length of address
			if(bind(fd, temp->ai_addr, temp->ai_addrlen) == -1){
				perror("bind failed\n");			
			}else{
				printf("bind\n");
			}
		
			return fd;
		
		}
		
		
	}
}

//sets up listem

int  listen_client(int fd){
	
	
	if(listen(fd, SOMAXCONN) ==-1){
		perror("listen failed\n");
		exit(-1);
	}else{
		printf("listen\n");
	}
	printf("leaving listen");
	return fd;
}


//accepts and retunes new fd 

int accept_client(int fd){
	int new_fd = -1;
	while(1){
		int new_fd = accept(fd, NULL, NULL);
		if(new_fd !=-1){	
			printf("accept\n");
			return new_fd;
			break;
		}	
	}
	return new_fd;
}

//recieves one chracter at a time from recv(), stopping once recv returns -1 or 0
//returns total string on fd (final_buff)


char* recieve(int new_fd){

	//recive one character at a time 

	char buffer[1];
	char *final_buff = malloc(10000);
	

	int total =0;
	while(1){
		int len = recv(new_fd, &buffer, 1, 0);
		
		if(len==-1){
			if(errno == EWOULDBLOCK){
				final_buff[total] = '\0';
				 break;
			}else{
				 perror("recv failed line 125\n");
				 break;
			}
		
		}

		final_buff[total] = buffer[0];
		
		if(total > 3 && final_buff[total] == '\n' && final_buff[total-1] == '\r' && final_buff[total-2] == '\n' && final_buff[total-3] == '\r'){
			final_buff[total] = '\0';
			break;					
				
		}
		total++;


		
	}
	
	return final_buff;

	
	
}

//helper function to get the content-type of file to send to client

char* get_Type(char *final_buff){
	char *type = malloc(64);
	if(strstr(final_buff, ".html")){
		strcpy(type, "text/html");
	} else if(strstr(final_buff, ".css")){
		strcpy(type, "text/css");
	} else if(strstr(final_buff, ".png")){
		strcpy(type, "image/png");
	} else if(strstr(final_buff, ".ico")){
		strcpy(type, "image/x-icon");
	} else if(strstr(final_buff, ".xml")){
		strcpy(type, "text/xml");
	} else if(strstr(final_buff, ".jgp")){
		strcpy(type, "image/jpg");
	} else {
		strcpy(type, "text/plain");	
	}
	return type;


}

//method to create the response based on information in that was recv()

void contstructResponse(char *final_buff, int fd){
	
	printf("final_buff \n  %s\n", final_buff);
	char www[4] = "www";
	
	//parse the different parts for the GET request
	char *method = strtok(final_buff, " ");
	char *uri = strtok(NULL, " ");
	char *version = strtok(NULL, "\r");
	printf("uri %s\n", uri);

	char *type = get_Type(uri);
	//case where root is path in request so send index.html
	if(strcmp(uri, "/") == 0){
		strcpy(uri, "/index.html");
		strcpy(type, "text/html");
	}


	strcat(www, uri);
	printf("updated uri %s\n", www);
	//use uri to find file to open
	FILE *f = fopen(www, "r");

	//if file was not found send 404 error
	if(f == NULL){
		char *response;
		printf("failed to find file\n");
		snprintf(response, 512, "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: 0\n\n");
		send(fd, response, strlen(response), 0);


	//else send correct information to client
	} else{
	
		printf("found file\n");
		fseek(f, 0L, SEEK_END);
		long size = ftell(f);
		rewind(f);
		char file_buff[size +1];
		int len = fread(file_buff, 1, size, f);
		file_buff[len] = '\0';
		fclose(f);
	
		char *response = malloc(size + 512);
		snprintf(response, 512 + size, "HTTP/1.1 200 OK\nContent-Type: %s\nContent-Length: %ld\n\n%s", type, size, file_buff);
		send(fd, response, strlen(response), 0);

		

	}

	//free malloced char *s

	free(final_buff);
	free(type);
	
	
	

	
}

//poll fucntion to take in requests from the client


int poll_for_fd(int lfd){

	
	struct pollfd p[32];
	
	//make listener fd first active on in array
	p[0].fd = lfd;
	p[0].events = POLLIN;
	int len =1;
	int num;
	printf("in poll\n");
	if(fcntl(lfd, F_GETFL) ==-1){
		perror("line187 bad listener\n");
	}
	while(1){
		num = poll(p, len, 100);
		if(num == -1){
			perror("poll failed\n");
		} else if(num > 0){
			printf("poll\n");
		}
		//nothing to do in this loop if no fds are active 
		if (num ==0){
			continue;	
		}

		//iterate through all fds to see if we can accept a worker or recv()
		for(int i =0; i < 32; i++){
			//printf("in for lopp i is:%d", i);
			if(p[i].revents & POLLIN){
				
				printf("have a revents and pollin %d\n", p[i].fd);

				//if the active fd is the listner already declared we can accept
				if(p[i].fd == p[0].fd){
					int worker = accept(lfd, NULL, NULL);
					if(worker ==-1){
						perror("accept failed\n");
					}else{
						printf("accept\n");
					}
					p[len].fd = worker;
					p[len].events = POLLIN;
					len++;
					printf("gained a worker from listener\n");
				//else it means the active fd can be recv()
				}else{
					printf("ready to receive\n");
					char *req = recieve(p[i].fd);
					contstructResponse(req, p[i].fd);
			
				}	
			}
			
			
		}

	}


	
}

int main(int argc, char **argv){


	struct addrinfo hints, *res;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	hints.ai_protocol = IPPROTO_TCP;
	
	
	getaddrinfo(NULL, "8012", &hints, &res);
		
	int fd = -1;
	fd = set_up_listen(fd, res);
	//printf("before listen");
	fd = listen_client(fd);
	printf("about to enter poll\n");
	poll_for_fd(fd);



	close(fd);
	exit(0);

	
}

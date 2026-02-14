#include <stdio.h> 
#include <string.h> 
#include <unistd.h> 
#include <arpa/inet.h> 
#define PORT 5353 
struct DNS { 
char domain[50]; 
char ip[20]; 
}; 
struct DNS records[] = { 
{"google.com",   "142.250.183.142"}, 
{"youtube.com",  "142.250.182.206"}, 
{"facebook.com", "157.240.22.35"}, 
{"example.com",  "93.184.216.34"}, 
{"test.com",    
 "192.168.1.50"}, 
{"myserver.local", "10.0.0.55"} 
}; 
int main() { 
int server_fd, client_fd; 
struct sockaddr_in server, client; 
socklen_t cli_len = sizeof(client); 
char input[100], output[100]; 
server_fd = socket(AF_INET, SOCK_STREAM, 0); 
server.sin_family = AF_INET; 
server.sin_port = htons(PORT); 
server.sin_addr.s_addr = INADDR_ANY; 
bind(server_fd, (struct sockaddr*)&server, sizeof(server)); 
listen(server_fd, 5); 
printf("DNS Server started on port %d...\n", PORT); 
while (1) { 
client_fd = accept(server_fd, (struct sockaddr*)&client, &cli_len); 
memset(input, 0, sizeof(input)); 
read(client_fd, input, sizeof(input)); 
input[strcspn(input, "\r\n")] = 0;  
printf("Client Query: %s\n", input); 
strcpy(output, "Not found"); 
if (strchr(input, '.')) { 
for (int i = 0; i < 6; i++) { 
                if (strcmp(input, records[i].ip) == 0) { 
                    strcpy(output, records[i].domain); 
                    break; 
                } 
            } 
        } 
        for (int i = 0; i < 6; i++) { 
            if (strcmp(input, records[i].domain) == 0) { 
                strcpy(output, records[i].ip); 
                break; 
            } 
        } 
 
        write(client_fd, output, strlen(output)); 
        close(client_fd); 
    } 
 
    return 0; 
}
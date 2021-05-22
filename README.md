# soal-shift-sisop-modul-3-A03-2021

Anggota : 

### 1. Naufal Fajar  I  05111940000007
### 2. Johnivan Aldo S  05111940000051 
### 3. Abdun Nafi'      05111940000066

## Soal 1
### Penyelesaian ###
Untuk mengerjakan soal ini, maka diperlukan program untuk server dan client, serta banyak tambahan fungsi - fungsi lain yang harus dimasukkan untuk menjalankan program dengan semestinya. 
Berikut adalah sourcecode untuk bagian **server.c**

```

#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>
#define PORT 8080
#define SO_REUSEPORT 15

int n;

struct user {
    char name[50];
    char pwd[50];
};

struct file {
    char name[50];
    char path[50];
    char publisher[50];
    char ext[20];
    char year[5];
};

char *file_ext(char *file) {
    char *p = strchr(file, '.');
    if (p == NULL) return "exe";
    if (p - file == 6) {
        p = strchr(p+1, '.');
    }

    char *ext;
    ext = (char *)malloc(sizeof(char*) * 50);
    sprintf(ext, "%.*s", strlen(file) - (p - file + 1), p + 1);
    for (size_t i = 0; i < strlen(ext); ++i) {
        *(ext + i) = tolower(*(ext + i));
    }
    return ext;
}

char *see(struct file files[], int n) {
    char *buf;
    buf = (char *)malloc(sizeof(char) * 1024);
    memset(buf, 0, sizeof(buf));

    for (int i = 0; i < n; ++i) {
        sprintf(files[i].ext, "%s", file_ext(files[i].path));
        char *p = strstr(files[i].path, files[i].ext);
        if (p == NULL) {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        } else {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        }
        
        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, files[i].year, files[i].ext, files[i].path);
    }

    return buf;
}

int find_user(char *name, char *pwd) {
    int n = 0, mode = 0;
    struct user users[50];

    for (int i = 0; i < 50; ++i) {
        memset(users[i].name, 0, sizeof(users[i].name));
        memset(users[i].pwd, 0, sizeof(users[i].pwd));
    }

    FILE *fptr;
    fptr = fopen("akun.txt", "r");
    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == ':') {
            mode ^= 1;
            continue;
        }
        if (str[i] == '\n' || str[i] == ' ') {
            mode ^= 1;
            ++n;
            continue;
        }

        if (mode == 0) {
            sprintf(users[n].name, "%s%c", users[n].name, str[i]);
        } else {
            sprintf(users[n].pwd, "%s%c", users[n].pwd, str[i]);
        }
    }

    fclose(fptr);

    for (int i = 0; i < n; ++i) {
            if (strcmp(users[i].name, name) == 0 && strcmp(users[i].pwd, pwd) == 0) {
            return 1;
        }
    }
    
    return 0;
}

void reg(char *name, char *pwd) {
    FILE *fptr;

    fptr = fopen("akun.txt", "a");

    fprintf(fptr, "%s:%s\n", name, pwd);
    
    fclose(fptr);
}

struct file *parse_files() {
    n = 0;
    int mode = 0;

    struct file *files;
    files = (struct file *)malloc(sizeof(struct file) * 50);

    for (int i = 0; i < 5; ++i) {
        memset(files[i].ext, 0, sizeof(files[i].ext));
        memset(files[i].name, 0, sizeof(files[i].name));
        memset(files[i].path, 0, sizeof(files[i].path));
        memset(files[i].publisher, 0, sizeof(files[i].publisher));
        memset(files[i].year, 0, sizeof(files[i].year));
    }

    FILE *fptr;
    fptr = fopen("files.tsv", "r");

    fseek(fptr, 0, SEEK_END); 
    long fsize = ftell(fptr); 
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == '\t') {
            mode += 1;
            continue;
        }
        if (str[i] == '\n') {
            mode = 0;
            ++n;
            continue;
        }

        if (mode == 0) {
            sprintf(files[n].path, "%s%c", files[n].path, str[i]);
        } else if (mode == 1) {
            sprintf(files[n].publisher, "%s%c", files[n].publisher, str[i]);
        } else {
            sprintf(files[n].year, "%s%c", files[n].year, str[i]);
        }
    }

    for (int i = 0; i < n; ++i) {
        sprintf(files[i].ext, "%s", file_ext(files[i].path));
        char *p = strstr(files[i].path, files[i].ext);
        if (p == NULL) {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        } else {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        }
    }

    fclose(fptr);

    return files;
}

void delete(char *file) {
    struct file *files = parse_files();
    struct file *new_files = (struct file *)malloc(sizeof(struct file) * 50);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        if (strcmp(files[i].name, file) == 0) {
            continue;
        }

        sprintf(new_files[j].ext, "%s", files[i].ext);
        sprintf(new_files[j].name, "%s", files[i].name);
        sprintf(new_files[j].path, "%s", files[i].path);
        sprintf(new_files[j].publisher, "%s", files[i].publisher);
        sprintf(new_files[j].year, "%s", files[i].year);

        ++j;
    }

    if (j == n) return;

    char old_name[50];
    sprintf(old_name, "FILES/%s", file);

    char new_name[50];
    sprintf(new_name, "FILES/old-%s", file);

    if (rename(old_name, new_name) < 0) {
        printf("error renaming %s to %s\n", old_name, new_name);
    }

    FILE *fptr;
    
    fptr = fopen("files.tsv", "w");

    for (int i = 0; i < j; ++i) {
        fprintf(fptr, "%s\t%s\t%s\n", new_files[i].path, new_files[i].publisher, new_files[i].year);
    }

    fclose(fptr);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    char *login_or_register = "login/register: ";
    char *id = "id: ";
    char *pwd = "password: ";
    char *login_success = "login success";
    char *login_fail = "login failed";
    char *register_success = "register success";
    char *file_pattern_not_found = "file pattern doesn't match";

    char buffer[1024] = {0};
    char user_id[1024];
    char user_pwd[1024];
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    
    mkdir("FILES", 0755);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        while (1) {
            send(new_socket, login_or_register, strlen(login_or_register), 0);
            read(new_socket, buffer, 1024);
            printf("user %s\n", buffer);

            memset(user_id, 0, sizeof(user_id));
            memset(user_pwd, 0, sizeof(user_pwd));

            if (strcmp(buffer, "register") == 0) {
                send(new_socket, id, strlen(id), 0);
                read(new_socket, user_id, 1024);
                printf("id: %s\n", user_id);
                
                send(new_socket, pwd, strlen(pwd), 0);
                read(new_socket, user_pwd, 1024);
                printf("password: %s\n", user_pwd);

                reg(user_id, user_pwd);
                send(new_socket, register_success, strlen(register_success), 0);
            } else if (strcmp(buffer, "login") == 0) {
                
                send(new_socket, id, strlen(id), 0);
                read(new_socket, user_id, 1024);
                printf("id: %s\n", user_id);
                
                send(new_socket, pwd, strlen(pwd), 0);
                read(new_socket, user_pwd, 1024);
                printf("password: %s\n", user_pwd);

                if (find_user(user_id, user_pwd)) {
                    send(new_socket, login_success, strlen(login_success), 0);
                    printf("%s\n", login_success);
                    break;
                } else {
                    send(new_socket, login_fail, strlen(login_fail), 0);
                    printf("%s\n", login_fail);
                }
            }

            memset(buffer, 0, sizeof(buffer));
            sleep(2);
        }
        printf("\n");

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);

            if (strncmp(buffer, "add", 3) == 0) {
                printf("user add\n");
                char publisher[1024];
                char year[1024];
                char path[1024];
                char filepath[1024];

                read(new_socket, publisher, 1024);
                printf("Publisher: %s\n", publisher);

                read(new_socket, year, 1024);
                printf("Tahun Publikasi: %s\n", year);

                read(new_socket, path, 1024);
                printf("Filepath: %s\n", path);


                sprintf(filepath, "FILES/%s", path);
                FILE *fptr;

                fptr = fopen("files.tsv", "a");
                fprintf(fptr, "%s\t%s\t%s\n", filepath, publisher, year);
                fclose(fptr);

                fptr = fopen("running.log", "a");
                fprintf(fptr, "Tambah : %s (%s:%s)\n", filepath, user_id, user_pwd);
                fclose(fptr);

                char file_content[65535];
                char file_length[1024];
                unsigned long fsize;

                memset(file_content, 0, sizeof(file_content));
                memset(file_length, 0, sizeof(file_length));

                read(new_socket, file_length, 1024);
                fsize = strtol(file_length, NULL, 0);

                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    read(new_socket, buffer, 1024);
                    strcat(file_content, buffer);
                }

                fptr = fopen(filepath, "w");
                fprintf(fptr, "%s", file_content);
                fclose(fptr);
            } else if (strncmp(buffer, "download", 8) == 0) {
                printf("user download\n");

                char file_to_download[50];
                sprintf(file_to_download, "%.*s", strlen(buffer) - 9, buffer + 9);

                char *file_exist = "found";
                char *file_not_exist = "not found";

                int found = 0;
                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) {
                    if (strcmp(files[i].name, file_to_download) == 0) {
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    send(new_socket, file_exist, sizeof(file_exist), 0);
                } else {
                    send(new_socket, file_not_exist, sizeof(file_not_exist), 0);
                    continue;
                }

                sleep(1);

                char file_length[1024];
                char *file_content = (char *)malloc(sizeof(char) * 65536);
                char file_to_open[1024];

                memset(file_length, 0, sizeof(file_length));
                memset(file_to_open, 0, sizeof(file_to_open));
                
                sprintf(file_to_open, "FILES/%s", file_to_download);

                FILE *fptr;
                fptr = fopen(file_to_open, "r");

                fseek(fptr, 0, SEEK_END);
                long fsize = ftell(fptr);
                rewind(fptr);

                fread(file_content, 1, fsize, fptr);

                fclose(fptr);

                sprintf(file_length, "%ld", fsize);
                send(new_socket, file_length, sizeof(file_length), 0);
                sleep(1);

                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                    send(new_socket, buffer, sizeof(buffer), 0);
                }

            } else if (strncmp(buffer, "delete", 6) == 0) {
                printf("user delete\n");

                char file_to_delete[50];
                sprintf(file_to_delete, "%.*s", strlen(buffer) - 7, buffer + 7);
                delete(file_to_delete);

                FILE *fptr;
                fptr = fopen("running.log", "a");

                fprintf(fptr, "Hapus : %s (%s:%s)\n", file_to_delete, user_id, user_pwd);

                fclose(fptr);

                memset(file_to_delete, 0, sizeof(file_to_delete));
            } else if (strncmp(buffer, "see", 3) == 0) {
                printf("user see\n");

                struct file *files = parse_files();
                char *files_tsv = (char *)malloc(sizeof(char) * 1024);

                sprintf(files_tsv, "%s", see(files, n));
                send(new_socket, files_tsv, strlen(files_tsv), 0);
            } else if (strncmp(buffer, "find", 4) == 0) {
                printf("user find\n");

                char file_to_find[50];
                sprintf(file_to_find, "%.*s", strlen(buffer) - 5, buffer + 5);

                char *buf = (char *)malloc(sizeof(char) * 1024);
                memset(buf, 0, sizeof(buf));

                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) {
                    char *p;
                    p = strstr(files[i].name, file_to_find);
                    if (p) {
                        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, files[i].year, files[i].ext, files[i].path);
                    }
                }

                if (strlen(buf) == 0) {
                    send(new_socket, file_pattern_not_found, strlen(file_pattern_not_found), 0);
                } else {
                    send(new_socket, buf, strlen(buf), 0);
                }
            } else {
                break;
            }
            printf("\n");
        }

        close(new_socket);
    }

    return 0;
}

```

Sedangkan berikut ini adalah sourcecode untuk menyelesaikan bagian **client.c**

```
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;

    char input[1024];

    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    // login/register
    do {
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("login/register: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // id
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("id: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // password
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("password: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // success or fail
        read(sock, buffer, 1024);
        printf("%s\n", buffer);
    } while (strcmp(buffer, "login success") != 0);

    printf("\n");

    // logged in
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        printf("command: ");
        scanf("\n");
        scanf("%[^\n]%*c", input);
        if (strncmp(input, "add", 3) == 0) {
            send(sock, input, sizeof(input), 0);
            
            // publisher
            memset(input, 0, sizeof(input));
            printf("Publisher: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

            // year
            memset(input, 0, sizeof(input));
            printf("Tahun publikasi: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

            // filepath
            memset(input, 0, sizeof(input));
            printf("Filepath: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

            // send file
            char file_length[1024];
            char *file_content = (char *)malloc(sizeof(char) * 65536);

            memset(file_length, 0, sizeof(file_length));

            FILE *fptr;
            fptr = fopen(input, "r");

            fseek(fptr, 0, SEEK_END);
            long fsize = ftell(fptr);
            rewind(fptr);

            fread(file_content, 1, fsize, fptr);

            fclose(fptr);

            // send file size
            sprintf(file_length, "%ld", fsize);
            send(sock, file_length, sizeof(file_length), 0);
            sleep(1);

            // send file content
            for (long i = 0; i < fsize; i += 1024) {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                send(sock, buffer, sizeof(buffer), 0);
            }
        } else if (strncmp(input, "download", 8) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);

            if (strcmp(buffer, "found") == 0) {
                // receive file
                char file_content[65535];
                char file_length[1024];
                long fsize;
                char file_to_save[50];
                sprintf(file_to_save, "%.*s", strlen(input) - 9, input + 9);

                memset(file_content, 0, sizeof(file_content));
                memset(file_length, 0, sizeof(file_length));

                // receive file size
                read(sock, file_length, 1024);
                fsize = strtol(file_length, NULL, 0);

                // receive file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    read(sock, buffer, 1024);
                    strcat(file_content, buffer);
                }

                FILE *fptr;
                fptr = fopen(file_to_save, "w");

                fprintf(fptr, "%s", file_content);

                fclose(fptr);
            } else {
                printf("file not found\n");
            }
        } else if (strncmp(input, "delete", 6) == 0) {
            send(sock, input, sizeof(input), 0);
        } else if (strncmp(input, "see", 3) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
        } else if (strncmp(input, "find", 4) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
        } else {
            send(sock, input, sizeof(input), 0);
            break;
        }

        memset(input, 0, sizeof(input));
        printf("\n");
    }

    return 0;
}
```
## Penjelasan Code ##

### Subsoal 1a.
Setelah server dan client tersambung, akan ada dua pilihan yaitu register dan login. Jika memilih **register**, client akan diminta input **id dan passwordnya** untuk dikirimkan ke server. User juga dapat melakukan **login**. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama **akun.txt** 

### Pada server ###
- Mendefinisikan port server, kemudian membuat struct untuk user yang terdiri dari **id user dan password user**. Kemudian membuat struct untuk files yang berisi nama file dan path filenya sebagai berikut
```
//mendefinisikan port server
#define PORT 8080
#define SO_REUSEPORT 15

int n;

//membuat struct user yg terdiri dari nama user dan password
struct user {
    char name[50];
    char pwd[50];
};

```

- Membuat fungsi find_user dimana fungsi ini digunakan untuk mencari akun atau user yang sudah melakukan pendaftaran. Fungsi ini nantinya akan dijalankan saat user melakukan login.

```
// untuk mencari user yang ada
int find_user(char *name, char *pwd) {
    // printf("%s:%s %d:%d\n", name, pwd, strlen(name), strlen(pwd));
    int n = 0, mode = 0;

    struct user users[50];

    for (int i = 0; i < 50; ++i) {
        memset(users[i].name, 0, sizeof(users[i].name));
        memset(users[i].pwd, 0, sizeof(users[i].pwd));
    }

    FILE *fptr;
    //membuka dan membaca di file akun.txt
    fptr = fopen("akun.txt", "r");
    //untuk membaca file  mulai dari offset yang diinginkan
    fseek(fptr, 0, SEEK_END); // offset dipindah relatif terhadap offset terakhir file.
    long fsize = ftell(fptr);
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    //membaca kalimat dalam sebuah FILE yang sudah dibuka di file.tsv
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == ':') {
            mode ^= 1;
            continue;
        }
        if (str[i] == '\n' || str[i] == ' ') {
            mode ^= 1;
            ++n;
            continue;
        }

        if (mode == 0) {
            sprintf(users[n].name, "%s%c", users[n].name, str[i]);
        } else {
            sprintf(users[n].pwd, "%s%c", users[n].pwd, str[i]);
        }
    }

    fclose(fptr);

    for (int i = 0; i < n; ++i) {
        // printf("%s:%s %d:%d\n", users[i].name, users[i].pwd, strlen(users[i].name), strlen(users[i].pwd));
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].pwd, pwd) == 0) {
            return 1;
        }
    }
    
    return 0;
}
```

- Membuat fungsi register untuk membantu user mendaftarkan akun. Fungsi ini juga akan menuliskan atau mengedit daftar nama akun dan password yang akan terdaftar pada akun.txt

```
//fungsi register
void reg(char *name, char *pwd) {
    FILE *fptr;

    //membuat file dengan nama akun.txt
    fptr = fopen("akun.txt", "a");

    // isinya id sama password yg terdaftar
    fprintf(fptr, "%s:%s\n", name, pwd);
    
    fclose(fptr);
}
```

### Pada Client ###

- Mendefinisikan port terlebih dahulu
```
#define PORT 8080
```

- Jike pembuatan socket tidak berhasil tampilkan Socket creation error
```
if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
```

- Melakukan penyambungan dengan server jika gagal maka tampilkan Connection Failed
```
memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
  // jika gagal menyambungkan ke server manapun maka tampil "Connection Failed"
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
```

- Client akan menginput pilihan login dan register. Setelah itu client akan menginputkan id beserta passwordnya. Data-data ini nantinya akan dikirim ke server. Jika login berhasil maka client akan mendapatkan tampilan login success
```
// login/register
    do {
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("login/register: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // id
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("id: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // pwd
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("password: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // success or fail
        read(sock, buffer, 1024);
        printf("%s\n", buffer);
    } while (strcmp(buffer, "login success") != 0);

    printf("\n");
```

### Subsoal 1b
Sistem memiliki sebuah database yang bernama files.tsv. Isi dari files.tsv ini adalah path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan. 
### Pada server ###
```
//membuat struct file yang terdiri nama file dan path filenya
struct file {
    char name[50];
    char path[50];
    char publisher[50];
    char ext[20];
    char year[5];
};

//membuat struct files yang akan menjadi databasenya
struct file *parse_files() {
    n = 0;
    int mode = 0;

    struct file *files;
    files = (struct file *)malloc(sizeof(struct file) * 50);

    for (int i = 0; i < 5; ++i) {
        //data yang akan diisikan berupa nama file, path filenya, nama publisher dan tahun publishnya
        memset(files[i].ext, 0, sizeof(files[i].ext));
        memset(files[i].name, 0, sizeof(files[i].name));
        memset(files[i].path, 0, sizeof(files[i].path));
        memset(files[i].publisher, 0, sizeof(files[i].publisher));
        memset(files[i].year, 0, sizeof(files[i].year));
    }

    FILE *fptr;
    //membuka file.tsv untuk menambahkan data di atas
    fptr = fopen("files.tsv", "r");

    //untuk membaca file  mulai dari offset yang diinginkan
    fseek(fptr, 0, SEEK_END); // offset dipindah relatif terhadap offset terakhir file.
    long fsize = ftell(fptr); // mengembalikan nilai saat ini dari posisi identifier.
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    //membaca kalimat dalam sebuah FILE yang sudah dibuka di file.tsv
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == '\t') {
            mode += 1;
            continue;
        }
        if (str[i] == '\n') {
            mode = 0;
            ++n;
            continue;
        }

        if (mode == 0) {
            sprintf(files[n].path, "%s%c", files[n].path, str[i]);
        } else if (mode == 1) {
            sprintf(files[n].publisher, "%s%c", files[n].publisher, str[i]);
        } else {
            sprintf(files[n].year, "%s%c", files[n].year, str[i]);
        }
    }

    for (int i = 0; i < n; ++i) {
        sprintf(files[i].ext, "%s", file_ext(files[i].path));
        char *p = strstr(files[i].path, files[i].ext);
        if (p == NULL) {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        } else {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        }
    }

    fclose(fptr);

    return files;
}
```

### Subsoal 1C
Client akan mengirmkan command **add** dan memasukkan beberapa data seperti publisher, tahun publikasi, dan filepath. Kemudian dari aplikasi client akan dimasukan data buku tersebut (perlu diingat bahwa Filepath ini merupakan path file yang akan dikirim ke server). Lalu client nanti akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan.

### Pada server
```
while (1) {
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);

// Menggunakan fungsi strncmp untuk membandingkan input user dengan add//
            if (strncmp(buffer, "add", 3) == 0) {
                printf("user add\n");
// Membuat karakter sessuai dengan input yang dibutuhkan                
		char publisher[1024];
                char year[1024];
                char path[1024];
                char filepath[1024];
//Menampilkan pada user untuk mengisi hal-hal yang diperlukan//
                read(new_socket, publisher, 1024);
                printf("Publisher: %s\n", publisher);

                read(new_socket, year, 1024);
                printf("Tahun Publikasi: %s\n", year);

                read(new_socket, path, 1024);
                printf("Filepath: %s\n", path);


                sprintf(filepath, "FILES/%s", path);
// Mengisi di files.tsv//
                FILE *fptr;

                fptr = fopen("files.tsv", "a");
                fprintf(fptr, "%s\t%s\t%s\n", filepath, publisher, year);
                fclose(fptr);

//Mengisi running log//
                fptr = fopen("running.log", "a");
                fprintf(fptr, "Tambah : %s (%s:%s)\n", filepath, user_id, user_pwd);
                fclose(fptr);

// receive file
                char file_content[65535];
                char file_length[1024];
                unsigned long fsize;

                memset(file_content, 0, sizeof(file_content));
                memset(file_length, 0, sizeof(file_length));

// receive file size
                read(new_socket, file_length, 1024);
                fsize = strtol(file_length, NULL, 0);

// receive file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    read(new_socket, buffer, 1024);
                    strcat(file_content, buffer);
                }

                // printf("%s\n", file_content);

                fptr = fopen(filepath, "w");
                fprintf(fptr, "%s", file_content);
                fclose(fptr);
```

### Pada client ###
```
//Memastikan input user adalah add menggunkana fungsi strncmp
if (strncmp(input, "add", 3) == 0) {
            send(sock, input, sizeof(input), 0);
            
// Memasukkan input publisher
            memset(input, 0, sizeof(input));
            printf("Publisher: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

// Memasukkan input year
            memset(input, 0, sizeof(input));
            printf("Tahun publikasi: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

// Memasukkan input filepath
            memset(input, 0, sizeof(input));
            printf("Filepath: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

// send file ke server
            char file_length[1024];
            char *file_content = (char *)malloc(sizeof(char) * 65536);

            memset(file_length, 0, sizeof(file_length));

            FILE *fptr;
            fptr = fopen(input, "r");

            fseek(fptr, 0, SEEK_END);
            long fsize = ftell(fptr);
            rewind(fptr);

            fread(file_content, 1, fsize, fptr);

            fclose(fptr);

// send file size
            sprintf(file_length, "%ld", fsize);
            send(sock, file_length, sizeof(file_length), 0);
            sleep(1);

// send file content
            for (long i = 0; i < fsize; i += 1024) {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                send(sock, buffer, sizeof(buffer), 0);
            }
```

### Subsoal 1d
Client dapat **mendownload** file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di folder client tersebut. 

## Pada server
```
// Memastikan bahwa input user adalah download
else if (strncmp(buffer, "download", 8) == 0) {
                printf("user download\n");

                char file_to_download[50];
                sprintf(file_to_download, "%.*s", strlen(buffer) - 9, buffer + 9);

// Memasukkan file status
                char *file_exist = "found";
                char *file_not_exist = "not found";


                int found = 0; 		//setting variabel found dengan 0 terlebih dahulu
                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) { 
// Apabila nama file yang dicari user sama dengan file yang akan didownload (ada di server)
		    if (strcmp(files[i].name, file_to_download) == 0) { 	
                        found = 1; 	//Ubah variabel found menjadi 1//
                        break;
                    }
                }

                if (found) {
                    send(new_socket, file_exist, sizeof(file_exist), 0);
                } else {
                    send(new_socket, file_not_exist, sizeof(file_not_exist), 0);
                    continue;
                }

                sleep(1);
// send file
                char file_length[1024];
                char *file_content = (char *)malloc(sizeof(char) * 65536);
                char file_to_open[1024];

                memset(file_length, 0, sizeof(file_length));
                memset(file_to_open, 0, sizeof(file_to_open));
                
                sprintf(file_to_open, "FILES/%s", file_to_download);

                FILE *fptr;
                fptr = fopen(file_to_open, "r");

                fseek(fptr, 0, SEEK_END);
                long fsize = ftell(fptr);
                rewind(fptr);

                fread(file_content, 1, fsize, fptr);

                fclose(fptr);

// send file size
                sprintf(file_length, "%ld", fsize);
                send(new_socket, file_length, sizeof(file_length), 0);
                sleep(1);

// send file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                    send(new_socket, buffer, sizeof(buffer), 0);
                }
```

### Pada client
```
// Mengecek apakah inputan user adalah download
else if (strncmp(buffer, "download", 8) == 0) {
                printf("user download\n");

                char file_to_download[50];
                sprintf(file_to_download, "%.*s", strlen(buffer) - 9, buffer + 9);

 // file status
                char *file_exist = "found";
                char *file_not_exist = "not found";

// find file
                int found = 0;
                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) {
                    if (strcmp(files[i].name, file_to_download) == 0) {
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    send(new_socket, file_exist, sizeof(file_exist), 0);
                } else {
                    send(new_socket, file_not_exist, sizeof(file_not_exist), 0);
                    continue;
                }

                sleep(1);
 // send file
                char file_length[1024];
                char *file_content = (char *)malloc(sizeof(char) * 65536);
                char file_to_open[1024];

                memset(file_length, 0, sizeof(file_length));
                memset(file_to_open, 0, sizeof(file_to_open));
                
                sprintf(file_to_open, "FILES/%s", file_to_download);

                FILE *fptr;
                fptr = fopen(file_to_open, "r");

                fseek(fptr, 0, SEEK_END);
                long fsize = ftell(fptr);
                rewind(fptr);

                fread(file_content, 1, fsize, fptr);

                fclose(fptr);

 // send file size
                sprintf(file_length, "%ld", fsize);
                send(new_socket, file_length, sizeof(file_length), 0);
                sleep(1);

// send file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                    send(new_socket, buffer, sizeof(buffer), 0);
                }

```

### Subsoal 1e
client juga dapat menghapus file yang tersimpan di server. Akan tetapi, Keverk takut file yang dibuang adalah file yang penting, maka file hanya akan diganti namanya menjadi ‘old-NamaFile.ekstensi’. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus.

### Pada server
```
//Memastikan input dari user adalah delete//
else if (strncmp(buffer, "delete", 6) == 0) {
                printf("user delete\n");

                char file_to_delete[50];
                sprintf(file_to_delete, "%.*s", strlen(buffer) - 7, buffer + 7);
                delete(file_to_delete);
//Menambahkan running.log
                FILE *fptr;
                fptr = fopen("running.log", "a");

                fprintf(fptr, "Hapus : %s (%s:%s)\n", file_to_delete, user_id, user_pwd);

                fclose(fptr);

                memset(file_to_delete, 0, sizeof(file_to_delete));
```

### Pada Client
```
else if (strncmp(input, "delete", 6) == 0) {
            send(sock, input, sizeof(input), 0);
```

### Subsoal 1f
Client dapat melihat semua isi files.tsv dengan memanggil suatu perintah yang bernama see. Output dari perintah tersebut keluar dengan format. 

### Pada client

- Membuat fungsi untuk memisahkan extension file
```
char *file_ext(char *file) {
    char *p = strchr(file, '.');
    if (p == NULL) return "exe";
    if (p - file == 6) {
        p = strchr(p+1, '.');
    }

    char *ext;
    ext = (char *)malloc(sizeof(char*) * 50);
    sprintf(ext, "%.*s", strlen(file) - (p - file + 1), p + 1);
    for (size_t i = 0; i < strlen(ext); ++i) {
        *(ext + i) = tolower(*(ext + i));
    }
    return ext;
}
```

- Melihat file yang ada di server
```
char *see(struct file files[], int n) {
    char *buf;
    buf = (char *)malloc(sizeof(char) * 1024);
    memset(buf, 0, sizeof(buf));

    for (int i = 0; i < n; ++i) {
        sprintf(files[i].ext, "%s", file_ext(files[i].path));
        char *p = strstr(files[i].path, files[i].ext);
        if (p == NULL) {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        } else {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        }
        
        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, files[i].year, files[i].ext, files[i].path);
    }

    return buf;
}
```

### Pada Client
```
else if (strncmp(buffer, "see", 3) == 0) {
                printf("user see\n");

                struct file *files = parse_files();
                char *files_tsv = (char *)malloc(sizeof(char) * 1024);

                sprintf(files_tsv, "%s", see(files, n));
                send(new_socket, files_tsv, strlen(files_tsv), 0);
```

### Subsoal 1g
Aplikasi client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Format output seperti format output f.

### Pada server
```
//Memastikan input user adalah perintah find
else if (strncmp(buffer, "find", 4) == 0) {
                printf("user find\n");

                char file_to_find[50];
                sprintf(file_to_find, "%.*s", strlen(buffer) - 5, buffer + 5);

                char *buf = (char *)malloc(sizeof(char) * 1024);
                memset(buf, 0, sizeof(buf));

                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) {
                    char *p;
// Menggunakan fungsi strstr untuk mencari string didalam stringm, apabila ketemu maka gunakan fungsi sprintf
                    p = strstr(files[i].name, file_to_find);
                    if (p) {
                        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, files[i].year, files[i].ext, files[i].path);
                    }
                }

// Jika tidak ada kata yang dimasukkan maka keluarkan file_pattern_not_found
                if (strlen(buf) == 0) {
                    send(new_socket, file_pattern_not_found, strlen(file_pattern_not_found), 0);
                } else {
                    send(new_socket, buf, strlen(buf), 0);
                }
            } else {
                break;
            }
```

### Pada client
```
else if (strncmp(input, "find", 4) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
        } else {
            send(sock, input, sizeof(input), 0);
            break;
        }
```

### Subsoal 1h
Dikarenakan Keverk waspada dengan pertambahan dan penghapusan file di server, maka Keverk membuat suatu log untuk server yang bernama running.log. 

**Sudah di jawab pada fungsi add (subsoal 1c) dan fungsi delete (subsoal 1e)

## Kendala yang dihadapi
1. Soal sangat kompleks dan membutuhkan banyak materi dimana kita harus belajar mandiri (dari internet)
2. Tidak diajarkan multiuser, sehingga dalam mengerjakan soal ini hanya menggunakan single user saja (soal 1a tidak sempurna)
3. Soal 1a - 1h saling terkait, jadi kalau tidak bisa mengerjakan salah satu maka akan kesulitan untuk mengerjakan soal selanjutnya

## Screenshot pengerjaan
- Menyambungkan server dan client terlebih dahulu
<img width="960" alt="1_konek server" src="https://user-images.githubusercontent.com/36225278/119238208-5a8cbe00-bb6b-11eb-9e3d-ed47b197e9d3.png">

- Melakukan register user
<img width="960" alt="2_register" src="https://user-images.githubusercontent.com/36225278/119238292-e30b5e80-bb6b-11eb-83cb-c5ebe8ab306a.png">

- Mencoba failed attempt login
<img width="960" alt="3_login failed" src="https://user-images.githubusercontent.com/36225278/119238303-f3bbd480-bb6b-11eb-9c46-a6ecb54b55a4.png">

- Mencoba login sukses
<img width="960" alt="4_login sukses" src="https://user-images.githubusercontent.com/36225278/119238318-00402d00-bb6c-11eb-89f7-dbe308500bff.png">


- 5 file yang telah disiapkan di dalam folder client
<img width="960" alt="5_5 file client" src="https://user-images.githubusercontent.com/36225278/119238346-2fef3500-bb6c-11eb-97e5-eaaa3754eb35.png">

- Mencoba command add
<img width="960" alt="6_add command" src="https://user-images.githubusercontent.com/36225278/119238382-50b78a80-bb6c-11eb-9c34-ee92e6570fb4.png">

- Melakukan command add ke semua file dari client ke server (karena minimal di soal 5 data)
<img width="960" alt="7_semua add command" src="https://user-images.githubusercontent.com/36225278/119238392-5f05a680-bb6c-11eb-87a5-fbdf0b18d4f0.png">

- Mencoba command see
<img width="960" alt="8_see command" src="https://user-images.githubusercontent.com/36225278/119238409-72b10d00-bb6c-11eb-8808-e0fc078f4893.png">

- Mencoba command delete
<img width="960" alt="9_delete command" src="https://user-images.githubusercontent.com/36225278/119238438-8197bf80-bb6c-11eb-86bd-a6215355dafb.png">

- Mencoba command find
<img width="960" alt="9_find command" src="https://user-images.githubusercontent.com/36225278/119238454-92e0cc00-bb6c-11eb-9607-a0046cb06b69.png">

- Mencoba command download
<img width="960" alt="12_download sukses" src="https://user-images.githubusercontent.com/36225278/119238469-a2f8ab80-bb6c-11eb-98a0-b86afd7bdd2c.png">

- File akun.txt
<img width="960" alt="akun" src="https://user-images.githubusercontent.com/36225278/119238547-261a0180-bb6d-11eb-9694-1f63784ea6a6.png">

- File running.log
<img width="960" alt="15_runninglog" src="https://user-images.githubusercontent.com/36225278/119238488-b60b7b80-bb6c-11eb-8b33-18c182471592.png">


## Soal 2


### SubSoal 2a. 
Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

### Penyelesaian 2a
1. Pertama kita buat matriks dengan ukuran 4x3 dan matrisk ukuran 3x6 dengan menggunakan perulangan
```
int matrixA[4][3], matrixB[3][6];

printf("Input Matrix A:\n");
    for (int i=0; i<4; i++) {
        for (int j=0; j<3; j++) {
            scanf("%d", &matrixA[i][j]);
        }
    }

    printf("Input Matrix B:\n");
    for (int i=0; i<3; i++) {
        for (int j=0; j<6; j++) {
            scanf("%d", &matrixB[i][j]);
        }
    }
```
2. Seteleh dibuat kita buat matrik hasil dari perkalian 2 matriks  dengan ukuran yaitu 4x6
```
int (*value)[4][6];

```
3. Setelah dibuat simpan hasil perkalian pada variabel temp lalu simpan kepada matriks hasil setiap kolom dan barisnya mengguankan perulangan.
```
int tempRest;
    for (int i=0; i<4; i++) {
        for (int j=0; j<6; j++) {
            tempRest = 0;
            for (int k=0; k<3; k++) {
                tempRest += matrixA[i][k] * matrixB[k][j];
            }
            (*value)[i][j] = tempRest;
        }
    }
```
4. Cetak Hasil nya dengan perulangan juga 
```
 printf("Output Program a :\n");
    for (int i=0; i<4; i++) {
        for (int j=0; j<6; j++) {
            printf("%d", (*value)[i][j]);
            if (j < 5) printf(" ");
            else printf("\n");
        }
    }
```

### SubSoal 2b
Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya matri(dari paling besar ke paling kecil) (Catatan!: gunakan thread untuk perhitungan di setiap cel).

```
If a >= b  -> a!/(a-b)!
If b > a -> a!
If 0 -> 0

```

### Penyelesaian 2b

1. Membuat key id untuk share memory dengan soal2a.c 
```
key_t key = 1234;

```
2. Membuat matriks baru untuk menyimpan matriks baru dengan ukuran yang sama dengan output soal2a.c yaitu 4x6
```
int matrixBaru[4][6];
```
3. Lalu deklarasikan hasil dari output soal2a.c dan variabel r dan c untuk size memory
```
int (*value)[4][6];
int r = 4, c = 6;

```
4. Medeklarasikan share memory id 
```
int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
```
5. Membuat max untuk size memory dan membuat thread
```
int max = r * c;
    pthread_t *threads;
    threads = (pthread_t*)malloc(max * sizeof(pthread_t));

```
6. Membuat perulangan untuk perhitungan kedua matriks dan melakukan pemanggilan fungsi faktorial di dalam perulangan dengan memasukan output dari soal2a ke data[0]

```
for (int i=0; i<r; i++) {
        for (int j=0; j<c; j++) {
            data = (int *)malloc((max)*sizeof(int));

            printf("Output Program a :\n");
            for (int i=0; i<4; i++) {
                 for (int j=0; j<6; j++) {
                 printf("%d", (*value)[i][j]);
                 if (j < 5) printf(" ");
                 else printf("\n");
        }
    }            data[0] = (*value)[i][j];
            data[1] = matrixBaru[i][j];
            pthread_create(&threads[count++], NULL, factorial, (void*)(data));
        }
    }

```

### SubSoal 2c

Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5” (Catatan!: Harus menggunakan IPC Pipes)

### Penyelesaian 2c

Fork pertama : Menerima input dari stdin ps aux kemudian dikirim ke pipe1

Fork kedua : Menerima input dari pipe1 kemudian kemudian diconcatenated dengan sort -nrk 3,3 dan dituliskan pada pipe2

Fork ketiga : Menerima input dari pipe2 kemudian diconcatenated dengan head -5 dan dikirim ke stdout

##  Hasil Output Program
![16538](https://user-images.githubusercontent.com/36225278/119238562-447ffd00-bb6d-11eb-8e4b-aa8585161efc.jpg)

## Soal 3

### 3a. Program menerima opsi -f , jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 

```
int main(int argc, char* argv[])
{
	if (strcmp(argv[1], "-f") == 0){
		pthread_t tid[argc-2];
		for(int i=2; i<argc; i++){
			pthread_create(&tid[i], NULL, move, (void *) argv[i]);
			
			if (!flag)
				printf ( "File %d : Berhasil Dikategorikan\n",i-1);
			
			else
				printf( "File %d : Sad, gagal :(\n", i-1 );
		}

		for(int j=2; j<argc; j++) 
			pthread_join(tid[j],NULL);
	}
```

Dimulai dengan fungsi main dengan parameter argc dan argv. argc sendiri merupakan argument count yang berisi jumlah argumen yang dipassing ke program tersebut dan argv merupakan array satu dimensi yang berisikan argumen argumen tersebut.

Lalu kita gunakan kondisi untuk mengecek argumen pertama apakah `-f` dengan `strcmp`. Setelah itu mendeklarasikan thread dan menggunakan loop for untuk membuat thread dari masing masing argumen dan berjalan pada fungsi `move`.

```
int flag;
void *move(void *argv){
	char *path;
	char cwd[500], folder[500], goalpath[500]; 
	
	path = (char *)argv;	
	getcwd(cwd, sizeof(cwd));

	char* file = basename(path);
	strcpy(folder, getext(file));
	
	if (strcmp(folder, "Hidden") != 0 && strcmp(folder,"Unknown") != 0)
	for(int i = 0; i < strlen(folder); i++)
		folder[i] = tolower(folder[i]);
	
	
	strcpy(goalpath,"");
	strcat(goalpath, cwd);
	strcat(goalpath, "/");
	strcat(goalpath,folder);
	makedir(goalpath);
	
	strcat(goalpath, "/");
	strcat(goalpath, file);

	flag = rename(path , goalpath); 
}
```
Pada fungsi move, di sini telah dipassingkan argumen tadi yang berupa path to file. Lalu dengan `getcwd` kita bisa mengetahui working directory saat ini dan menggunakan `basename()` untuk mendapatkan nama filenya saja dari pathtofile. Kemudian untuk mencari extensi dari file tersebut maka dipanggilah fungsi `getext()` yang telah dibuat seperti ini.

```
char *getext(char *filename) {
    if(filename[0] == '.') 
	return "Hidden"; 
	
    char *temp = strchr(filename, '.');
    if(!temp) 
	return "Unknown";

    return temp + 1;
}
```
Pada fungsi getext ini, yang dilakukan pertama kali adalah mengecek indeks paling pertama, apakah itu berupa titik (.) atau tidak. Jika iya maka file itu merupakan hidden file dan akan mereturn "Hidden". Lalu jika tidak, akan dilakukan pengecekan lagi dengan `strchr` untuk mendapatkan ".(ekstensi)" dari file tersebut. Jika tidak ditemukan (.), maka akan mereturn Unknown dan jika iya maka mereturn string tersebut + 1 (maksudnya disini, dikarenakan temp berisi .ekstensi maka dengan adanya +1 akan tersisa ekstensi saja).

### Kembali lagi ke move

Setelah mendapatkan ekstensi file, maka akan di copy ke array folder yang sudah disiapkan dengan `strcpy`. Kemudian akan dilakukan pengulangan for untuk mengecilkan semua character dari ekstensi tersebut, kecuali `Unknown` dan `Hidden` karena soal meminta seperti itu. Kemudian mengassign goalpath dengan "workdir/folder" yang sudah kita dapatkan sebelumnya dan membuat folder dengan fungsi `makedir()`.
```
void makedir(char *dir) {
	struct stat st;
	if (stat(dir, &st) == -1) 
		mkdir(dir, 0777);
}
```
Pada fungsi makedir, akan dilakukan pengecekan properti direktori dengan stat() dari struct stat. Jika -1 berarti belum ada dan maka akan dibuat foldernya dengan mkdir(dir, 0777). Disini 0777 mengindikasikan permission access.

### Kembali lagi ke move

Lalu setelah itu, goalpath akan ditambahkan /file, dan me rename path seperti goalpath yang merupakan pemindahan path file tersebut(dikategorikan) serta mengassign value rename tersebut ke variable `flag`. Jika rename berhasil maka value yang dihasilkan adalah 0.

### Kembali lagi ke main

Terdapat kondisi if else untuk indikasi keberhasilan pengkategorian. Maka akan dicek nilai flag yang tadi, jika flag=0 maka berhasil, selain itu gagal. Terakhir, seperti biasa ada pthread join untuk menunggu thread. <br>

![3_3_1](https://user-images.githubusercontent.com/81413071/119229836-f0145780-bb43-11eb-8b50-73942ff059b9.PNG) <br />
![3_3_2](https://user-images.githubusercontent.com/81413071/119229837-f1de1b00-bb43-11eb-933e-285b8b5c30c8.PNG) <br />
![3_3_3](https://user-images.githubusercontent.com/81413071/119229839-f276b180-bb43-11eb-97ca-d69aa1f33915.PNG) <br />
![3_3_4](https://user-images.githubusercontent.com/81413071/119229840-f276b180-bb43-11eb-978b-ae9a2ace6787.PNG) <br />

Berhasil dikategorikan. <br />

![3_3_5](https://user-images.githubusercontent.com/81413071/119229842-f30f4800-bb43-11eb-9dbf-9f8bc71f1055.PNG) <br />
![3_3_6](https://user-images.githubusercontent.com/81413071/119229844-f3a7de80-bb43-11eb-9e4e-ea79bdbe9348.PNG) <br />
![3_3_7](https://user-images.githubusercontent.com/81413071/119229845-f4407500-bb43-11eb-8164-20092c240627.PNG) <br />
![3_3_8](https://user-images.githubusercontent.com/81413071/119229846-f4407500-bb43-11eb-9dde-3c4ea93a2708.PNG) <br />



### 3b. Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. 

Melanjutkan if dari soal 3a, maka untuk 3b adalah seperti berikut
```
else if(strcmp(argv[1], "-d") == 0){

		if(check(argv)) {
			traverse(argv[2]);
			printf("Direktori sukses disimpan!\n");
			}
		else
			printf("Yah, gagal disimpan :(\n");
	}
```
Sama seperti 3a, program melakukan pengecekan argumen `-d`. Lalu jika iya, maka akan terdapat if else lagi. Jika check(argv)==1 maka akan memanggil fungsi traverse() dan print sukses setelah itu, jika tidak maka print gagal. Check() merupakan fungsi untuk mengecek apakah direktori ada atau tidak atau tidak bisa dibuka.
```
int check(char *argv){
DIR* dir = opendir(argv);
if (dir) {
    return 1;
    closedir(dir);
} 
else if (ENOENT == errno) 
    return 0;
else 
    return 0;
}

```

```
void traverse(char *argv){
	int k=2; 
	struct dirent *dp;
   	DIR *dir = opendir(argv);
	
	pthread_t thread[1000];
	
	while ((dp = readdir(dir)) != NULL) {
		char path[300];
		
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
			if(dp->d_type == DT_REG) {
				if (strcmp(dp->d_name, "soal3") == 0 || strcmp(dp->d_name, "soal3.c") == 0)
					continue;
				strcpy(path,"");
				strcat(path, argv);
				strcat(path, "/");
				strcat(path, dp->d_name);

				pthread_create(&thread[k], NULL, move, (void *) path);
				pthread_join(thread[k], NULL);
				k++;
			}
			else if(dp->d_type == DT_DIR) {
				struct dirent *ep;
   				DIR *dp1 = opendir(argv);
				char path1[500];
				
				strcpy(path1,"");
				strcat(path1, argv);
				strcat(path1, "/");
				strcat(path1, dp->d_name);
				traverse(path1);
   				closedir(dp1);
			}
		}
	}

	closedir(dir);
}

```
Pada fungsi traverse, ini sama seperti traversing pada umumnya, cuma kita menambahkan pthread. Tidak hanya file saja, kita juga menggunakan kondisi saat traversing. Yaitu DT_REG dan DT_DIR. DT_REG jika itu merupakan regular file, dan DT_DIR jika itu merupakan direktori. Jika itu file, maka akan dibuatkan pathnya dan akan dipassing untuk create pthread untuk selanjutnya dipindahkan dengan fungsi `move` dan dijoinkan (seperti soal3a). Jika itu direktori, maka akan dibuatkan pathnya dan akan dipassing untuk merekursi kembali fungsi traverse untuk mentraverse isi direktori ini. <br />

![3_3_9](https://user-images.githubusercontent.com/81413071/119229847-f4d90b80-bb43-11eb-8357-b2ce6058a13e.PNG) <br />
![3_3_10](https://user-images.githubusercontent.com/81413071/119229848-f571a200-bb43-11eb-9e97-14795ec1147b.PNG) <br />
![3_3_11](https://user-images.githubusercontent.com/81413071/119229849-f571a200-bb43-11eb-8ce9-3dcd54b6de52.PNG) <br />

### 3c. Selain menerima opsi-opsi di atas, program ini menerima opsi *.
Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

```
else if (strcmp(argv[1], "*") == 0) {
		char cwd[500];
		getcwd(cwd, sizeof(cwd));
		traverse(cwd);	
	}
```
Sama seperti soal3b, cuma ini mengkategorikan seluruh file yang ada di working directory. Maka menggunakan `getcwd` , dan langsung memanggil traverse.

![3_3_12](https://user-images.githubusercontent.com/81413071/119229850-f60a3880-bb43-11eb-9549-81a6101a8fd5.PNG) <br />
![3_3_13](https://user-images.githubusercontent.com/81413071/119229851-f6a2cf00-bb43-11eb-8b40-ad7b1c751ae2.PNG) <br />
![3_3_14](https://user-images.githubusercontent.com/81413071/119229853-f6a2cf00-bb43-11eb-8d55-590137c82774.PNG) <br />

### 3d. Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.
Sudah di atas. <br />

![3_3_16](https://user-images.githubusercontent.com/81413071/119230949-b560ee00-bb48-11eb-9050-84ec0b47fa0a.PNG) <br />
![3_3_15](https://user-images.githubusercontent.com/81413071/119230952-b6921b00-bb48-11eb-81d5-1652faff2d50.PNG) <br />

### 3e. Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.
Sudah di atas.

### Kendala 
1. Pusing
2. ETS
3. Masih bingung kondisi sukses gagal
4. Segmentation fault (core dumped)
5. gatau lupa banyak


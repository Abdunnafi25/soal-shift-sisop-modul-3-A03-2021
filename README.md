# soal-shift-sisop-modul-3-A03-2021

Anggota : 

### 1. Naufal Fajar  I  05111940000007
### 2. Johnivan Aldo S  05111940000051 
### 3. Abdun Nafi'      05111940000066

## Soal 1

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

Screenshot menyusul

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

## Soal 3

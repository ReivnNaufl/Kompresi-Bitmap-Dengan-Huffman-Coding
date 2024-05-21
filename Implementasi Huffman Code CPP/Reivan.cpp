#include <stdio.h>
#include <stdlib.h>
#include "Reivan.h"

//Fungsi untuk membuat node baru bertipe node dengan menginisialisasikan value sesuai dengan parameter 
nAddress createNode(infotype infoIns) {
	//deklarasi var dan insialisasi
	nAddress nodePtr = (nAddress)malloc(sizeof(node));

	
	if (nodePtr != NULL) {//jika malloc berhasil
		//insialisasi subvar
		freq(nodePtr) = 1;
		info(nodePtr) = infoIns;
		left(nodePtr) = NULL;
		right(nodePtr) = NULL;

		return nodePtr;
	}
	else {//jika gagal
		printf("ALOKASI MEMORI GAGAL!");
		exit(1);
	}
}

//Fungsi untuk membuat node baru bertipe queue dengan menginisialisasikan value sesuai dengan parameter 
qAddress createQueueNode(nAddress nodePtr) {
	//deklarasi var dan insialisasi
	qAddress qPtr = (qAddress)malloc(sizeof(queue));

	if (qPtr != NULL) {//jika malloc berhasil
		//insialisasi subvar
		pNode(qPtr) = nodePtr;
		next(qPtr) = NULL;

		return qPtr;
	}
	else {//jika gagal
		printf("ALOKASI MEMORI GAGAL!");
		exit(1);
	}
}

//fungsi unruk mengecek apakah byte sudah ada dalam suatu list atau tidak
bool isDupe(qAddress head, infotype check) {
	//deklarasi var dan insialisasi
	qAddress p = head;

	while (p != NULL) {//jika belum akhir queue
		if (info(pNode(p)) == check) {//jika info sama dengan check
			return true;
		}
		else {
			p = next(p);
		}
	}
	return false;
}

//Procedure untuk untuk memasukan node baru kedalam akhir queue
void queueInsert(qAddress* head, qAddress newNode) {
	//deklarasi var dan insialisasi
	qAddress p = *head;

	if (*head == NULL) {//jika queue kosong
		*head = newNode;
	}
	else {//jika tidak kosong
		while (next(p) != NULL) {//jika elemen selanjutnya belum kosong
			p = next(p);
		}
		next(p) = newNode;
	}
}

//procedure untuk menambahkan freq node dengan info container 
void freqPlusOne(qAddress head, infotype container) {
	//deklarasi var dan insialisasi
	qAddress p = head;

	while (info(pNode(p)) != container) {//hingga info elemen sama dengan container
		p = next(p);
	}
	freq(pNode(p)) += 1;
}

//Function untuk membuat queue berisi byte byte unik suatu file dan menghitung freq amsing masing byte
qAddress createList(char *fileName) {
	//deklarasi var
	FILE* fFile;
	nAddress nodePtr;
	qAddress qPtr = NULL, head = NULL;
	infotype byteContainer;

	//buka file
	fFile = fopen(fileName, "rb");
	if (fFile == NULL) {//jika fopen gagal
		printf("GAGAL MEMUAT FILE!");
		exit(1);
	}

	//baca 1 byte dari file
	byteContainer = fgetc(fFile);

	while (!feof(fFile)) {//hingga akhir file
		if (isDupe(head, byteContainer)) {//jika byte duplikat
			freqPlusOne(head, byteContainer);//freq + 1
		}
		else {//jika byte unik
			//buat node baru & insert ke queue
			nodePtr = createNode(byteContainer);
			qPtr = createQueueNode(nodePtr);
			queueInsert(&head, qPtr);
		}
		byteContainer = fgetc(fFile);
	}

	return head;
}

//Procedure untuk menampilkan queue ke layar
void printList(qAddress head) {
	//deklarasi var dan inisialisasi
	qAddress p = head;
	int total = 0;

	while (p != NULL) {//hingga elemen terakhir
		printf("%x %d\n", info(pNode(p)), freq(pNode(p)));//print info dan freq
		total += freq(pNode(p));//tambah freq ke total
		p = next(p);
	}
	printf("\n%d\n-----\n\n", total);//print pembataas
}

//Procedure untuk mengesort ascending queue berdasarkan freq dan byte
void mergeSort(qAddress *head) {
	//deklarasi var dan beberapa insialisasi
	qAddress pHead = *head, left, right;

	if ((pHead == NULL) || (next(pHead) == NULL)) {//kasus base/stop
		return;
	}

	//pecah queue jadi 2
	splitList(pHead, &left, &right);

	//rekursif subqueue kiri dan kanan 
	mergeSort(&left);
	mergeSort(&right);

	//sort dan gabungkan sublist kiri dan kanan
	*head = merge(left, right);
}

//Procedure untuk memecah queue menjadi 2
void splitList(qAddress src, qAddress* lSubList, qAddress* rSubList) {
	//deklarasi var dan insialisasi
	qAddress mid = src, end = next(src);

	while (end != NULL) {//jika end belum ke elemen terakhir
		//majukan end 2 kali dan mid 1 kali
		end = next(end);
		if (end != NULL) {
			end = next(end);
			mid = next(mid);
		}
	}

	*lSubList = src;//sublist kiri dimulai dari elemen pertama
	*rSubList = next(mid);//sublist kanan dimulai dari elemen paling tengah
	next(mid) = NULL;//pisah queue
}

//Functionuntuk manggabungkan 2 queue secara sorted ascending berdasarkan freq dan byte
qAddress merge(qAddress lSubList, qAddress rSubList) {
	//deklarasi var dan insialisasi
	qAddress merged = NULL, lPtr = lSubList, rPtr = rSubList, temp = NULL;

	while ((lPtr != NULL) and (rPtr != NULL)) {//hingga salah satu sublist kosong
		if (freq(pNode(lPtr)) < freq(pNode(rPtr))) {//jika freq lPtr lebih kecil
			//masukan lPtr ke queue
			queueInsert(&merged, lPtr);
			temp = lPtr;
			lPtr = next(lPtr);
			next(temp) = NULL;
		}
		else if (freq(pNode(lPtr)) > freq(pNode(rPtr))) {//jika freq lPtr lebih besar
			//masukan rPtr ke queue
			queueInsert(&merged, rPtr);
			temp = rPtr;
			rPtr = next(rPtr);
			next(temp) = NULL;
		}
		else {//jika freq sama
			if (info(pNode(lPtr)) < info(pNode(rPtr))) {//jika info lPtr lebih kecil
				//masukan lPtr ke queue
				queueInsert(&merged, lPtr);
				temp = lPtr;
				lPtr = next(lPtr);
				next(temp) = NULL;
			}
			else {//jika info lPtr lebih besar
				//masukan rPtr ke queue
				queueInsert(&merged, rPtr);
				temp = rPtr;
				rPtr = next(rPtr);
				next(temp) = NULL;
			}
		}
	}

	//masukan sisa rPtr ke queue
	if (lPtr == NULL) {
		queueInsert(&merged, rPtr);
	}

	//masukan sisa lPtr ke queue
	if (rPtr == NULL) {
		queueInsert(&merged, lPtr);
	}

	//retunr hasil merge dan sort
	return merged;
}
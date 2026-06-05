#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

// projek uts: sistem manajemen stok barang minimarket
// materi yang harus ada: variable, tipe data, array, structure, pointer, single linked list, double linked list, 

struct Barang {
    int id;
    string kode;
    string nama;
    int kategori_id;
    double harga_beli;
    double harga_jual;
    int stok;
    int stok_minimum;
    int satuan_id;
    string tanggal_kadaluarsa;
    int supplier_id;
    bool status;
};

struct Kategori { int id; string nama; };
struct Satuan { int id; string nama; };
struct Supplier { int id; string nama; };

struct NodeBarang {
	Barang data;
	NodeBarang* next;
};

NodeBarang* head = NULL;
NodeBarang* tail = NULL;

vector<Kategori> list_kat;
vector<Satuan> list_sat;
vector<Supplier> list_sup;

Barang br;
Kategori kat;
Satuan sat;
Supplier sup;

int next_id_barang = 1; // auto increment id

void tambah_barang();
void edit_barang();
void tampilkan_barang();

int main() {
	tambah_barang();
	tampilkan_barang();
}

bool is_kosong() {
    return head == NULL;
}

void tambah_barang() {
	int jumlah;
	
	cout << "Jumlah barang yang ingin di-input: ";
	cin >> jumlah;
	for (int i = 0; i < jumlah; i++) {
        cout << "\nMasukkan data ke-" << (i+1) << ": " << endl;
    	
		br.id = next_id_barang++; // id
		cout << "Nama Barang: "; cin.ignore(); getline(cin, br.nama);
		cout << "Harga Beli: "; cin >> br.harga_beli;
		cout << "Harga Jual: "; cin >> br.harga_jual;
		cout << "Stok: "; cin >> br.stok;
		
		NodeBarang* new_node = new NodeBarang();
	    new_node->data = br;
	    new_node->next = NULL;
	    
	    if(is_kosong()) {
        	head = new_node;
        	tail = new_node;
    	} else {
        	tail->next = new_node;
        	tail = new_node;
    	}
    	cout << "[Barang ke-" << (i+1) << " berhasil ditambahkan!]" << endl;
    }
}

 void tampilkan_barang() {
	if (is_kosong()) {
    	cout << "List Barang kosong." << endl;
    	return;
	}

    NodeBarang* current = head;
    int no = 1;
    
    cout << "\n=== DAFTAR BARANG ===\n";
    while(current != NULL) {
        cout << no++ << ". ";
        cout << "ID: " << current->data.id;
        cout << " | Nama: " << current->data.nama;
        cout << " | Stok: " << current->data.stok;
        cout << " | Harga: Rp" << current->data.harga_jual << endl;
        
        current = current->next;  // pindah ke node berikutnya
    }
      
}

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <limits> 
using namespace std;

// projek uts: sistem manajemen stok barang minimarket
// materi yang harus ada: variable, tipe data, array, structure, pointer, single linked list, double linked list, 

struct Barang {
    int id;
    string kode_barang;
    string nama;
    string kategori;
    double harga_beli;
    double harga_jual;
    int stok;
    string satuan;
    string tanggal_kadaluarsa;
    string supplier;
    bool status;
};

struct NodeBarang {
	Barang data;
	NodeBarang* next;
};

NodeBarang* head = NULL;
NodeBarang* tail = NULL;
int next_id = 1; // auto increment id

void tambah_barang();
void edit_barang();
void tampilkan_barang();

Barang br;

int main() {
	tambah_barang();
	tampilkan_barang();
}

bool is_kosong() {
    return head == NULL;
}

string generate_kode_barang(int id) {
	char buffer[20];
    sprintf(buffer, "BRG%04d", id); 
    return string(buffer);
}

void tambah_barang() {
    int jumlah;
    
    cout << "Jumlah barang yang ingin di-input: ";
    cin >> jumlah;
    cin.ignore(); 
    
    cout << "\n=== TAMBAH BARANG ===\n";
    
    for (int i = 0; i < jumlah; i++) {
        cout << "\nMasukkan data ke-" << (i+1) << ": \n";
        
        br.id = next_id++;
        br.kode_barang = generate_kode_barang(br.id);
        br.status = true;
        
        cout << "Kode barang (otomatis): " << br.kode_barang << endl;
        
        cout << "> Nama barang: "; 
        getline(cin, br.nama);
        
        cout << "> Kategori: "; 
        getline(cin, br.kategori);
        if(br.kategori.empty()) br.kategori = "Lainnya";
        
        cout << "> Harga beli: "; 
        cin >> br.harga_beli;
        
        cout << "> Harga jual: "; 
        cin >> br.harga_jual;
        
        cout << "> Stok awal: "; 
        cin >> br.stok;
        
        cout << "> Satuan (pcs/botol/pack): "; 
        cin >> br.satuan;
        if(br.satuan.empty()) br.satuan = "pcs";
        
        cout << "> Tanggal kadaluarsa (DD-MM-YYYY) [Enter jika tidak ada]: "; 
        cin.ignore();
        getline(cin, br.tanggal_kadaluarsa);
        if(br.tanggal_kadaluarsa.empty()) br.tanggal_kadaluarsa = "-";
        
        cout << "> Supplier: "; 
        getline(cin, br.supplier);
        if(br.supplier.empty()) br.supplier = "-";
        
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
        
        cout << "\n[Barang ke-" << (i+1) << " berhasil ditambahkan!]" << endl;
        cout << "\n" << string(100, '-') << endl;
    }
}

void tampilkan_barang() {
	if (is_kosong()) {
    	cout << "List Barang kosong." << endl;
    	return;
	}
	
	cout << "\n=== DAFTAR BARANG ===" << endl;
	cout << "\n" << string(100, '=') << endl;
    cout << left << setw(5) << "No"
         << setw(12) << "Kode"
         << setw(20) << "Nama"
         << setw(12) << "Kategori"
         << setw(10) << "Stok"
         << setw(10) << "Satuan"
         << "Harga Jual" << endl;
    cout << string(100, '-') << endl;
    
    NodeBarang* current = head;
    int no = 1;
    
    while(current != NULL) {
        cout << left << setw(5) << no++
             << setw(12) << current->data.kode_barang
             << setw(20) << current->data.nama
             << setw(12) << current->data.kategori
             << setw(10) << current->data.stok
             << setw(10) << current->data.satuan
             << "Rp" << fixed << setprecision(0) << current->data.harga_jual << endl;
        
        current = current->next;
    }
    cout << string(100, '=') << endl;      
}

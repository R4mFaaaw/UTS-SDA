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

// int main() {
// 	tambah_barang();
// 	tampilkan_barang();
// }

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

// Fitur Mengahapus barang 
void hapus_barang() {
    if (is_kosong()) {
        cout << "List barang kosong.\n";
        return;
    }

    string kode;
    char konfirmasi;

    cout << "\nMasukkan kode barang yang ingin dihapus: ";
    cin >> kode;

    NodeBarang* current = head;
    NodeBarang* prev = NULL;

    while (current != NULL && current->data.kode_barang != kode) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        cout << "Barang dengan kode " << kode << " tidak ditemukan.\n";
        return;
    }

    // tampilkan barang yang akan dihapus
    cout << "\nData yang akan dihapus:\n";
    cout << "Kode       : " << current->data.kode_barang << endl;
    cout << "Nama       : " << current->data.nama << endl;
    cout << "Stok       : " << current->data.stok << endl;
    cout << "Harga      : Rp" << current->data.harga_jual << endl;

    cout << "\nYakin ingin menghapus? (Y/n): ";
    cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y') {
        if (current == head) {
            head = head->next;
            if (head == NULL) {
                tail = NULL;
            }
        } 
        else {
            prev->next = current->next;

            if (current == tail) {
                tail = prev;
            }
        }

        delete current;
        cout << "Barang berhasil dihapus.\n";
    } 
    if(konfirmasi == 'n' || konfirmasi == 'N') {
        cout << "Penghapusan dibatalkan.\n";
    }
    else{
        cout <<"tidak valid hanya boleh y/n\n";
    }
}
// FItur Mencari barang
void cari_barang() {
    if (is_kosong()) {
        cout << "List barang kosong.\n";
        return;
    }

    int opsi;
    cout << "Cari barang berdasarkan:\n";
    cout << "1. Kode Barang\n";
    cout << "2. Nama Barang\n";
    cout << "Pilih mana: ";
    cin >> opsi;
    cin.ignore();

    NodeBarang* current = head;

    if (opsi == 1) {
        string kode;
        cout << "\nMasukkan kode barang yang ingin dicari: ";
        cin >> kode;

        while (current != NULL && current->data.kode_barang != kode) {
            current = current->next;
        }
    }
    else if (opsi == 2) {
        string cari;
        cout << "\nMasukkan nama yang ingin dicari: ";
        getline(cin, cari);

        while (current != NULL && current->data.nama != cari) {
            current = current->next;
        }
    }
    else {
        cout << "Tidak valid! Harus berupa angka 1/2\n";
        return;
    }

    if (current == NULL) {
        cout << "Barang tidak ditemukan.\n";
        return;
    }

    cout << "\nData yang dicari:\n";
    cout << "Kode               : " << current->data.kode_barang << endl;
    cout << "Nama               : " << current->data.nama << endl;
    cout << "Stok               : " << current->data.stok << endl;
    cout << "Kategori           : " << current->data.kategori << endl;
    cout << "Supplier           : " << current->data.supplier << endl;
    cout << "Tgl Kadaluarsa     : " << current->data.tanggal_kadaluarsa << endl;
    cout << "Harga              : Rp" << current->data.harga_jual << endl;
    cout << "Satuan             : " << current->data.satuan << endl;
}

// Fitur Update Barang
void update_barang(){
    if (is_kosong() == 1){
        cout << "Tidak ada barang yang dapat diupdate.\n";
    }
    else{
        string kode;
        cout << "\nKode barang yang ingin diupdate: ";
        cin >> kode;
        cin.ignore();

        NodeBarang *current;
        current = head;

        while (current != NULL){
            if (current->data.kode_barang == kode){
                break;
            }
            current = current->next;
        }

        if (current == NULL){
            cout << "Barang tidak ditemukan.\n";
        }
        else{
            int pilih;
            while (true){
                cout << "\nData ditemukan!";
                cout << "\n1. Update Nama";
                cout << "\n2. Update Kategori";
                cout << "\n3. Update Harga Jual";
                cout << "\n4. Update Stok";
                cout << "\nPilih: ";
                cin >> pilih;
                cin.ignore();

                if (pilih >= 1 && pilih <= 4){
                    break;
                }
                else{
                    cout << "\n Pilihan tidak valid, silakan coba lagi!";
                }
            }

            if (pilih == 1){
                cout << "\nNama baru: ";
                getline(cin, current->data.nama);
            }
            else if (pilih == 2){
                cout << "\nKategori baru: ";
                getline(cin, current->data.kategori);
            }
            else if (pilih == 3){
                cout << "\nHarga jual baru: ";
                cin >> current->data.harga_jual;
            }
            else if (pilih == 4){
                int tambah;
                cout << "\nTambah stok: ";
                cin >> tambah;
                current->data.stok = current->data.stok + tambah;
            }

            cout << "\nData berhasil diupdate!";
        }
    }
}

int main() {
    int pilihan;

    while (true) {
        cout << "===   SISTEM MANAJEMEN STOK   ===" << endl;
        cout << "1. Tambah Barang" << endl;
        cout << "2. Tampilkan Barang" << endl;
        cout << "3. Hapus Barang" << endl;
        cout << "4. Update Barang" << endl;
        cout << "5. Cari Barang"<<endl;
        cout << "6. Fitur LOG barang"<<endl;
        cout << "0. Keluar" << endl;
        cout << "--------------------------------------------"<<endl;
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        if (pilihan == 1) {
            tambah_barang();
        }
        else if (pilihan == 2) {
            tampilkan_barang();
        }
        else if (pilihan == 3) {
            hapus_barang();
        }
        else if (pilihan == 4){
            update_barang();
        }
        else if (pilihan == 5){
            cari_barang();

        }else if (pilihan == 6){
            
        }
        else if (pilihan == 0) {
            cout << "Terima kasih!.\n";
            break;
        }
        else {
            cout << "Pilihan tidak valid.\n";
        }
    }

    return 0;
}
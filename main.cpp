#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <limits> 
#include <cctype>
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
string toLowerCase(string text) {
    for (int i = 0; i < text.length(); i++) {
        text[i] = tolower(text[i]);
    }
    return text;
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

    NodeBarang* current = head;
    int no = 1;

    while(current != NULL) {
        cout << "No              : " << no++ << endl;
        cout << "Kode Barang     : " << current->data.kode_barang << endl;
        cout << "Nama Barang     : " << current->data.nama << endl;
        cout << "Kategori        : " << current->data.kategori << endl;
        cout << "Stok            : " << current->data.stok << endl;
        cout << "Satuan          : " << current->data.satuan << endl;
        cout << "Harga Jual      : Rp" << current->data.harga_jual << endl;
        cout << string(100, '-') << endl;

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

    while (current != NULL &&
               toLowerCase(current->data.kode_barang) != toLowerCase(kode)) {
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
void cari_barang() {
    if (is_kosong()) {
        cout << "List barang kosong.\n";
        return;
    }

    int opsi;
    cout << "\n=== CARI BARANG ===\n";
    cout << "1. Berdasarkan Kode Barang\n";
    cout << "2. Berdasarkan Nama Barang\n";
    cout << "Pilih opsi: ";
    cin >> opsi;
    cin.ignore();

    NodeBarang* current = head;

    if (opsi == 1) {
        string kode;
        cout << "\nMasukkan kode barang yang ingin dicari: ";
        cin >> kode;

        while (current != NULL &&
               toLowerCase(current->data.kode_barang) != toLowerCase(kode)) {
            current = current->next;
        }
    }
    else if (opsi == 2) {
        string cari;
        cout << "\nMasukkan nama barang yang ingin dicari: ";
        getline(cin, cari);

        while (current != NULL &&
               toLowerCase(current->data.nama).find(toLowerCase(cari)) == string::npos) {
            current = current->next;
        }
    }
    else {
        cout << "Pilihan tidak valid! Gunakan 1 atau 2.\n";
        return;
    }

    if (current == NULL) {
        cout << "\nBarang tidak ditemukan.\n";
        return;
    }


    cout << "\n=== HASIL PENCARIAN ===\n";
    cout << string(60, '=') << endl;

    cout << "Kode               : " << current->data.kode_barang << endl;
    cout << "Nama               : " << current->data.nama << endl;
    cout << "Stok               : " << current->data.stok << endl;
    cout << "Kategori           : " << current->data.kategori << endl;
    cout << "Supplier           : " << current->data.supplier << endl;
    cout << "Tgl Kadaluarsa     : " << current->data.tanggal_kadaluarsa << endl;
    cout << "Harga              : Rp" << current->data.harga_jual << endl;
    cout << "Satuan             : " << current->data.satuan << endl;

    cout << string(60, '=') << endl;
}

// Fitur Update Barang
void update_barang(){
    if (is_kosong() == 1){
        cout << "\nTidak ada barang yang dapat diupdate!\n";
    }
    else{
        tampilkan_barang();
        string kode;
        NodeBarang *current;

        while (true){
            cout << "\nMasukkan kode barang: ";
            cin >> kode;

            current = head;
            while (current != NULL){
                if (current->data.kode_barang == kode){
                    break;
                }
                current = current->next;
            }

            if (current != NULL){
                break;
            }
            else{
                cout << "\nKode barang tidak ditemukan, silakan input kembali!\n";
            }
        }

        int pilih;

        while (true){
            cout << "\n1. Update Nama";
            cout << "\n2. Update Kategori";
            cout << "\n3. Update Harga";
            cout << "\n4. Update Stok";
            cout << "\nPilih        : ";
            cin >> pilih;

            if (pilih >= 1 && pilih <= 4){
                break;
            }
            else{
                cout << "\nPilihan tidak valid, silakan input kembali!\n l";
            }
        }

        if (pilih == 1){
            string nama;
            cin.ignore();

            while (true){
                cout << "Nama baru      : ";
                getline(cin, nama);

                if (nama != ""){
                    current->data.nama = nama;
                    break;
                }
                else{
                    cout << "\nNama tidak boleh kosong, silakan input kembali!\n";
                }
            }
        }
        else if (pilih == 2){
            string kategori;
            cin.ignore();

            while (true){
                cout << "Kategori baru  : ";
                getline(cin, kategori);

                if (kategori != ""){
                    current->data.kategori = kategori;
                    break;
                }
                else{
                    cout << "\nKategori tidak boleh kosong, silakan input kembali!\n";
                }
            }
        }
        else if (pilih == 3){
            double harga;

            while (true){
                cout << "Harga baru     : ";
                cin >> harga;

                if (harga > 0){
                    current->data.harga_jual = harga;
                    break;
                }
                else{
                    cout << "\nHarga harus lebih dari 0, silakan input kembali!\n";
                }
            }
        }
        else if (pilih == 4){
            int jumlah;

            while (true){
                cout << "Stok sekarang: " << current->data.stok << endl;
                cout << "Tambah/Kurang stok (+/-): ";
                cin >> jumlah;

                if (current->data.stok + jumlah < 0){
                    cout << "\nStok tidak boleh minus, silakan input kembali!\n";
                }
                else{
                    current->data.stok = current->data.stok + jumlah;
                    break;
                }
            }
        }

        cout << "\nData berhasil diupdate!\n";
    }
}

int main() {
    int pilihan;

    while (true) {
        cout << "\n=========================================\n";
        cout << "     SISTEM MANAJEMEN STOK MINIMARKET\n";
        cout << "=========================================\n";
        cout << "1. Tambah Barang" << endl;
        cout << "2. Tampilkan Barang" << endl;
        cout << "3. Hapus Barang" << endl;
        cout << "4. Update Barang" << endl;
        cout << "5. Cari Barang"<<endl;
        cout << "6. Fitur LOG barang"<<endl;
        cout << "0. Keluar" << endl;
        cout << "--------------------------------------------"<<endl;
        cout << "Pilih menu: ";
        
        
        //validasi input pilihan menu
         if (!(cin >> pilihan)) {
            cout << "\nInput tidak valid! Harus berupa angka.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.get();
            continue;
        }

        cin.ignore();

        //validasi pilihan menu
        if (pilihan < 0 || pilihan > 6) {
            cout << "\nPilihan tidak valid!\n";
            cout << "Tekan ENTER untuk kembali ke menu...";
            cin.get();
            continue;
        }

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
        cout << "\nTekan ENTER untuk kembali ke menu...";
        cin.get();
    }

    return 0;
}